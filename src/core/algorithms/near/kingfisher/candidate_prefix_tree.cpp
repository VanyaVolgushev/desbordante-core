#include "candidate_prefix_tree.h"

#include <stdexcept>

namespace algos {

NodeAdress::NodeAdress(std::vector<OrderedFeatureIndex> vec) : vec_(vec) {
    if (vec.empty()) {
        return;
    }
    OrderedFeatureIndex prev{vec[0].val};
    for (size_t i = 1; i < vec.size(); ++i) {
        if (vec[i].val <= prev.val) {
            throw std::logic_error("NodeAdress must be an increasing sequence.");
        }
        prev = vec[i];
    }
    vec_ = std::move(vec);
}

std::vector<OrderedFeatureIndex> NodeAdress::Get() const {
    return vec_;
}

std::vector<OrderedFeatureIndex> NodeAdress::GetExcept(size_t at) const {
    auto copy = vec_;
    copy.erase(copy.begin() + at);
    return copy;
}

OrderedFeatureIndex NodeAdress::PopBack() {
    auto out = vec_[0];
    vec_.erase(vec_.begin());
    return out;
}

OrderedFeatureIndex NodeAdress::Front() {
    return vec_.front();
}

size_t NodeAdress::Size() const {
    return vec_.size();
}

bool NodeAdress::Empty() const {
    return vec_.empty();
}

std::vector<FeatureIndex> NodeAdress::ToFeatures(std::vector<FeatureIndex> const& order) const {
    std::vector<FeatureIndex> result;
    result.reserve(vec_.size());
    for (auto i : vec_) {
        result.emplace(result.begin(), order[i.val]);  // TODO: ensure this is correct
    }
    return result;
}

std::string NodeAdress::ToString() const {
    std::ostringstream oss;
    oss << "[";
    for (size_t i = 0; i < vec_.size(); ++i) {
        oss << vec_[i].val;
        if (i < vec_.size() - 1) oss << ", ";
    }
    oss << "]";
    return oss.str();
}
 
std::vector<model::NeARIDs>& CandidatePrefixTree::GetNeARIDs() {
    return k_best_;
}

Node& CandidatePrefixTree::GetNode(NodeAdress adress) {
    Node& current = root_;
    while (!adress.Empty()) {
        auto next_ordered_index = adress.PopBack();
        current = current.GetChild(next_ordered_index);
    }
    return current;
}

void CandidatePrefixTree::IncreaseDepth() {
    ++depth_;
}

void CandidatePrefixTree::CheckDepth1() {
    //for (auto& [node_feat, node_ptr] : root_.children) {
    //    auto& branchable_node = *std::static_pointer_cast<BranchableNode>(node_ptr);
    //    auto nodeAdress = NodeAdress{node_feat};
    //    // Check all possible branches' best-case p values
    //    for (auto child_feat = OrderedFeatureIndex{0}; child_feat < feat_count_; child_feat++) {
    //        bool lb1_ok = lower_bound1_(child_feat) < max_p_;
    //        bool lb2_ok_pos = lower_bound2_(nodeAdress, child_feat, true) < max_p_;
    //        bool lb2_ok_neg = lower_bound2_(nodeAdress, child_feat, false) < max_p_;
    //        branchable_node.p_possible_[child_feat] = lb1_ok && lb2_ok_pos;
    //        branchable_node.n_possible_[child_feat] = lb1_ok && lb2_ok_neg;
    //    }
    //}
}

void CandidatePrefixTree::PerformBFS() {}

CandidatePrefixTree::CandidatePrefixTree(size_t feat_count_, GetLowerBound1 lower_bound1,
                                         GetLowerBound2or3 lower_bound2,
                                         GetLowerBound2or3 lower_bound3, GetFishersP get_p,
                                         double max_p)
    : feat_count_(feat_count_),
      lower_bound1_(lower_bound1),
      lower_bound2_(lower_bound2),
      lower_bound3_(lower_bound3),
      get_p_(get_p),
      max_p_(max_p) {
    for (size_t feat = 0; feat < feat_count_; ++feat) {
        auto node = BranchableNode(feat_count_, OrderedFeatureIndex(feat));
        root_.AddChild(OrderedFeatureIndex(feat), std::move(node));
    }
    CheckDepth1();
    PerformBFS();
}

}  // namespace algos
