#pragma once

#include "encoded_value_range.h"
#include "algorithms/nar/nar.h"
#include "model/table/column_layout_typed_relation_data.h"

namespace algos::des {

class EncodedNAR {
    using TypedRelation = model::ColumnLayoutTypedRelationData;
    using FeatureDomains = const std::vector<std::shared_ptr<model::ValueRange>>;
public:
    model::NARQualities qualities;

    double implication_sign_pos = -1;
    std::vector<EncodedValueRange> encoded_feature_ranges = std::vector<EncodedValueRange>();
    // sets fitness support and confidence
    void SetQualities(FeatureDomains domains, TypedRelation const* typed_relation);
    model::NAR Decode(FeatureDomains domains) const;
    EncodedNAR(FeatureDomains domains, TypedRelation const* typed_relation);
private:
//    double CalculateSupport(model::NAR, FeatureDomains domains, TypedRelation const* typed_relation);
//    double CalculateConfidence(model::NAR, FeatureDomains domains, TypedRelation const* typed_relation);
};

} // namespace algos::des
