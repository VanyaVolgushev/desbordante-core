#pragma once

#include <enum.h>

#include "encoded_nar.h"
#include "enums.h"
#include "rng.h"

namespace algos::des {

struct DifferentialOptions {
    double differential_scale;
    double crossover_probability;
    DifferentialStrategy differential_strategy = DifferentialStrategy::best1Exp;
};

EncodedNAR Rand1Bin(std::vector<EncodedNAR> const& population, size_t candidate_index,
                    DifferentialOptions options);

using MutationFunction = EncodedNAR (*)(std::vector<EncodedNAR> const& population, size_t candidate_index,
                                       DifferentialOptions options);

MutationFunction EnumToMutationStrategy(DifferentialStrategy strategy);

}  // namespace algos::des
