#pragma once

#include "model/types/type.h"
#include "model/table/column_layout_typed_relation_data.h"
#include <enum.h>

namespace model {

class ValueRange {
public:
    virtual TypeId GetTypeId() const = 0;
    virtual bool Includes(const std::byte* value) const = 0;
    //virtual model:Double GetLowerBound 
protected:
    ValueRange() {}; 
};

class CategoricalValueRange : public ValueRange {
public:
    explicit CategoricalValueRange(TypedColumnData const& column);
    TypeId GetTypeId() const override { return TypeId::kString; }
    bool Includes(const std::byte* value) const override {
        String svalue = Type::GetValue<String>(value);
        return std::find(domain_.begin(), domain_.end(), svalue) != domain_.end();
    }
private:
    std::vector<String> domain_;
};

class RealValueRange : public ValueRange {
public:
    explicit RealValueRange(TypedColumnData const& column);
    TypeId GetTypeId() const override { return TypeId::kDouble; }
    bool Includes(const std::byte* value) const override {
        Double dvalue = Type::GetValue<Double>(value);
        bool flipped = lower_bound_ > upper_bound_;
        if(flipped) {
            return dvalue < lower_bound_ && dvalue > upper_bound_;
        } else {
            return dvalue > lower_bound_ && dvalue < upper_bound_;
        }
    }
private:
    Double upper_bound_;
    Double lower_bound_;
};

class IntegerValueRange : public ValueRange {
public:
    explicit IntegerValueRange(TypedColumnData const& column);
    TypeId GetTypeId() const override { return TypeId::kInt; }
    bool Includes(const std::byte* value) const override {
        Int ivalue = Type::GetValue<Int>(value);
        bool flipped = lower_bound_ > upper_bound_;
        if(flipped) {
            return ivalue < lower_bound_ && ivalue > upper_bound_;
        } else {
            return ivalue > lower_bound_ && ivalue < upper_bound_;
        }
    }
private:
    Int upper_bound_;
    Int lower_bound_;
};

std::shared_ptr<ValueRange> CreateValueRange(TypedColumnData const& column);

} // namespace model
