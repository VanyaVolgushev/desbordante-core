#pragma once

#include "model/types/type.h"
#include "model/table/column_layout_typed_relation_data.h"
#include "algorithms/nar/des/enums.h"

namespace algos::des {

class Feature {
public:
    virtual FeatureTypeId GetTypeId() = 0;
    virtual ~Feature() = 0;
};

class CategoricalFeature : public Feature {
public:
    explicit CategoricalFeature(model::TypedColumnData const& column);
    FeatureTypeId GetTypeId() override {
        return FeatureTypeId::kCategorical;
    }
private:
    std::vector<std::string> domain_;
};

class RealFeature : public Feature {
public:
    explicit RealFeature(model::TypedColumnData const& column);
    FeatureTypeId GetTypeId() override {
        return FeatureTypeId::kReal;
    }
private:
    double lower_bound_;
    double upper_bound_;
};

class IntegerFeature : public Feature {
public:
    explicit IntegerFeature(model::TypedColumnData const& column);
    FeatureTypeId GetTypeId() override {
        return FeatureTypeId::kInteger;
    }
private:
    long int lower_bound_;
    long int upper_bound_;
};

} // namespace algos::cfd
