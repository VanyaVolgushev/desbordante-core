#pragma once

#include <cstddef>
#include <vector>
#include <string>
#include <sstream>

#include "types.h"

namespace model {

struct NeARIDs {
    std::vector<FeatureIndex> ante;
    FeatureIndex cons;
    bool cons_positive = false;

    NeARIDs() = default;

    NeARIDs(std::vector<FeatureIndex> ante, FeatureIndex cons, bool cons_positive)
        : ante(std::move(ante)),
          cons(std::move(cons)),
          cons_positive(cons_positive) {}

    std::string ToString() const {
        return "<A string representation of NeAR>"; // TODO: implement this
    }

    // TODO: this might be wrong
    NeARIDs UndoOrder(std::vector<FeatureIndex> const& order) const {
        NeARIDs reordered_near{};
        reordered_near.ante.reserve(ante.size());
        for (FeatureIndex ante_i: ante) {
            reordered_near.ante.emplace_back(order[ante_i]);
        }
        reordered_near.cons = order[cons];
        reordered_near.cons_positive = cons_positive;
        return reordered_near;
    }
};

struct NeARStrings {
    std::vector<std::string> ante;
    std::string cons;

    std::string ToString() const {
        std::ostringstream oss;

        for (size_t i = 0; i < ante.size(); ++i) {
            if (i != 0) 
                oss << ", ";
            oss << ante[i];
        }
        oss << " -> " << cons;

        return oss.str();
    }
};

} // namespace model