#pragma once

#include "feature.h"
#include "model/table/column_layout_typed_relation_data.h"

namespace algos::des {

using model::Type;
using model::TypeId;

std::unique_ptr<Feature> CreateFeature(model::TypedColumnData const& column) {
    switch (column.GetTypeId()) {
        case TypeId::kInt:
            return std::make_unique<IntegerFeature>(IntegerFeature(column));
        case TypeId::kDouble:
            return std::make_unique<RealFeature>(RealFeature(column));
        case TypeId::kString:
            return std::make_unique<CategoricalFeature>(CategoricalFeature(column));
        case TypeId::kMixed:
            return std::make_unique<CategoricalFeature>(CategoricalFeature(column));
        default:
            throw std::invalid_argument(std::string("Column has invalid type_id in function: ") + __func__);
    }
}

}  // namespace model
