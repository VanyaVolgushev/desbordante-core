#include "value_range.h"

namespace model {

CategoricalValueRange::CategoricalValueRange(model::TypedColumnData const& column) {
    domain_ = std::vector<std::string>();
    for (size_t row_index = 0; row_index < column.GetNumRows(); row_index++) {
        std::byte const* value = column.GetValue(row_index);
        std::string string_value = model::Type::GetValue<std::string>(value);
        bool first_occurrence =
                std::find(domain_.begin(), domain_.end(), string_value) == domain_.end();
        if (first_occurrence) {
            domain_.emplace_back(string_value);
        }
    }
}

std::string CategoricalValueRange::ToString() const {
    std::string result;
    result += "[";
    if (domain_.size() > 0) {
        result += domain_[0];
    }
    for (size_t i = 1; i < domain_.size(); i++) {
        result += (", " + domain_[i]);
    }
    result += "]";
    return result;
}

std::string IntValueRange::ToString() const {
    std::string result;
    result += "[";
    result += std::to_string(lower_bound);
    result += " - ";
    result += std::to_string(upper_bound);
    result += "]";
    return result;
}

IntValueRange::IntValueRange(model::TypedColumnData const& column) {
    bool initialized = false;
    for (size_t row_index = 0; row_index < column.GetNumRows(); row_index++) {
        std::byte const* value = column.GetValue(row_index);
        long int int_value = model::Type::GetValue<model::Int>(value);
        if (!initialized) {
            lower_bound = int_value;
            upper_bound = int_value;
            initialized = true;
            continue;
        }
        if (int_value > upper_bound) {
            upper_bound = int_value;
        }
        if (int_value < lower_bound) {
            lower_bound = int_value;
        }
    }
}

NumericalValueRange::NumericalValueRange(model::TypedColumnData const& column) {
    bool initialized = false;
    for (size_t row_index = 0; row_index < column.GetNumRows(); row_index++) {
        std::byte const* value = column.GetValue(row_index);
        double double_value = model::Type::GetValue<model::Double>(
                value);  // is it okay to use a non-long double here? clion says GetValue returns
                         // just double.
        if (!initialized) {
            lower_bound = double_value;
            upper_bound = double_value;
            initialized = true;
            continue;
        }
        if (double_value > upper_bound) {
            upper_bound = double_value;
        }
        if (double_value < lower_bound) {
            lower_bound = double_value;
        }
    }
}

std::string NumericalValueRange::ToString() const {
    std::string result;
    result += "[";
    result += std::to_string(lower_bound);
    result += " - ";
    result += std::to_string(upper_bound);
    result += "]";
    return result;
}

std::shared_ptr<ValueRange> CreateValueRange(model::TypedColumnData const& column) {
    switch (column.GetTypeId()) {
        case TypeId::kInt:
        case TypeId::kDouble:
            return std::make_shared<NumericalValueRange>(column);
        case TypeId::kString:
            return std::make_shared<CategoricalValueRange>(column);
        default:
            throw std::invalid_argument(std::string("Column has invalid type_id in function: ") +
                                        __func__);
    }
}

}  // namespace model