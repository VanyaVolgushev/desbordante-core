#pragma once

#include <cstddef>
#include <sstream>
#include <string>
#include <vector>

#include "types.h"
#include <iomanip>

namespace model {

struct NeARIDs {
    std::vector<FeatureIndex> ante;
    FeatureIndex cons;
    bool cons_positive = true;
    double p_value = -1.0;

    NeARIDs() = default;

    NeARIDs(std::vector<FeatureIndex> ante, FeatureIndex cons, bool cons_positive)
        : ante(std::move(ante)), cons(std::move(cons)), cons_positive(cons_positive) {}

    std::string ToString() const {
        std::ostringstream oss;
        oss << std::scientific << std::setprecision(3) << p_value << "  ";
        oss << std::defaultfloat;
        oss << "{";
        for (size_t i = 0; i < ante.size(); ++i) {
            oss << ante[i];
            if (i + 1 < ante.size()) oss << ", ";
        }
        oss << "} -> ";
        if (!cons_positive) oss << "not ";
        oss << cons;
        return oss.str();
    }

    NeARIDs UndoOrder(std::vector<FeatureIndex> const& order) const {
        NeARIDs reordered_near = *this;
        reordered_near.ante.clear();
        reordered_near.ante.reserve(ante.size());
        for (FeatureIndex ante_i : ante) {
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
            if (i != 0) oss << ", ";
            oss << ante[i];
        }
        oss << " -> " << cons;

        return oss.str();
    }
};

}  // namespace model