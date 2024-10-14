#pragma once

#include "algorithms/nar/value_range.h"
#include "model/table/column_layout_typed_relation_data.h"
#include "rng.h"

namespace algos::des {

class EncodedFeatureRange {

public:
    double permutation_;
    double threshold_;
    double lower_bound_;
    double upper_bound_;
    size_t feature_index_;
    EncodedFeatureRange(size_t feature_index) : feature_index_(feature_index) {
        permutation_ = RNG().Next();
        threshold_ = RNG().Next();
        lower_bound_ = RNG().Next();
        upper_bound_ = RNG().Next();
    }
};

} // namespace algos::des
