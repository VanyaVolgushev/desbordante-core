#pragma once

#include "algorithms/association_rules/ar.h"

struct RuleNode {
    model::ArIDs rule;
    std::list<RuleNode> children;
    RuleNode() = default;

    /* Temporary fix. Now we allocate generated AR twice -- in ar_collection_
     * and also in a rule node by copying it.
     * */
    explicit RuleNode(model::ArIDs const& rule) : rule(rule) {}
};
