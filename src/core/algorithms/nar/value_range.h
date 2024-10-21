#pragma once

#include "model/types/type.h"
#include "model/table/column_layout_typed_relation_data.h"
#include <enum.h>

namespace model {

class ValueRange {
public:
    virtual TypeId GetTypeId() const = 0;
    virtual bool Includes(const std::byte* value) const = 0;
    virtual std::string ToString() const = 0;
protected:
    ValueRange() {}; 
};

class StringValueRange : public ValueRange {
public:
    std::vector<String> domain_;
    explicit StringValueRange(TypedColumnData const& column);
    StringValueRange(String value) : domain_() { domain_.emplace_back(value); };
    TypeId GetTypeId() const override { return TypeId::kString; }
    bool Includes(const std::byte* value) const override {
        String svalue = Type::GetValue<String>(value);
        return std::find(domain_.begin(), domain_.end(), svalue) != domain_.end();
    }
    //TODO: move tostrings to .cpp
    std::string ToString() const override {
        std::string result;
        result += "[";
        if(domain_.size() > 0) {
            result += domain_[0];
        }
        for (size_t i = 1; i < domain_.size(); i++) {
            result += (", " + domain_[i]);
        }
        result += "]";
        return result;
    }
};
//TODO: rename upper bound to bound1 and bound2 in EncodedNAR
class DoubleValueRange : public ValueRange {
public:
    Double lower_bound_;
    Double upper_bound_;
    explicit DoubleValueRange(TypedColumnData const& column);
    DoubleValueRange(Double lower_bound, Double upper_bound) : lower_bound_(lower_bound), upper_bound_(upper_bound) {};
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
    std::string ToString() const override {
        std::string result;
        result += "[";
        result += std::to_string(lower_bound_);
        result += " - ";
        result += std::to_string(upper_bound_);
        result += "]";
        return result;
    }
};

class IntValueRange : public ValueRange {
public:
    Int lower_bound_;
    Int upper_bound_;
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
    std::string ToString() const override {
        std::string result;
        result += "[";
        result += std::to_string(lower_bound_);
        result += " - ";
        result += std::to_string(upper_bound_);
        result += "]";
        return result;
    }
    explicit IntValueRange(TypedColumnData const& column);
    IntValueRange(Int lower_bound, Int upper_bound) : lower_bound_(lower_bound), upper_bound_(upper_bound) {};
};

std::shared_ptr<ValueRange> CreateValueRange(TypedColumnData const& column);

} // namespace model
