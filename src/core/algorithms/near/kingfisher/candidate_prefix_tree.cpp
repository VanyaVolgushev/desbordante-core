#include "candidate_prefix_tree.h"

#include <algorithm>
#include <iostream>
#include <stdexcept>

#include <boost/math/distributions/hypergeometric.hpp>

#include "algorithms/near/kingfisher/util/print_ascii_tree.h"
#include "algorithms/near/kingfisher/util/vector_to_string.h"

namespace kingfisher {

// TODO: PLI
std::vector<FeatureIndex> CandidatePrefixTree::GetFeatureFrequency() const {
    std::vector<std::pair<size_t, FeatureIndex>> feature_frequency;
    feature_frequency.reserve(transactional_data_->GetUniverseSize());
    // iota
    for (size_t i = 0; i < transactional_data_->GetUniverseSize(); ++i) {
        feature_frequency.emplace_back(0, i);
    }
    for (auto const& [_, itemset] : transactional_data_->GetTransactions()) {
        for (auto const& item : itemset.GetItemsIDs()) {
            ++feature_frequency[item].first;
        }
    }

    std::sort(feature_frequency.begin(), feature_frequency.end(),
              [](auto const& a, auto const& b) { return a.first < b.first; });

    std::vector<FeatureIndex> feature_frequency_order;
    feature_frequency_order.reserve(feature_frequency.size());
    for (auto const& [_, index] : feature_frequency) {
        feature_frequency_order.push_back(index);
    }
    return feature_frequency_order;
}


double CandidatePrefixTree::GetLowerBound1(OFeatureIndex feature) const {
    std::cout << "Lower bound 1 for feat " + std::to_string(feature_frequency_order_[feature]) +
                         " is: ";
    // double bound;
    // std::cin >> bound;
    return 0;
}

double CandidatePrefixTree::GetLowerBound2(kingfisher::NodeAdress const& node_addr,
                                           OFeatureIndex cons_index, bool cons_positive) const {
    // TODO: проверка на то что выражения под факториалами неотрицательны
    std::cout << "Lower bound 2 for " +
                         VectorToString(node_addr.ToFeatures(feature_frequency_order_)) + " at " +
                         std::to_string(feature_frequency_order_[cons_index]) +
                         " with cons positive:" + std::to_string(cons_positive) + " is: ";
    //
    // double bound;
    // std::cin >> bound;
    return 0;
}

// NodeAdress is provided with indices in order of frequency
double CandidatePrefixTree::GetLowerBound3(kingfisher::NodeAdress const& node_addr,
                                           OFeatureIndex cons_index, bool cons_positive) const {
    std::cout << "Lower bound 3 for " +
                         VectorToString(node_addr.ToFeatures(feature_frequency_order_)) + " at " +
                         std::to_string(feature_frequency_order_[cons_index]) +
                         " with cons positive:" + std::to_string(cons_positive) + " is: ";
    //
    // double bound;
    // std::cin >> bound;
    return 0;
}

double CandidatePrefixTree::GetFishersP(model::NeARIDs const& rule) const {
    model::NeARIDs real_rule = rule.UndoOrder(feature_frequency_order_);

    auto hasAnte = [&](std::vector<unsigned> const& ids, model::NeARIDs const& r) {
        for (auto f : r.ante)
            if (std::find(ids.begin(), ids.end(), f) == ids.end()) return false;
        return true;
    };
    auto hasCons = [&](std::vector<unsigned> const& ids, model::NeARIDs const& r) {
        bool found = std::find(ids.begin(), ids.end(), r.cons) != ids.end();
        return r.cons_positive ? found : !found;
    };

    // Count a, b, c, d
    std::size_t a = 0, b = 0, c = 0, d = 0;
    for (auto const& [_, tx] : transactional_data_->GetTransactions()) {
        bool X = hasAnte(tx.GetItemsIDs(), real_rule);
        bool Y = hasCons(tx.GetItemsIDs(), real_rule);
        if (X && Y)
            ++a;
        else if (X && !Y)
            ++b;
        else if (!X && Y)
            ++c;
        else
            ++d;
    }

    unsigned const N = a + b + c + d;
    unsigned const r = a + c;
    unsigned const n = a + b;

    using boost::math::hypergeometric_distribution;
    hypergeometric_distribution<> dist(r, n, N);

    // Find valid lower bound for the quantile
    int const support_min = std::max(0, int(n + r - N));
    int const quantile = int(a) - 1;

    if (quantile < support_min) {
        // Whole right tail = 1
        return 1.0;
    }

    return boost::math::cdf(boost::math::complement(dist, static_cast<double>(quantile)));
}

double CandidatePrefixTree::GetItemsetFrequency(kingfisher::NodeAdress node_addr) const {
    std::cout << node_addr.ToString();
    return 0.0;
}

// Undoes internal index order on the discovered NeARs then returns them
std::vector<model::NeARIDs> CandidatePrefixTree::GetNeARIDs() const {
    std::vector<model::NeARIDs> real_nears;
    real_nears.reserve(k_best_.size());
    for (auto const& near : k_best_) {
        real_nears.push_back(near.UndoOrder(feature_frequency_order_));
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
    near.p_value = GetFishersP(near);
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

bool CandidatePrefixTree::ConsPossible(NodeAdress node_addr, OFeatureIndex cons_index,
                                       bool cons_positive, double best_measure) const {
    // Check if frequency requirements are satisifed
    if (!node_addr.Contains(cons_index) && GetItemsetFrequency(node_addr) < min_frequency_) {
        return false;
    }
    if (node_addr.Contains(cons_index) &&
        false /*GetItemsetFrequency(node_addr, cons_index, cons_positive) < min_frequency_*/) {
        return false;
    }
    // Check if lower bound requirements are satisifed
    double lower_bound;
    if (!node_addr.Contains(cons_index)) {
        if (GetItemsetFrequency(node_addr) <= 1.0 /*get_frequency_(cons_index, cons_positive)*/) {
            lower_bound = GetLowerBound2(node_addr, cons_index, cons_positive);
        } else {
            lower_bound = GetLowerBound1(cons_index);
        }
    } else {
        lower_bound = GetLowerBound3(node_addr, cons_index, cons_positive);
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
    double frequency = GetItemsetFrequency(node_addr);
    is_minimal = frequency == 1.0;
    if (!is_minimal) {
        // TODO: cache frequencies
        for (auto parent_addr : node_addr.GetParents()) {
            if (frequency == GetItemsetFrequency(parent_addr)) {
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
        // Prune out consequences in parents using Lapis Philosophorum principle from paper
    }

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
            bool lb1_ok = GetLowerBound1(child_feat) < max_p_;
            bool lb2_ok_pos = GetLowerBound2(nodeAdress, child_feat, true) < max_p_;
            bool lb2_ok_neg = GetLowerBound2(nodeAdress, child_feat, false) < max_p_;
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

CandidatePrefixTree::CandidatePrefixTree(size_t feat_count, double max_p, double min_frequency,
                                         unsigned max_rules,std::shared_ptr<model::TransactionalData> transactional_data)
    : feat_count_(feat_count), max_p_(max_p), max_rules_(max_rules), min_frequency_(min_frequency), transactional_data_(transactional_data) {
        feature_frequency_order_ = GetFeatureFrequency();
        for (size_t feat = 0; feat < feat_count_; ++feat) {
        auto node = BranchableNode(feat_count_, OFeatureIndex(feat));
        root_.AddChild(OFeatureIndex(feat), std::make_shared<BranchableNode>(std::move(node)));
    }
    CheckDepth1();
    PerformBFS();
    FinalizeTopK();
}

}  // namespace kingfisher
