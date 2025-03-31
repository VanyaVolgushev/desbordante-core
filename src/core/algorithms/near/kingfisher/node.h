#pragma once

#include <algorithm>
#include <cstddef>
#include <memory>
#include <ranges>
#include <vector>
#include <map>

#include <boost/dynamic_bitset.hpp>

#include "algorithms/near/near_types.h"

namespace algos {

class BranchableNode;
// TODO: remove underscores from names
// Represents a node of the branch-and-bound search tree.
struct Node {
    std::map<OrderedFeatureIndex, std::shared_ptr<Node>>
            children;

    Node& GetChild(OrderedFeatureIndex feat_index) {
        return *children.at(feat_index);
    }

    virtual void AddChild(OrderedFeatureIndex feat_index, Node&& child) {
        children.emplace(feat_index, std::make_shared<Node>(std::move(child)));
    }

    Node() = default;
};

struct BranchableNode : public Node {
    // A 1 in a position means that the feature of that ordered index (or its negation
    // in case of n_possible) can be a consequence in this node or its children and will be
    // evaluated.
    boost::dynamic_bitset<> p_possible_;
    boost::dynamic_bitset<> n_possible_;
    // A 1 in a position means that a NodeAdress with that ordered index added is valid and a child
    // with that adress wasn't previously deleted
    boost::dynamic_bitset<> b_possible_;
    // Best possible target function values for each feature as the consequence
    std::vector<double> p_best_;
    std::vector<double> n_best_;

    void Intersect(BranchableNode other) {
        p_possible_ &= other.p_possible_;
        n_possible_ &= other.n_possible_;
        std::ranges::transform(p_best_, other.p_best_, p_best_.begin(),
                               [](double a, double b) { return std::min(a, b); });
        std::ranges::transform(n_best_, other.n_best_, n_best_.begin(),
                               [](double a, double b) { return std::min(a, b); });
    }

    BranchableNode(size_t feat_count, OrderedFeatureIndex adds_feat)
        : Node(),
          p_possible_(feat_count),
          n_possible_(feat_count),
          b_possible_(feat_count),
          p_best_(feat_count, 0.0),
          n_best_(feat_count, 0.0) {
        if (adds_feat.val + 1 < b_possible_.size())
            // sets b_possible_ to 00...011...1 where the rightmost 0 has index adds_feat
            b_possible_.set(adds_feat.val + 1, b_possible_.size() - adds_feat.val - 1, true);
        p_possible_.flip();
        n_possible_.flip();
    }
};

struct RoutingNode : public Node {
    bool isRoot_;

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
