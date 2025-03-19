#pragma once

#include <vector>

#include <boost/dynamic_bitset.hpp>

#include "algorithms/near/near_types.h"

namespace algos {

// Represents a node of the branch-and-bound search tree.
class Node {
private:
    OrderedFeatureIndex adds_feat_;
    std::unordered_map<OrderedFeatureIndex, Node, OrderedFeatureIndexHash> children_;
    // A 1 in a position means that the feature of that ordered index (or its negation
    // in case of n_possible) can be a consequence in this node or its children and will be
    // evaluated.
    boost::dynamic_bitset<> p_possible;
    boost::dynamic_bitset<> n_possible;
    // Best possible target function values for each feature as the consequence
    std::vector<double> p_best;
    std::vector<double> n_best;

public:
    Node& GetChild(OrderedFeatureIndex feat_index) {
        return children_.at(feat_index);
    }

    Node& AddChild(OrderedFeatureIndex feat_index, size_t feat_count) {
        children_.insert({feat_index, Node(feat_index, feat_count)});
        return children_.at(feat_index);
    }

    void Intersect(Node other) {
        other.p_best.at(0);
        // TODO
    }

    Node(OrderedFeatureIndex adds_feat, size_t feat_count)
        : adds_feat_(adds_feat),
          p_possible(feat_count),
          n_possible(feat_count),
          p_best(feat_count, 0.0),
          n_best(feat_count, 0.0) {
        p_possible.flip();
        n_possible.flip();
    }

    Node() = default;
};

}  // namespace algos
