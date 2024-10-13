#pragma once

#include "model/types/type.h"
#include "model/table/column_layout_typed_relation_data.h"
#include <enum.h>

namespace model {

BETTER_ENUM(FeatureTypeId, char, kCategorical = 0, kReal, kInteger);
    
class FeatureBounds {
public:
    size_t column_index_;
    virtual model::TypeId GetTypeId() const = 0;
    //virtual model:Double GetLowerBound 
protected:
    FeatureBounds() {}; 
};

class CategoricalFeatureBounds : public FeatureBounds {
public:
    explicit CategoricalFeatureBounds(model::TypedColumnData const& column, size_t column_index);
    model::TypeId GetTypeId() const override { return model::TypeId::kString; }
private:
    std::vector<model::String> domain_;
};

class RealFeatureBounds : public FeatureBounds {
public:
    explicit RealFeatureBounds(model::TypedColumnData const& column, size_t column_index);
    model::TypeId GetTypeId() const override { return model::TypeId::kDouble; }
private:
    model::Double lower_bound_;
    model::Double upper_bound_;
};

class IntegerFeatureBounds : public FeatureBounds {
public:
    explicit IntegerFeatureBounds(model::TypedColumnData const& column, size_t column_index);
    model::TypeId GetTypeId() const override { return model::TypeId::kInt; }
private:
    model::Int lower_bound_;
    model::Int upper_bound_;
};

std::shared_ptr<FeatureBounds> CreateFeatureBounds(model::TypedColumnData const& column, size_t column_index);

} // namespace model
