#pragma once

#include <enum.h>
#include "enums.h"
#include "encoded_nar.h"
#include "rng.h"

namespace algos::des {

struct DifferentialOptions {
    double differential_scale;
    double crossover_probability;
    DifferentialStrategy differential_strategy = DifferentialStrategy::best1Exp;
};

//TODO: this is bad
EncodedNAR Rand1Bin(std::vector<EncodedNAR> population,
                    EncodedNAR best_individual,
                    size_t candidate_index,
                    DifferentialOptions options);

typedef EncodedNAR (*MutationFunction)(std::vector<EncodedNAR> population,
                                       EncodedNAR best_individual,
                                       size_t candidate_index,
                                       DifferentialOptions options);

MutationFunction EnumToMutationStrategy(DifferentialStrategy strategy);

} // namespace algos::des