#include "value_range.h"

namespace model {

ValueRange::~ValueRange() {}

StringValueRange::StringValueRange(TypedColumnData const& column) {
    for (size_t row_index = 0; row_index < column.GetNumRows(); ++row_index) {
        std::byte const* value = column.GetValue(row_index);
        std::string string_value = Type::GetValue<std::string>(value);
        bool first_occurrence = std::ranges::find(domain, string_value) == domain.end();
        if (first_occurrence) {
            domain.push_back(std::move(string_value));
        }
    }
}

bool StringValueRange::Includes(std::byte const* value) const {
    String const& svalue = Type::GetValue<String>(value);
    return std::ranges::find(domain, svalue) != domain.end();
}

std::string StringValueRange::ToString() const {
    std::ostringstream result;
    result << "[";
    if (domain.empty()) {
        return "]";
    }
    result << domain.front();
    for (size_t i = 1; i < domain.size(); ++i) {
        result << ", " + domain[i];
    }
    result << "]";
    return result.str();
}

std::shared_ptr<ValueRange> CreateValueRange(model::TypedColumnData const& column) {
    switch (column.GetTypeId()) {
        case TypeId::kString:
            return std::make_shared<StringValueRange>(column);
        case TypeId::kDouble:
            return std::make_shared<NumericValueRange<Double>>(column);
        case TypeId::kInt:
            return std::make_shared<NumericValueRange<Int>>(column);
        default:
            throw std::invalid_argument(std::string("Column has invalid type_id in function: ") +
                                        __func__);
    }
}

}  // namespace model
