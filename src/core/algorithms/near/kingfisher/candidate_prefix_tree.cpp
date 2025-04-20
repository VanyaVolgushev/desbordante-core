#include "candidate_prefix_tree.h"

#include <algorithm>
#include <iostream>
#include <stdexcept>

namespace kingfisher {
using Entry = std::pair<double, model::NeARIDs>;

// Debug
// Recursively visualize the tree, using an indentation proportional to the current depth.
// Helper function: Recursively prints the children of a node using ASCII tree branch symbols.
void PrintAsciiTreeChildren(Node const& node, size_t feat_count, std::string const& prefix) {
    // Collect children in order of feature indices 0..feat_count_-1.
    std::vector<std::pair<OFeatureIndex, std::shared_ptr<Node>>> childNodes;
    for (OFeatureIndex i = 0; i < feat_count; ++i) {
        if (node.HasChild(i)) {
            childNodes.push_back({i, node.children.at(i)});
        }
    }

    // Iterate over collected children.
    for (size_t i = 0; i < childNodes.size(); i++) {
        bool isLast = (i == childNodes.size() - 1);
        // Choose the branch connector.
        std::cout << prefix << (isLast ? "└── " : "├── ");
        // Print the feature index and node label.
        std::cout << "[" << childNodes[i].first << "] Node" << std::endl;
        // Adjust the prefix for the child recursion.
        std::string childPrefix = prefix + (isLast ? "    " : "│   ");
        PrintAsciiTreeChildren(*childNodes[i].second, feat_count, childPrefix);
    }
}

void PrintAsciiTree(Node const& root, size_t feat_count) {
    std::cout << "Node" << std::endl;
    PrintAsciiTreeChildren(root, feat_count, "");
}

std::vector<model::NeARIDs>& CandidatePrefixTree::GetNeARIDs() {
    return k_best_;  // TODO: strip p-values
}

std::optional<BranchableNode> CandidatePrefixTree::MakeBranchableFromParents(
        NodeAdress adress_of_node_to_make) const {
    BranchableNode new_node{feat_count_, adress_of_node_to_make.Back()};
    auto const parent_adresses = adress_of_node_to_make.GetParents();

    for (auto const& parent_adress : parent_adresses) {
        std::optional<Node const* const> parent = GetNode(parent_adress);
        if (!parent.has_value()) {
            // TODO: Use Lapis
            return {};
        }
        // Parent can never be a RoutingNode
        new_node.Intersect(dynamic_cast<BranchableNode const&>(*parent.value()));
        if (new_node.Pruned()) {
            // TODO: Use Lapis
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

void CandidatePrefixTree::IncreaseDepth() {
    ++depth_;
}

void CandidatePrefixTree::AddChildrenToQueue(NodeAdress adress) {
    for (NodeAdress child_adress : adress.GetChildren(feat_count_)) {
        bfs_queue_.emplace(child_adress);
    }
}

inline void CandidatePrefixTree::TrySaveRule(double fishers_p, model::NeARIDs near) {
    Entry e{fishers_p, std::move(near)};
    if (topk_queue_.size() < max_rules_) {
        topk_queue_.push(std::move(e));
    } else if (fishers_p > topk_queue_.top().first) {
        topk_queue_.pop();
        topk_queue_.push(std::move(e));
    }
}

void CandidatePrefixTree::EvaluatePossibleRules(NodeAdress node, boost::dynamic_bitset<> p_possible,
                                                boost::dynamic_bitset<> n_possible) {
    auto processRules = [&](boost::dynamic_bitset<> const& possible, bool cons_positive) {
        for (size_t feat = 0; feat < feat_count_; ++feat) {
            if (possible[feat]) {
                model::NeARIDs near{node.GetExceptFeat(feat), feat, cons_positive};
                double fishers_p = get_p_(near);
                TrySaveRule(fishers_p, near);
                // TODO: Update p_best?
            }
        }
    };

    processRules(p_possible, true);
    processRules(n_possible, false);
}

// returns: whether the node exists after check
bool CandidatePrefixTree::CheckNode(NodeAdress node_addr) {
    // TODO: USE BOUNDARIES, CHECK ACTUAL RULES. . .
    OFeatureIndex adds_feature = node_addr.Back();
    auto parent_adress = node_addr.GetParent();
    auto parent = GetNode(parent_adress);
    if (!parent.has_value()) {
        // TODO: use lapis???
        // std::cout << "\nNode: " << node_addr.ToString() << " had no parent";
        return false;
    }

    auto node = MakeBranchableFromParents(node_addr);
    // TODO: what if node doesnt exist?
    EvaluatePossibleRules(node_addr, node.value().p_possible_, node.value().n_possible_);
    auto node_ptr = std::make_shared<BranchableNode>(std::move(node.value()));
    auto node_ptr_upcasted = static_cast<std::shared_ptr<Node>>(node_ptr);
    parent.value()->AddChild(adds_feature, node_ptr_upcasted);

    // std::cout << "\nNode: " << node_addr.ToString() << " returns true";
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
            branchable_node.p_possible_[child_feat] = lb1_ok && lb2_ok_pos;
            branchable_node.n_possible_[child_feat] = lb1_ok && lb2_ok_neg;
        }
        // TODO: what if the tables are all zeroes?
    }
}

void CandidatePrefixTree::PerformBFS() {
    for (auto& [depth1_node_feat, depth1_node_ptr] : root_.children) {
        NodeAdress depth1_node_addr{depth1_node_feat};
        AddChildrenToQueue(std::move(depth1_node_addr));  // Initialize queue with depth 2 nodes
    }
    while (bfs_queue_.size() != 0) {
        CheckNode(bfs_queue_.front());
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
              [](auto const& A, auto const& B) { return A.first > B.first; });
}

CandidatePrefixTree::CandidatePrefixTree(size_t feat_count_, GetLowerBound1 lower_bound1,
                                         GetLowerBound2or3 lower_bound2,
                                         GetLowerBound2or3 lower_bound3, GetFishersP get_p,
                                         double max_p, unsigned max_rules)
    : feat_count_(feat_count_),
      lower_bound1_(lower_bound1),
      lower_bound2_(lower_bound2),
      lower_bound3_(lower_bound3),
      get_p_(get_p),
      max_p_(max_p),
      max_rules_(max_rules) {
    for (size_t feat = 0; feat < feat_count_; ++feat) {
        auto node = BranchableNode(feat_count_, OFeatureIndex(feat));
        root_.AddChild(OFeatureIndex(feat), std::make_shared<BranchableNode>(std::move(node)));
    }
    CheckDepth1();
    PerformBFS();
}

}  // namespace kingfisher
