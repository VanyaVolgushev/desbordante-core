#include "candidate_prefix_tree.h"

#include <algorithm>
#include <iostream>
#include <stdexcept>

namespace kingfisher {
// Debug
// Helper to convert a dynamic_bitset to a human-readable string (MSB first).
inline std::string BitsetToString(boost::dynamic_bitset<> const& bs) {
    std::string s;
    s.reserve(bs.size());
    for (size_t i = 0; i < bs.size(); ++i) {
        // Print highest-index bit first
        s.push_back(bs[bs.size() - 1 - i] ? '1' : '0');
    }
    return s;
}

// Recursively prints all children of a node in ASCII form, including bitsets when available.
void PrintAsciiTreeChildren(Node const& node, size_t feat_count, std::string const& prefix) {
    std::vector<std::pair<OFeatureIndex, std::shared_ptr<Node>>> childNodes;
    for (OFeatureIndex i = 0; i < feat_count; ++i) {
        if (node.HasChild(i)) {
            childNodes.emplace_back(i, node.children.at(i));
        }
    }

    for (size_t i = 0; i < childNodes.size(); ++i) {
        bool isLast = (i + 1 == childNodes.size());
        std::cout << prefix << (isLast ? "└── " : "├── ");
        std::cout << "[" << childNodes[i].first << "] Node";

        if (auto bp = dynamic_cast<BranchableNode const*>(childNodes[i].second.get())) {
            std::cout << " [p:" << BitsetToString(bp->p_possible)
                      << " n:" << BitsetToString(bp->n_possible) << "]";
        }
        std::cout << std::endl;

        std::string childPrefix = prefix + (isLast ? "    " : "│   ");
        PrintAsciiTreeChildren(*childNodes[i].second, feat_count, childPrefix);
    }
}

// Entry point for printing an ASCII tree from the root.
void PrintAsciiTree(Node const& root, size_t feat_count) {
    std::cout << "Node";
    if (auto bp = dynamic_cast<BranchableNode const*>(&root)) {
        std::cout << " [p:" << BitsetToString(bp->p_possible)
                  << " n:" << BitsetToString(bp->n_possible) << "]";
    }
    std::cout << std::endl;
    PrintAsciiTreeChildren(root, feat_count, "");
}



// Undoes internal index order on the discovered NeARs then returns them
std::vector<model::NeARIDs> CandidatePrefixTree::GetNeARIDs(
        std::vector<FeatureIndex> const& frequency_order) const {
    std::vector<model::NeARIDs> real_nears;
    real_nears.reserve(k_best_.size());
    for (auto const& near : k_best_) {
        real_nears.push_back(near.UndoOrder(frequency_order));
    }
    return real_nears;
}

std::optional<BranchableNode> CandidatePrefixTree::MakeBranchableFromParents(
        NodeAdress adress_of_node_to_make) const {
    BranchableNode new_node{feat_count_, adress_of_node_to_make.Back()};
    auto const parent_adresses = adress_of_node_to_make.GetParents();

    for (auto const& parent_adress : parent_adresses) {
        std::optional<Node const* const> parent = GetNode(parent_adress);
        if (!parent.has_value()) {
            return {};
        }
        // Parent can never be a RoutingNode
        new_node.Intersect(dynamic_cast<BranchableNode const&>(*parent.value()));
        if (new_node.Pruned()) {
            return {};
        }
    }

    return new_node;
}

std::optional<Node* const> CandidatePrefixTree::GetNode(NodeAdress adress) {
    Node* current = &root_;
    while (!adress.Empty()) {
        auto next_index = adress.PopFront();
        if (current->HasChild(next_index)) {
            current = &current->GetChild(next_index);
        } else {
            return {};
        }
    }
    return current;
}

std::optional<Node const* const> CandidatePrefixTree::GetNode(NodeAdress adress) const {
    Node const* current =
            &root_;  // We have to use a pointer here because references can't be reseated
    while (!adress.Empty()) {
        auto next_index = adress.PopFront();
        if (current->HasChild(next_index)) {
            current = &current->GetChild(next_index);
        } else {
            return {};
        }
    }
    return current;
}

void CandidatePrefixTree::AddChildrenToQueue(NodeAdress adress) {
    for (NodeAdress child_adress : adress.GetChildren(feat_count_)) {
        bfs_queue_.emplace(child_adress);
    }
}

void CandidatePrefixTree::ConsiderRule(NodeAdress node_addr, OFeatureIndex cons_index,
                                       bool cons_positive, double parents_best) {
    model::NeARIDs near{node_addr.GetExceptFeat(cons_index), cons_index, cons_positive};
    near.p_value = get_p_(near);
    if (near.p_value >= max_p_ || near.p_value >= parents_best) {
        return;
    }
    if (topk_queue_.size() < max_rules_) {
        topk_queue_.push(std::move(near));
    } else if (near.p_value < topk_queue_.top().p_value) {
        topk_queue_.pop();
        topk_queue_.push(std::move(near));
        max_p_ = near.p_value;
    }
}

//
bool CandidatePrefixTree::ConsPossible(NodeAdress node_addr, OFeatureIndex cons_index,
                                       bool cons_positive, double best_measure) const {
    // Check if frequency requirements are satisifed
    if (!node_addr.Contains(cons_index) && get_frequency_(node_addr) < min_frequency_) {
        return false;
    }
    if (node_addr.Contains(cons_index) &&
        false/*get_frequency_(node_addr, cons_index, cons_positive) < min_frequency_*/) {
        return false;
    }
    // Check if lower bound requirements are satisifed
    double lower_bound;
    if (!node_addr.Contains(cons_index)) {
        if (get_frequency_(node_addr) <= 1.0/*get_frequency_(cons_index, cons_positive)*/) {
            lower_bound = lower_bound2_(node_addr, cons_index, cons_positive);
        } else {
            lower_bound = lower_bound1_(cons_index);
        }
    } else {
        lower_bound = lower_bound3_(node_addr, cons_index, cons_positive);
    }
    if (lower_bound > max_p_ || (lower_bound >= best_measure)) {
        return false;
    }
    return true;
}

// returns: whether the node exists after check
bool CandidatePrefixTree::CheckNode(NodeAdress node_addr) {
    // Create new node from parents on the previous layer
    OFeatureIndex adds_feature = node_addr.Back();
    auto parent_adress = node_addr.GetParent();
    auto maybe_parent_ptr = GetNode(parent_adress);
    if (!maybe_parent_ptr.has_value()) {  // direct parent was pruned
        return false;
    }
    auto& parent_ptr = maybe_parent_ptr.value();

    auto maybe_node = MakeBranchableFromParents(node_addr);
    if (!maybe_node.has_value()) {  // Intersection empty or one of parents was pruned
        return false;
    }
    auto& node = maybe_node.value();
    // Prune out impossible consequences in this node using lower bounds
    for (OFeatureIndex i = 0; i < feat_count_; i++) {
        if (node.p_possible[i])
            node.p_possible[i] = ConsPossible(node_addr, i, true, node.p_best[i]);
        if (node.n_possible[i])
            node.n_possible[i] = ConsPossible(node_addr, i, false, node.n_best[i]);
    }
    if (node.Pruned()) {
        return false;
    }
    // Evaluate rules in this node and save good ones
    for (OFeatureIndex i = 0; i < feat_count_; i++) {
        if (!node_addr.Contains(i)) continue;
        if (node.p_possible[i]) ConsiderRule(node_addr, i, true, node.p_best[i]);
        if (node.n_possible[i]) ConsiderRule(node_addr, i, false, node.n_best[i]);
    }
    // Check if a minimal rule was found (DISABLED WHILE GETFREQ IS NOT IMPLEMENTED)
    /*
    bool is_minimal = false;
    double frequency = get_frequency_(node_addr);
    is_minimal = frequency == 1.0;
    if (!is_minimal) {
        // TODO: cache frequencies
        for (auto parent_addr : node_addr.GetParents()) {
            if (frequency == get_frequency_(parent_addr)) {
                is_minimal = true;
                break;
            }
        }
    }
    // Prune out consequences in this node by minimality
    if (is_minimal) {
        for (OFeatureIndex i = 0; i < feat_count_; i++) {
            if (node_addr.Contains(i)) {
                continue;
            }
            node.p_possible[i] = false;
            node.n_possible[i] = false;
        }
    }
    */
    if (node.Pruned()) {
        return false;
    }
    for (auto parent_addr : node_addr.GetParents()) {
    }
    // Prune out consequences in parents using Lapis Philosophorum principle from paper

    // Add the new node to tree
    auto node_ptr = std::make_shared<BranchableNode>(std::move(node));
    auto node_ptr_upcasted = static_cast<std::shared_ptr<Node>>(node_ptr);
    parent_ptr->AddChild(adds_feature, node_ptr_upcasted);
    return true;
}

void CandidatePrefixTree::CheckDepth1() {
    for (auto& [node_feat, node_ptr] : root_.children) {
        auto& branchable_node = *std::static_pointer_cast<BranchableNode>(node_ptr);
        auto nodeAdress = NodeAdress{node_feat};
        // Check all possible branches' best-case p values
        for (OFeatureIndex child_feat = 0; child_feat < feat_count_; child_feat++) {
            bool lb1_ok = lower_bound1_(child_feat) < max_p_;
            bool lb2_ok_pos = lower_bound2_(nodeAdress, child_feat, true) < max_p_;
            bool lb2_ok_neg = lower_bound2_(nodeAdress, child_feat, false) < max_p_;
            branchable_node.p_possible[child_feat] = lb1_ok && lb2_ok_pos;
            branchable_node.n_possible[child_feat] = lb1_ok && lb2_ok_neg;
        }
        if (branchable_node.Pruned()) {
            // TODO: DELETE
        }
    }
}

// Called Lapis Philosophorum in the paper
void CandidatePrefixTree::Propagate(NodeAdress node_addr) {
    // TODO: implement
    std::cout << "\n\"Propagated\"" << node_addr.ToString() << "!\n";
}

void CandidatePrefixTree::PerformBFS() {
    for (auto& [depth1_node_feat, depth1_node_ptr] : root_.children) {
        NodeAdress depth1_node_addr{depth1_node_feat};
        AddChildrenToQueue(std::move(depth1_node_addr));  // Initialize queue with depth 2 nodes
    }
    while (bfs_queue_.size() != 0) {
        if (CheckNode(bfs_queue_.front()) == false) {
            Propagate(bfs_queue_.front());
        }
        AddChildrenToQueue(bfs_queue_.front());
        std::cout << "\n_____checked " + bfs_queue_.front().ToString() + "________\n";
        bfs_queue_.pop();
        PrintAsciiTree(root_, feat_count_);
    }
}

void CandidatePrefixTree::FinalizeTopK() {
    k_best_.clear();
    k_best_.reserve(topk_queue_.size());
    while (!topk_queue_.empty()) {
        k_best_.push_back(std::move(topk_queue_.top()));
        topk_queue_.pop();
    }
    std::sort(k_best_.begin(), k_best_.end(),
              [](auto const& A, auto const& B) { return A.p_value < B.p_value; });
}

CandidatePrefixTree::CandidatePrefixTree(size_t feat_count_, GetLowerBound1 lower_bound1,
                                         GetLowerBound2or3 lower_bound2,
                                         GetLowerBound2or3 lower_bound3, GetFishersP get_p,
                                         GetItemsetFrequency get_frequency, double max_p,
                                         double min_frequency, unsigned max_rules)
    : feat_count_(feat_count_),
      lower_bound1_(lower_bound1),
      lower_bound2_(lower_bound2),
      lower_bound3_(lower_bound3),
      get_p_(get_p),
      get_frequency_(get_frequency),
      max_p_(max_p),
      min_frequency_(min_frequency),
      max_rules_(max_rules) {
    for (size_t feat = 0; feat < feat_count_; ++feat) {
        auto node = BranchableNode(feat_count_, OFeatureIndex(feat));
        root_.AddChild(OFeatureIndex(feat), std::make_shared<BranchableNode>(std::move(node)));
    }
    CheckDepth1();
    PerformBFS();
    FinalizeTopK();
}

}  // namespace kingfisher
