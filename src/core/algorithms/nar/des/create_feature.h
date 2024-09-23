#pragma once

#include "feature.h"

namespace algos::des {
    using model::Type;
    using model::TypeId;

    Feature CreateFeature() {
        switch (type_id) {
            case TypeId::kInt:
                return IntegerFeature();
            case TypeId::kDouble:
                return RealFeature();
            case TypeId::kString:
                return CategoricalFeature();
            default:
                throw std::invalid_argument(std::string("Invalid type_id in function: ") + __func__);
        }
    }

}  // namespace model
