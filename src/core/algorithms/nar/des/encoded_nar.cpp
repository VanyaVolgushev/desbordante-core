#include "encoded_nar.h"

namespace algos::des {

using model::NAR;

double EncodedNAR::Evaluate(FeatureDomains domains, TypedRelation const* typed_relation) {
    NAR this_decoded = Decode(domains);
    if(this_decoded.ante_.size() == 0 || this_decoded.cons_.size() == 0) {
        return 0.0;
    }
    for (size_t i = 0; i < typed_relation->GetNumRows(); i++) {
        for(model::TypedColumnData const& column: typed_relation->GetColumnData()) {
            column[i] //TODO: RIGHT NOW
        }
    }


    //this_decoded.
}

NAR EncodedNAR::Decode(FeatureDomains domains) const {
    auto resultNAR = NAR();

    return resultNAR;
}

EncodedNAR::EncodedNAR(FeatureDomains domains, TypedRelation const* typed_relation) {
    //TODO
    Evaluate(domains, typed_relation);
}

}  // namespace algos::des
