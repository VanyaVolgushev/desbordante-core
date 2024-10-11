#pragma once

#include <vector>
#include "feature_bounds.h"

namespace model {

class NAR {
public:

    double fitness_;
    double support_;
    double confidence_;

    std::map<size_t, std::unique_ptr<FeatureBounds>> ante_;
    std::map<size_t, std::unique_ptr<FeatureBounds>> cons_;
};

} // namespace algos
