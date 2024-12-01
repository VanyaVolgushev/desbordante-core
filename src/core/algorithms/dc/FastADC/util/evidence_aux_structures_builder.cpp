#include "evidence_aux_structures_builder.h"

#include <easylogging++.h>

namespace algos::fastadc {

// TODO: no, just accept lambda by typaname Func with &&, cast to std::function is costly
void EvidenceAuxStructuresBuilder::BuildAll(PredicatesVector const& predicates, size_t group_size,
                                            PackAction action) {
    assert(predicates.size() % group_size == 0);
    size_t num_groups = predicates.size() / group_size;

    for (size_t i = 0; i < num_groups; ++i) {
        size_t base_index = i * group_size;
        PredicatesSpan group_span(predicates.begin() + base_index, group_size);

        action(group_span);
    }
}

void EvidenceAuxStructuresBuilder::ProcessNumPredicates(PredicatesVector const& predicates,
                                                        std::vector<PredicatePack>& pack,
                                                        size_t& count) {
    PackAction action = [&](PredicatesSpan const& group_span) {
        PredicatePtr eq = GetPredicateByType(group_span, OperatorType::kEqual);
        PredicatePtr gt = GetPredicateByType(group_span, OperatorType::kGreater);

        static constexpr OperatorType kEqList[] = {OperatorType::kEqual, OperatorType::kUnequal,
                                                   OperatorType::kLess,
                                                   OperatorType::kGreaterEqual};
        static constexpr OperatorType kGtList[] = {OperatorType::kLess, OperatorType::kGreater,
                                                   OperatorType::kLessEqual,
                                                   OperatorType::kGreaterEqual};
        static constexpr OperatorType kCardinality[] = {OperatorType::kUnequal, OperatorType::kLess,
                                                        OperatorType::kLessEqual};

        correction_map_[count] = BuildMask(group_span, kEqList);
        correction_map_[count + 1] = BuildMask(group_span, kGtList);
        cardinality_mask_ |= BuildMask(group_span, kCardinality);

        pack.emplace_back(eq, count, gt, count + 1);
        count += 2;
    };

    BuildAll(predicates, 6, action);
}

void EvidenceAuxStructuresBuilder::ProcessCatPredicates(PredicatesVector const& predicates,
                                                        std::vector<PredicatePack>& pack,
                                                        size_t& count) {
    PackAction action = [&](PredicatesSpan const& group_span) {
        PredicatePtr eq = GetPredicateByType(group_span, OperatorType::kEqual);

        static constexpr OperatorType kEqList[] = {OperatorType::kEqual, OperatorType::kUnequal};
        static constexpr OperatorType kCardinality[] = {OperatorType::kUnequal};

        correction_map_[count] = BuildMask(group_span, kEqList);
        cardinality_mask_ |= BuildMask(group_span, kCardinality);

        pack.emplace_back(eq, count);
        count++;
    };

    BuildAll(predicates, 2, action);
}

void EvidenceAuxStructuresBuilder::BuildAll() {
    size_t count = 0;

    /*
     * The size of the correction map is calculated based on the number of categorical and
     * numerical predicates.
     * Categorical predicates are represented by 1 bit per pair of attributes, so we divide by 2.
     * Numerical predicates are represented by 2 bits per pair of attributes, and there are 6
     * possible numerical predicates, so we divide by 6 and multiply by 2 to account for the extra
     * bits.
     */
    correction_map_.resize(str_single_.size() / 2 + str_cross_.size() / 2 +
                           2 * num_single_.size() / 6 + 2 * num_cross_.size() / 6);

    ProcessCatPredicates(str_single_, packs_.str_single, count);
    ProcessCatPredicates(str_cross_, packs_.str_cross, count);
    ProcessNumPredicates(num_single_, packs_.num_single, count);
    ProcessNumPredicates(num_cross_, packs_.num_cross, count);

    LOG(DEBUG) << "  [CLUE] # of bits in clue: " << count;
    assert(count <= kPredicateBits);
}

}  // namespace algos::fastadc
