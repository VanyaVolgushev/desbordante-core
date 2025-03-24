#pragma once

#include <vector>

#include "algorithms/near/near_types.h"
#include "node.h"

namespace algos {

// Can be any increasing sequence of OrderedFeatureIndex-es
class NodeAdress {
private:
    std::vector<OrderedFeatureIndex> vec_;

public:
    std::vector<OrderedFeatureIndex> Get() const {
        return vec_;
    }

    // Returns copy of adress without one feature
    std::vector<OrderedFeatureIndex> GetExcept(size_t at) const {
        auto copy = vec_;
        copy.erase(copy.begin() + at);
        return copy;
    }

    // Returns smallest element and removes it
    OrderedFeatureIndex PopBack() {
        auto out = vec_[0];
        vec_.erase(vec_.begin());
        return out;
    }

    OrderedFeatureIndex Front() {
        return vec_.front();
    }

    size_t Size() const {
        return vec_.size();
    }

    bool Empty() const {
        return vec_.empty();
    }

    NodeAdress(std::vector<OrderedFeatureIndex> vec) : vec_(vec) {
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
};

// Responsible for storing the search tree, maintaining its consistency and propagating information
// about pruned paths between nodes.
class CandidatePrefixTree {
private:
    RoutingNode root_;
    size_t feat_count_;
    const std::vector<FeatureIndex> feat_frequency_order_;
    size_t depth_;

public:
    Node& GetNode(NodeAdress adress) {
        Node& current = root_;
        while (!adress.Empty()) {
            auto next_ordered_index = adress.PopBack();
            current = current.GetChild(next_ordered_index);
        }
        return current;
    }

    BranchableNode& CreateNode(NodeAdress adress) {
        BranchableNode new_node{feat_count_};

        // New nodes table is an intersection of all its subsets tables
        for (size_t i = 0; i < adress.Size(); ++i) {
            NodeAdress subsetAdress = adress.GetExcept(i);
            BranchableNode& subset = (BranchableNode&)(GetNode(subsetAdress)); //TODO: if subset not branchable
            new_node.Intersect(subset);
        }
        // TODO: if result is empty
        NodeAdress parentAdress = adress.GetExcept(adress.Size() - 1);
        BranchableNode& parent = (BranchableNode&)GetNode(parentAdress); // TODO: throw exception if triyng to create node that is child of RoutingNode
        parent.AddChild(adress.Front(), std::move(new_node));
        return (BranchableNode&)parent.GetChild(adress.Front());
    }

    void IncreaseDepth() {
        ++depth_;
    }
    // feat_frequency_order should be ascending
    CandidatePrefixTree(std::vector<FeatureIndex> feat_frequency_order)
        : root_(true),
          feat_count_(feat_frequency_order.size()),
          feat_frequency_order_(std::move(feat_frequency_order)) {
        // Create all possible nodes at depth 1
        for (size_t feat = 0; feat < feat_count_; ++feat) {
            root_.AddChild(OrderedFeatureIndex(feat), BranchableNode(feat_count_));
        }
    }
};

}  // namespace algos
