#pragma once

#include <vector>
#include "feature_bounds.h"


namespace algos {

class NAR {
public:

    double fitness;
    double support;
    double confidence;

    std::vector<std::unique_ptr<FeatureBounds>> ante;
    std::vector<std::unique_ptr<FeatureBounds>> cons;
};

} // namespace algos
