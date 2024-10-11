#pragma once

#include "algorithms/nar/feature_bounds.h"
#include "model/table/column_layout_typed_relation_data.h"
#include "rng.h"

namespace algos::des {

class EncodedFeatureBounds {

public:
    virtual model::FeatureTypeId GetTypeId() const = 0;
    double permutation_;
    double threshold_;

protected:
    EncodedFeatureBounds(){}
};

// Doesn't actually encode a range of values of a categorical feature, just a single value. This is because the paper I 
// took the algorithm from had this limitation imposed. I need to keep my functionality identical to theirs, but I
// also want to keep class names consistent.
class EncodedCategoricalFeatureBounds : public EncodedFeatureBounds {
    public:
    explicit EncodedCategoricalFeatureBounds(double value) {
        (value >= 0 && value <= 1) ? value_ = value : throw  std::out_of_range("Encoded values must be between 0 and 1.");
    }
    EncodedCategoricalFeatureBounds() : EncodedCategoricalFeatureBounds(RNG().Next()) {};
    model::FeatureTypeId GetTypeId() const override { return model::FeatureTypeId::kCategorical; }
    double value_;
};

class EncodedNumericalFeatureBounds : public EncodedFeatureBounds {
    public:
    explicit EncodedNumericalFeatureBounds(double lower_bound, double upper_bound) {
        if(lower_bound >= 0 && lower_bound <= 1) {
            lower_bound_ = lower_bound;
        } else {
            throw std::out_of_range("Encoded values must be between 0 and 1.");
        }
        if(upper_bound >= 0 && upper_bound <= 1) {
            upper_bound_ = upper_bound;
        } else {
            throw std::out_of_range("Encoded values must be between 0 and 1.");
        }
    }
    EncodedNumericalFeatureBounds() : EncodedNumericalFeatureBounds(RNG().Next(), RNG().Next()) {};
    double lower_bound_;
    double upper_bound_;
};

class EncodedIntegerFeatureBounds : public EncodedNumericalFeatureBounds {
public:
    //EncodedIntegerFeatureBounds(double lower_bound, double upper_bound)
    //                            : EncodedNumericalFeatureBounds(lower_bound, upper_bound) {};
    //EncodedIntegerFeatureBounds() : EncodedNumericalFeatureBounds() {};
    model::FeatureTypeId GetTypeId() const override { return model::FeatureTypeId::kInteger; }
};

class EncodedRealFeatureBounds : public EncodedNumericalFeatureBounds {
public:
    //EncodedRealFeatureBounds(double lower_bound, double upper_bound)
    //                            : EncodedNumericalFeatureBounds(lower_bound, upper_bound) {};
    //EncodedRealFeatureBounds() : EncodedNumericalFeatureBounds() {};
    model::FeatureTypeId GetTypeId() const override { return model::FeatureTypeId::kReal; }
};

std::shared_ptr<EncodedFeatureBounds> CreateEncodedFeatureBounds(model::FeatureTypeId typeId);

} // namespace algos::des
