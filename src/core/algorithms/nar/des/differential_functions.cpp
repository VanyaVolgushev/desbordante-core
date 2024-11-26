#include "differential_functions.h"

#include <algorithm>

namespace algos::des {

// gets slow if population ~= number_of_indices
std::unordered_set<size_t> GetRandIndices(size_t except_index, size_t population,
                                          size_t number_of_indices) {
    std::unordered_set<size_t> indices;
    indices.insert(except_index);
    while (indices.size() < number_of_indices + 1) {
        size_t random_index = (size_t)(RNG().Next() * population);
        indices.insert(random_index);
    }
    indices.erase(except_index);
    return indices;
}

EncodedNAR Rand1Bin(std::vector<EncodedNAR> const& population, size_t candidate_index,
                    DifferentialOptions options) {
    auto candidate = population[candidate_index];
    auto sample_indices = GetRandIndices(candidate_index, population.size(), 3);
    size_t sample_index1 = *std::next(sample_indices.begin(), 0);
    size_t sample_index2 = *std::next(sample_indices.begin(), 1);
    size_t sample_index3 = *std::next(sample_indices.begin(), 2);

    auto new_individual = population[candidate_index];
    auto sample1 = population[sample_index1];
    auto sample2 = population[sample_index2];
    auto sample3 = population[sample_index3];

    for (size_t i = 0; i < new_individual.VectorSize(); i++) {
        if (RNG().Next() < options.crossover_probability) {
            double new_feature_val =
                    sample1[i] + options.differential_scale * (sample2[i] - sample3[i]);
            new_feature_val = std::clamp(new_feature_val, 0.0, 1.0);
            new_individual[i] = new_feature_val;
        }
    }
    return new_individual;
}

typedef EncodedNAR (*MutationFunction)(std::vector<EncodedNAR> const& population, size_t candidate_index,
                                       DifferentialOptions options);

// TODO: name is probably inconsistent with how it's called in the field.
MutationFunction EnumToMutationStrategy(DifferentialStrategy strategy) {
    switch (strategy) {
        case DifferentialStrategy::rand1Bin:
            return Rand1Bin;
            break;
        default:
            throw std::logic_error("No mutation function corresponding to DifferentialStategy.");
    }
}

}  // namespace algos::des
