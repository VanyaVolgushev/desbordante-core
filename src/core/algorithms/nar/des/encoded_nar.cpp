#include "encoded_nar.h"
#include "algorithms/nar/value_range.h"
#include "model/types/types.h"

namespace algos::des {

using model::NAR;


void EncodedNAR::SetQualities(FeatureDomains domains, TypedRelation const* typed_relation) {
    auto result = Decode(domains).SetQualities(typed_relation);
    qualities = result;
}

NAR EncodedNAR::Decode(FeatureDomains domains) const {
    auto resultingNAR = model::NAR();
    resultingNAR.qualities = qualities;

    std::vector<size_t> feature_order(encoded_feature_ranges.size());
    std::iota(std::begin(feature_order), std::end(feature_order), 0);
    auto CompareByPermutation  = [e=encoded_feature_ranges](size_t& a, size_t& b) -> bool {
        return e[a].permutation > e[b].permutation;
    };
    std::sort(feature_order.begin(), feature_order.end(), CompareByPermutation);

    uint implication_sign_after = implication_sign_pos * (encoded_feature_ranges.size() - 1);
    uint handling_feat_num = -1;
    for(size_t feature_index: feature_order) {
        handling_feat_num++;
        EncodedValueRange encoded_feature = encoded_feature_ranges[feature_index];
        if (encoded_feature.threshold < 0.5) {
            continue;
        }
        auto domain = domains[feature_index];
        auto decoded = encoded_feature_ranges[feature_index].Decode(domain);
        if(handling_feat_num > implication_sign_after) {
            resultingNAR.cons.insert({feature_index, decoded});
        } else {
            resultingNAR.ante.insert({feature_index, decoded});
        }
    }
    return resultingNAR;
}

EncodedNAR::EncodedNAR(FeatureDomains domains, TypedRelation const* typed_relation) {
    size_t feature_count = domains.size();
    for(size_t feature_index = 0; feature_index < feature_count; feature_index++) {
        encoded_feature_ranges.emplace_back(EncodedValueRange());
    }
    implication_sign_pos = RNG().Next();
    SetQualities(domains, typed_relation);
}

EncodedNAR::EncodedNAR(size_t feature_count) {
    for(size_t feature_index = 0; feature_index < feature_count; feature_index++) {
        encoded_feature_ranges.emplace_back(EncodedValueRange());
    }
    implication_sign_pos = RNG().Next();
}

}  // namespace algos::des
