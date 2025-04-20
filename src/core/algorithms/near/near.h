#pragma once

#include <cstddef>
#include <vector>
#include <string>
#include <sstream>

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
    std::vector<std::string> ante_;
    std::string cons_;

    std::string ToString() const {
        std::ostringstream oss;

        for (size_t i = 0; i < ante_.size(); ++i) {
            if (i != 0) 
                oss << ", ";
            oss << ante_[i];
        }
        oss << " -> " << cons_;

        return oss.str();
    }
};

} // namespace model