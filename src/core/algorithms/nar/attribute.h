#pragma once

#include "model/types/type.h"
#include "model/table/column_layout_typed_relation_data.h"
#include "algorithms/nar/enums.h"

namespace algos {

class Attribute {
public:
    virtual FeatureTypeId GetTypeId() = 0;
protected:
    Attribute(){}; 
};

class CategoricalAttribute : public Attribute {
public:
    explicit CategoricalAttribute(std::string value) : value_(value) {};
    FeatureTypeId GetTypeId() override {
        return FeatureTypeId::kCategorical;
    }
private:
    std::string value_;
};

class RealAttribute : public Attribute {
public:
    explicit RealAttribute(double value) : value_(value) {};
    FeatureTypeId GetTypeId() override {
        return FeatureTypeId::kReal;
    }
private:
    double value_;
};

class IntegerAttribute : public Attribute {
public:
    explicit IntegerAttribute(long int value) : value_(value) {};
    FeatureTypeId GetTypeId() override {
        return FeatureTypeId::kInteger;
    }
private:
    long int value_;
};

} // namespace algos
