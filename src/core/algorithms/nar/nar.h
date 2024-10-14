#pragma once

#include <vector>
#include "feature_bounds.h"
#include "model/types/types.h"

namespace model {

class NAR {
public:

    double fitness_ = -1;
    double support_ = -1;
    double confidence_ = -1;

    std::map<size_t, std::unique_ptr<FeatureBounds>> ante_= std::map<size_t, std::unique_ptr<FeatureBounds>>();
    std::map<size_t, std::unique_ptr<FeatureBounds>> cons_= std::map<size_t, std::unique_ptr<FeatureBounds>>();

    bool IncludesInAnte(size_t feature_index, const std::byte* value) const {
        return MapIncludes(ante_, feature_index, value);
    }

    bool IncludesInCons(size_t feature_index, const std::byte* value) const {
        return MapIncludes(cons_, feature_index, value);
    }

private:
    static bool MapIncludes(std::map<size_t, std::unique_ptr<FeatureBounds>> map, size_t feature_index, const std::byte* value) {
        for(auto const& iterator: map) {
            if (iterator.first != feature_index) {
                continue;
            }
            if (iterator.second->Includes(value)) {
                return true;
            }
        }
        return false;
    }
};

} // namespace algos
