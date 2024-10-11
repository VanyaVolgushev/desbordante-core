#include "encoded_nar.h"

namespace algos::des {

using model::NAR;

double EncodedNAR::Evaluate(TypedRelation const* typed_relation) {
    NAR this_decoded = Decode();
    if(this_decoded.ante_.size() == 0 || this_decoded.cons_.size() == 0) {
        return 0.0;
    }
    //this_decoded.
}

NAR EncodedNAR::Decode() const {
    //TODO
    return NAR();
}

EncodedNAR::EncodedNAR(const std::vector<FeatureTypeId> feature_types) {
    for (size_t i = 0; i < feature_types.size(); i++)
    {
        auto encoded_feature_bounds = CreateEncodedFeatureBounds(feature_types[i]);
        encoded_feature_bounds_vec_.emplace_back(encoded_feature_bounds);
    }
}

}  // namespace algos::des
