#pragma once

#include <cstddef>
#include <vector>
#include <string>

#include "near_types.h"

namespace model {

struct NeARIDs {
    std::vector<FeatureIndex> ante_;
    FeatureIndex cons_;
    bool cons_negated_ = false;

    NeARIDs() = default;

    NeARIDs(std::vector<FeatureIndex> ante, FeatureIndex cons, bool cons_negated)
        : ante_(std::move(ante)),
          cons_(std::move(cons)),
          cons_negated_(cons_negated) {}

    std::string ToString() const {
        return "<A string representation of NeAR>"; // TODO: implement this
    }
};

struct NeARStrings {
    // . . .
};

} // namespace model