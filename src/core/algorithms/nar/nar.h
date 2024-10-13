#pragma once

#include <vector>
#include "feature_bounds.h"

namespace model {

class NAR {
public:

    double fitness_ = -1;
    double support_ = -1;
    double confidence_ = -1;

    std::map<size_t, std::unique_ptr<FeatureBounds>> ante_= std::map<size_t, std::unique_ptr<FeatureBounds>>();
    std::map<size_t, std::unique_ptr<FeatureBounds>> cons_= std::map<size_t, std::unique_ptr<FeatureBounds>>();

    bool Includes()
};

} // namespace algos
