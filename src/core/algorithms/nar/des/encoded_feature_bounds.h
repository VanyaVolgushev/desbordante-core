#pragma once

#include "algorithms/nar/feature_bounds.h"
#include "model/table/column_layout_typed_relation_data.h"
#include "rng.h"

namespace algos::des {

class EncodedFeatureBounds {

public:
    double permutation_;
    double threshold_;
    double lower_bound_;
    double upper_bound_;
    size_t column_index_;
    EncodedFeatureBounds(size_t column_index) : column_index_(column_index) {
        permutation_ = RNG().Next();
        threshold_ = RNG().Next();
        lower_bound_ = RNG().Next();
        upper_bound_ = RNG().Next();
    }
};

} // namespace algos::des
