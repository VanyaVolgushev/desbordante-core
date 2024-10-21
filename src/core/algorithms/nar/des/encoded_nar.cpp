#include "encoded_nar.h"
#include "algorithms/nar/value_range.h"
#include "model/types/types.h"

namespace algos::des {

using model::NAR;

//TODO: this function is way too big and cluttered
void EncodedNAR::Evaluate(FeatureDomains domains, TypedRelation const* typed_relation) {
    NAR this_decoded = Decode(domains);
    if(this_decoded.ante.size() == 0 || this_decoded.cons.size() == 0) {
        fitness = 0.0;
        return;
    }

    uint num_rows_fit_ante = 0;
    uint num_rows_fit_ante_and_cons = 0;
    for (size_t rowi = 0; rowi < typed_relation->GetNumRows(); rowi++) {
        bool row_fits_ante = true;
        bool row_fits_cons = true;
        for(size_t coli = 0; coli < typed_relation->GetNumColumns(); coli++) {
            const model::TypedColumnData& column = typed_relation->GetColumnData(coli);
            auto data = column.GetValue(rowi);

            bool ante_contains_feature = this_decoded.ante.find(coli) != this_decoded.ante.end();
            if(ante_contains_feature) {
                if(!this_decoded.ante[coli]->Includes(data)) {
                    row_fits_ante = false;
                    break;
                }
                continue;
            }
            bool cons_contains_feature = this_decoded.cons.find(coli) != this_decoded.cons.end();
            if(cons_contains_feature) {
                if(!this_decoded.cons[coli]->Includes(data)) {
                    row_fits_cons = false;
                }
                continue;
            }
        }
        if(row_fits_ante) {
            num_rows_fit_ante++;
        }
        if(row_fits_ante && row_fits_cons) {
            num_rows_fit_ante_and_cons++;
        }
    }
    
    if (num_rows_fit_ante == 0) {
        confidence = 0;
    } else {
        confidence = num_rows_fit_ante_and_cons / (double)num_rows_fit_ante;
    }
    support = num_rows_fit_ante_and_cons / (double)typed_relation->GetNumRows();
    double inclusion = (this_decoded.ante.size() + this_decoded.cons.size()) / (double)domains.size();
    fitness = (confidence + support + inclusion) / 3.0;
}

NAR EncodedNAR::Decode(FeatureDomains domains) const {
    auto resultingNAR = NAR();
    resultingNAR.fitness = fitness;
    resultingNAR.support = support;
    resultingNAR.confidence = confidence;

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
        if (encoded_feature.threshold < RNG().Next()) {
            continue;
        }
        auto domain = domains[feature_index];
        if(handling_feat_num > implication_sign_after) {
            resultingNAR.cons[feature_index] = encoded_feature_ranges[feature_index].Decode(domain);
        } else {
            resultingNAR.ante[feature_index] = encoded_feature_ranges[feature_index].Decode(domain);
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
    Evaluate(domains, typed_relation);
}

}  // namespace algos::des
