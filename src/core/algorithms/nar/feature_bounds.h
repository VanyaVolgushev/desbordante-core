#pragma once

#include "model/types/type.h"
#include "model/table/column_layout_typed_relation_data.h"
#include <enum.h>

namespace model {

BETTER_ENUM(FeatureTypeId, char, kCategorical = 0, kReal, kInteger);
    
class FeatureBounds {
public:
    virtual FeatureTypeId GetTypeId() const = 0;
protected:
    FeatureBounds(){}; 
};

class CategoricalFeatureBounds : public FeatureBounds {
public:
    explicit CategoricalFeatureBounds(model::TypedColumnData const& column);
    FeatureTypeId GetTypeId() const override { return FeatureTypeId::kCategorical; }
private:
    std::vector<std::string> domain_;
};

class RealFeatureBounds : public FeatureBounds {
public:
    explicit RealFeatureBounds(model::TypedColumnData const& column);
    FeatureTypeId GetTypeId() const override { return FeatureTypeId::kReal; }
private:
    double lower_bound_;
    double upper_bound_;
};

class IntegerFeatureBounds : public FeatureBounds {
public:
    explicit IntegerFeatureBounds(model::TypedColumnData const& column);
    FeatureTypeId GetTypeId() const override { return FeatureTypeId::kInteger; }
private:
    long int lower_bound_;
    long int upper_bound_;
};

std::shared_ptr<FeatureBounds> CreateFeatureBounds(model::TypedColumnData const& column);

} // namespace model
