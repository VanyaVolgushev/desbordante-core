#include "encoded_nar.h"

namespace algos::des {

using model::NAR;

double EncodedNAR::Evaluate(FeatureDomains domains, TypedRelation const* typed_relation) {
    double result = 0;
    NAR this_decoded = Decode(domains);
    if(this_decoded.ante_.size() == 0 || this_decoded.cons_.size() == 0) {
        return 0.0;
    }
    for (size_t rowi = 0; rowi < typed_relation->GetNumRows(); rowi++) {
        for(model::TypedColumnData const& column: typed_relation->GetColumnData()) {
            const std::byte* value = column.GetValue(rowi);
            if (domains[rowi]->Includes(value)) {
                result++;
            }
        }
    }
    return result;

    //this_decoded.
}

NAR EncodedNAR::Decode(FeatureDomains domains) const {
    auto resultNAR = NAR();
    //TODO
    return resultNAR;
}

EncodedNAR::EncodedNAR(FeatureDomains domains, TypedRelation const* typed_relation) {
    size_t feature_count = domains.size();
    for(size_t feature_index = 0; feature_index < feature_count; feature_index++) {
        encoded_feature_ranges.emplace_back(EncodedFeatureRange(feature_index));
    }
    implication_sign_pos_ = RNG().Next();
    Evaluate(domains, typed_relation);
}

}  // namespace algos::des
