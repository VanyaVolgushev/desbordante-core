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
    //for(size_t feature_index; feature_index) {
    //    encoded_feature_bounds_vec_.
    //}
    Evaluate(domains, typed_relation);
}

}  // namespace algos::des
