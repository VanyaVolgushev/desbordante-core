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

class CandidatePrefixTree {
private:
    Node root_;
    size_t feat_count;
    std::vector<FeatureIndex> feat_frequency_order_;
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

    Node& CreateNode(NodeAdress adress) {
        NodeAdress parentAdress = adress.GetExcept(adress.Size() - 1);
        Node& parent = GetNode(parentAdress);

        Node& new_node = parent.AddChild(adress.Front(), feat_count);

        // New nodes table is an intersection of all its subsets tables
        for (size_t i = 0; i < adress.Size(); ++i) {
            NodeAdress subsetAdress = adress.GetExcept(i);
            Node subset = GetNode(subsetAdress);
            new_node.Intersect(subset);
        }
        // TODO: what to do if result is empty
        return new_node;
    }

    void IncreaseDepth() {
        ++depth_;
    }

    CandidatePrefixTree(Node root, size_t feat_count, std::vector<FeatureIndex> feat_frequency_order, size_t depth)
    : root_(std::move(root)), feat_count(feat_count), feat_frequency_order_(std::move(feat_frequency_order)), depth_(depth) {}

};

}  // namespace algos
