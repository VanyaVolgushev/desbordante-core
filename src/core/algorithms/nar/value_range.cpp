#include "value_range.h"

namespace model {

    //TODO: ADD NULL CHECKING (IN GENERAL)

CategoricalValueRange::CategoricalValueRange(model::TypedColumnData const& column) {
    domain_ = std::vector<std::string>();
    for (size_t row_index = 0; row_index < column.GetNumRows(); row_index++) {
        const std::byte* value = column.GetValue(row_index);
        std::string string_value = model::Type::GetValue<std::string>(value);
        bool first_occurrence = std::find(domain_.begin(),
                                          domain_.end(), string_value) == domain_.end();
        if(first_occurrence) {
            domain_.emplace_back(string_value);
        }
    }
}

IntegerValueRange::IntegerValueRange(model::TypedColumnData const& column) {
    bool initialized = false;
    for (size_t row_index = 0; row_index < column.GetNumRows(); row_index++) {
        const std::byte* value = column.GetValue(row_index);
        long int int_value = model::Type::GetValue<model::Int>(value);
        if(!initialized) {
            lower_bound_ = int_value;
            upper_bound_ = int_value;
            initialized = true;
            continue;
        }
        if(int_value > upper_bound_) {
            upper_bound_ = int_value;
        }
        if(int_value < lower_bound_)  {
            lower_bound_ = int_value;
        }
    }
}

RealValueRange::RealValueRange(model::TypedColumnData const& column) {
    bool initialized = false;
    for (size_t row_index = 0; row_index < column.GetNumRows(); row_index++) {
        const std::byte* value = column.GetValue(row_index);
        double double_value = model::Type::GetValue<model::Double>(value); // is it okay to use a non-long double here? clion says GetValue returns just double.
        if(!initialized) {
            lower_bound_ = double_value;
            upper_bound_ = double_value;
            initialized = true;
            continue;
        }
        if(double_value > upper_bound_) {
            upper_bound_ = double_value;
        }
        if(double_value < lower_bound_)  {
            lower_bound_ = double_value;
        }
    }
}

std::shared_ptr<ValueRange> CreateValueRange(model::TypedColumnData const& column) {
    switch (column.GetTypeId()) {
        case TypeId::kInt:
            return std::make_shared<IntegerValueRange>(IntegerValueRange(column));
        case TypeId::kDouble:
            return std::make_shared<RealValueRange>(RealValueRange(column));
        case TypeId::kString:
            return std::make_shared<CategoricalValueRange>(CategoricalValueRange(column));
        case TypeId::kMixed:
            return std::make_shared<CategoricalValueRange>(CategoricalValueRange(column));
        default:
            throw std::invalid_argument(std::string("Column has invalid type_id in function: ") + __func__);
    }
}

} // namespace model