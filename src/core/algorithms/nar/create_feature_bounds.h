#pragma once

#include "feature_bounds.h"
#include "model/table/column_layout_typed_relation_data.h"

namespace algos {

using model::Type;
using model::TypeId;

std::shared_ptr<FeatureBounds> CreateFeatureBounds(model::TypedColumnData const& column) {
    switch (column.GetTypeId()) {
        case TypeId::kInt:
            return std::make_shared<IntegerFeatureBounds>(IntegerFeatureBounds(column));
        case TypeId::kDouble:
            return std::make_shared<RealFeatureBounds>(RealFeatureBounds(column));
        case TypeId::kString:
            return std::make_shared<CategoricalFeatureBounds>(CategoricalFeatureBounds(column));
        case TypeId::kMixed:
            return std::make_shared<CategoricalFeatureBounds>(CategoricalFeatureBounds(column));
        default:
            throw std::invalid_argument(std::string("Column has invalid type_id in function: ") + __func__);
    }
}

}  // namespace algos
