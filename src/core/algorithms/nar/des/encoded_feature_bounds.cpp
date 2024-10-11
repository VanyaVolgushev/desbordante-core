#include "encoded_feature_bounds.h"

namespace algos::des {

std::shared_ptr<EncodedFeatureBounds> CreateEncodedFeatureBounds(model::FeatureTypeId typeId) {
    using model::FeatureTypeId;
    switch (typeId) {
        case FeatureTypeId::kCategorical:
            return std::make_shared<EncodedCategoricalFeatureBounds>(EncodedCategoricalFeatureBounds());
        case FeatureTypeId::kInteger:
            return std::make_shared<EncodedIntegerFeatureBounds>(EncodedIntegerFeatureBounds());
        case FeatureTypeId::kReal:
            return std::make_shared<EncodedRealFeatureBounds>(EncodedRealFeatureBounds());
        default:
            throw std::invalid_argument(std::string("Column has invalid type_id in function: ") + __func__);
    }
}

} // namespace algos::des
