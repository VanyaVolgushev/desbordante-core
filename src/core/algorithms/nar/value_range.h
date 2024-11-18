#pragma once

#include <enum.h>

#include "model/table/column_layout_typed_relation_data.h"
#include "model/types/type.h"

namespace model {

class ValueRange {
public:
    TypeId GetTypeId() const;
    virtual bool Includes(std::byte const* value) const = 0;
    virtual std::string ToString() const = 0;

protected:
    std::unique_ptr<model::Type> type_ = nullptr;
    ValueRange() {};
    virtual ~ValueRange() = default;
};

class CategoricalValueRange : public ValueRange {
    std::vector<std::byte const*> domain_;

public:
    explicit CategoricalValueRange(TypedColumnData const& column);

    CategoricalValueRange(std::byte const* data) : domain_() {
        domain_.emplace_back(data);
        type_ = std::make_unique<Type>(TypeId::kString);
    }

    bool Includes(std::byte const* value) const override {
        String pattern = Type::GetValue<String>(value);
        auto found =
                std::find_if(domain_.begin(), domain_.end(), [pattern](std::byte const*& data) {
                    return Type::GetValue<String>(data) == pattern;
                });

        return found != domain_.end();
    }

    std::string ToString() const override;

    ~CategoricalValueRange() {
        for (auto v : domain_) type_->Free(v);
    }
};

class NumericalValueRange : public ValueRange {
public:
    std::byte const* lower_bound_;
    std::byte const* upper_bound_;
    explicit NumericalValueRange(TypedColumnData const& column);

    NumericalValueRange(std::byte const* lower_bound, std::byte const* upper_bound,
                        model::Type const* type)
        : lower_bound_(lower_bound), upper_bound_(upper_bound) {
        this->type_ = type->CloneType();
    };

    bool Includes(std::byte const* value) const override {
        auto res_with_lower = type_->Compare(value, lower_bound_);
        auto res_with_upper = type_->Compare(value, upper_bound_);
        return (res_with_lower == CompareResult::kGreater ||
                res_with_lower == CompareResult::kEqual) &&
                (res_with_upper == CompareResult::kLess ||
                res_with_upper == CompareResult::kEqual);
    }

    std::string ToString() const override;

    ~NumericalValueRange() {
        type_->Free(lower_bound_);
        type_->Free(upper_bound_);
    }
};

std::shared_ptr<ValueRange> CreateValueRange(TypedColumnData const& column);

}  // namespace model
