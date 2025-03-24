#pragma once

#include <algorithm>
#include <cstddef>
#include <memory>
#include <ranges>
#include <vector>

#include <boost/dynamic_bitset.hpp>

#include "algorithms/near/near_types.h"

namespace algos {

// Represents a node of the branch-and-bound search tree.
class Node {
protected:
    std::unordered_map<OrderedFeatureIndex, Node, OrderedFeatureIndexHash> children_;

public:
    Node& GetChild(OrderedFeatureIndex feat_index) {
        return children_.at(feat_index);
    }

    virtual void AddChild(OrderedFeatureIndex feat_index, Node&& child) {
        children_.emplace(feat_index, std::move(child));
    }

    Node() = default;
};

class BranchableNode : public Node {
private:
    // A 1 in a position means that the feature of that ordered index (or its negation
    // in case of n_possible) can be a consequence in this node or its children and will be
    // evaluated.
    boost::dynamic_bitset<> p_possible;
    boost::dynamic_bitset<> n_possible;
    // Best possible target function values for each feature as the consequence
    std::vector<double> p_best;
    std::vector<double> n_best;

public:
    void Intersect(BranchableNode other) {
        p_possible &= other.p_possible;
        n_possible &= other.n_possible;
        std::ranges::transform(p_best, other.p_best, p_best.begin(), [](double a, double b) { return std::min(a, b); });
        std::ranges::transform(n_best, other.n_best, n_best.begin(), [](double a, double b) { return std::min(a, b); });

    }

    BranchableNode(size_t feat_count)
        : Node(),
          p_possible(feat_count),
          n_possible(feat_count),
          p_best(feat_count, 0.0),
          n_best(feat_count, 0.0) {
        p_possible.flip();
        n_possible.flip();
    }
};

class RoutingNode : public Node {
private:
    bool isRoot_;

public:
    RoutingNode(bool isRoot = false) : isRoot_(isRoot) {}

    void AddChild(OrderedFeatureIndex feat_index, Node&& child) override {
        if (isRoot_) {
            Node::AddChild(feat_index, std::move(child));
        } else {
            throw std::logic_error("Trying to add child to non-root RoutingNode.");
        }
    }
};

}  // namespace algos
