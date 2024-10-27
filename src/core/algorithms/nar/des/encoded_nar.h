#pragma once

#include "encoded_value_range.h"
#include "algorithms/nar/nar.h"
#include "model/table/column_layout_typed_relation_data.h"

namespace algos::des {

class EncodedNAR {
    using TypedRelation = model::ColumnLayoutTypedRelationData;
    using FeatureDomains = const std::vector<std::shared_ptr<model::ValueRange>>;
public:

    size_t VectorSize();
    size_t FeatureCount();
    double& operator[] (size_t index);
    double const& operator[] (size_t index) const;

    const model::NARQualities& GetQualities() const;
    model::NAR SetQualities(FeatureDomains domains, TypedRelation const* typed_relation);

    model::NAR Decode(FeatureDomains domains) const;
    EncodedNAR(FeatureDomains domains, TypedRelation const* typed_relation);
    EncodedNAR(size_t feature_count);
private:
    double implication_sign_pos_ = -1;
    std::vector<EncodedValueRange> encoded_feature_ranges_ = std::vector<EncodedValueRange>();
    
    model::NARQualities qualities_;
    bool qualities_consistent_ = false;
//    double CalculateSupport(model::NAR, FeatureDomains domains, TypedRelation const* typed_relation);
//    double CalculateConfidence(model::NAR, FeatureDomains domains, TypedRelation const* typed_relation);
};

} // namespace algos::des
