#pragma once

#include <cstddef>
#include <vector>

// Wrappers to prevent accidental implicit conversions of ordered/unordered indices
struct FeatureIndex {
    size_t val;
};

struct OrderedFeatureIndex {
    size_t val;

    FeatureIndex ToFeatureIndex(std::vector<FeatureIndex> order) {
        return order[val];
    }

    // OrderedFeatureIndex(std::vector<OrderedFeatureIndex> order, FeatureIndex index)
    //     : val(0) {}  // TODO

    bool operator==(OrderedFeatureIndex const& other) const {
        return val == other.val;
    }

    explicit OrderedFeatureIndex(size_t index) : val(index) {}

    OrderedFeatureIndex() = default;
};

struct OrderedFeatureIndexHash {
    size_t operator()(OrderedFeatureIndex const& key) const {
        return std::hash<size_t>()(key.val);
    }
};
