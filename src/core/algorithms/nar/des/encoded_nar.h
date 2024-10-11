#pragma once

#include "encoded_feature_bounds.h"
#include "algorithms/nar/nar.h"
#include "model/table/column_layout_typed_relation_data.h"

namespace algos::des {
    using model::FeatureTypeId;

class EncodedNAR {
    using TypedRelation = model::ColumnLayoutTypedRelationData;
    using FeatureDomains = const std::vector<std::shared_ptr<model::FeatureBounds>>;
public:
    double fitness_ = -1;
    double support_ = -1;
    double confidence_ = -1;

    double implication_sign_pos_ = -1;
    std::vector<std::shared_ptr<EncodedFeatureBounds>> encoded_feature_bounds_vec_;

    double Evaluate(TypedRelation const* typed_relation);
    model::NAR Decode() const;
    EncodedNAR(const std::vector<FeatureTypeId> feature_types);
};

} // namespace algos::des
