#include "encoded_nar.h"
#include "algorithms/nar/value_range.h"
#include "model/types/types.h"

namespace algos::des {

using model::NAR;

size_t EncodedNAR::VectorSize() {
    return encoded_feature_ranges_.size() * EncodedValueRange().field_count + 1;
}

size_t EncodedNAR::FeatureCount() {
    return encoded_feature_ranges_.size();
}

//TODO: remove code duplication here
double& EncodedNAR::operator[] (size_t index) {
    qualities_consistent_ = false;
            if(index == 0) {
        return implication_sign_pos_;
    } else {
        index--;
        size_t feature = index / EncodedValueRange().field_count;
        size_t feature_field = index % EncodedValueRange().field_count;
        return encoded_feature_ranges_[feature][feature_field];
    }
}

const double& EncodedNAR::operator[](size_t index) const {
    if(index == 0) {
        return implication_sign_pos_;
    } else {
        index--;
        size_t feature = index / EncodedValueRange().field_count;
        size_t feature_field = index % EncodedValueRange().field_count;
        return encoded_feature_ranges_[feature][feature_field];
    }
}
 
NAR EncodedNAR::SetQualities(FeatureDomains domains, TypedRelation const* typed_relation) {
    NAR this_decoded = Decode(domains);
    this_decoded.SetQualities(typed_relation);
    qualities_ = this_decoded.GetQualities();
    qualities_consistent_ = true;
    return this_decoded;
}

const model::NARQualities& EncodedNAR::GetQualities() const {
    if (!qualities_consistent_) {
        throw std::logic_error("Getting uninitialized qualities from NAR.");
    }
    return qualities_;
}

NAR EncodedNAR::Decode(FeatureDomains domains) const {
    auto resultingNAR = model::NAR();

    std::vector<size_t> feature_order(encoded_feature_ranges_.size());
    std::iota(std::begin(feature_order), std::end(feature_order), 0);
    auto CompareByPermutation  = [e=encoded_feature_ranges_](size_t& a, size_t& b) -> bool {
        return e[a].permutation > e[b].permutation;
    };
    std::sort(feature_order.begin(), feature_order.end(), CompareByPermutation);

    uint implication_sign_after = implication_sign_pos_ * (encoded_feature_ranges_.size() - 1);
    uint handling_feat_num = -1;
    for(size_t feature_index: feature_order) {
        handling_feat_num++;
        EncodedValueRange encoded_feature = encoded_feature_ranges_[feature_index];
        if (encoded_feature.threshold < RNG().Next()) {
            continue;
        }
        auto domain = domains[feature_index];
        auto decoded = encoded_feature_ranges_[feature_index].Decode(domain);
        if(handling_feat_num > implication_sign_after) {
            resultingNAR.InsertInCons(feature_index, decoded);
        } else {
            resultingNAR.InsertInAnte(feature_index, decoded);
        }
    }
    return resultingNAR;
}

EncodedNAR::EncodedNAR(FeatureDomains domains, TypedRelation const* typed_relation) {
    size_t feature_count = domains.size();
    for(size_t feature_index = 0; feature_index < feature_count; feature_index++) {
        encoded_feature_ranges_.emplace_back(EncodedValueRange());
    }
    implication_sign_pos_ = RNG().Next();
    SetQualities(domains, typed_relation);
}

EncodedNAR::EncodedNAR(size_t feature_count) {
    for(size_t feature_index = 0; feature_index < feature_count; feature_index++) {
        encoded_feature_ranges_.emplace_back(EncodedValueRange());
    }
    implication_sign_pos_ = RNG().Next();
}

}  // namespace algos::des
