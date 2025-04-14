#pragma once

#include <cstddef>
#include <vector>
#include <string>

#include "near_types.h"

namespace model {

struct NeARIDs {
    std::vector<FeatureIndex> ante_;
    FeatureIndex cons_;
    bool cons_positive_ = false;

    NeARIDs() = default;

    NeARIDs(std::vector<FeatureIndex> ante, FeatureIndex cons, bool cons_positive)
        : ante_(std::move(ante)),
          cons_(std::move(cons)),
          cons_positive_(cons_positive) {}

    std::string ToString() const {
        return "<A string representation of NeAR>"; // TODO: implement this
    }
};

struct NeARStrings {
    // . . .
};

} // namespace model