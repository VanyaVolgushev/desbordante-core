#pragma once

#include <list>
#include <vector>

namespace algos {

struct ItemsetNode {
    std::vector<unsigned> items;
    double support = 0;
    std::vector<ItemsetNode> children;

    ItemsetNode() = default;
    ItemsetNode(ItemsetNode&& other) = default;
    ItemsetNode& operator=(ItemsetNode&& other) = default;

    ItemsetNode(ItemsetNode const& node) = delete;
    ItemsetNode& operator=(ItemsetNode const&) = delete;

    explicit ItemsetNode(unsigned item_id) : items({item_id}), support(0) {}

    explicit ItemsetNode(std::vector<unsigned>&& items_to_add) : ItemsetNode() {
        std::swap(items_to_add, items);
    }
};

}  // namespace algos
