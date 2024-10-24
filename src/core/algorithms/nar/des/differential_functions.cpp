#include "differential_functions.h"
#include <algorithm>
#include "counter.h"

namespace algos::des {

// gets quite slow if population ~= number_of_indices
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

double CrossOverForBest1Exp(double best_individual_val, double sample1_val,
                 double sample2_val, double sample3_val, DifferentialOptions opt) {
    double result = sample1_val + opt.differential_scale * (sample2_val - sample3_val);
    return std::clamp(result, 0.0, 1.0) ;
}

//TODO: this is bad
EncodedNAR Rand1Bin(std::vector<EncodedNAR> population,
                    EncodedNAR best_individual,
                    size_t candidate_index,
                    DifferentialOptions options) {
    auto candidate = population[candidate_index];
    auto sample_indices = GetRandIndices(candidate_index,
                                             population.size(), 3);
    size_t sample_index1 = *std::next(sample_indices.begin(), 0);
    size_t sample_index2 = *std::next(sample_indices.begin(), 1);
    size_t sample_index3 = *std::next(sample_indices.begin(), 2);

    auto trial_individual = EncodedNAR(candidate.encoded_feature_ranges.size());
    auto sample1 = population[sample_index1];
    auto sample2 = population[sample_index2];
    auto sample3 = population[sample_index3];    

    if (RNG().Next() < options.crossover_probability) {
        trial_individual.implication_sign_pos = CrossOverForBest1Exp(best_individual.implication_sign_pos,
                                                          sample1.implication_sign_pos,
                                                          sample2.implication_sign_pos,
                                                          sample3.implication_sign_pos,
                                                          options);
    }
    size_t rshift = RNG().Next() * trial_individual.encoded_feature_ranges.size();
    for (size_t n; n< trial_individual.encoded_feature_ranges.size(); n++) {
        size_t i = (n + rshift) % trial_individual.encoded_feature_ranges.size();
        if (RNG().Next() < options.crossover_probability) {
            trial_individual.encoded_feature_ranges[i].bound1 = CrossOverForBest1Exp(best_individual.encoded_feature_ranges[i].bound1,
                                                          sample1.encoded_feature_ranges[i].bound1,
                                                          sample2.encoded_feature_ranges[i].bound1,
                                                          sample3.encoded_feature_ranges[i].bound1,
                                                          options);
        }
        if (RNG().Next() < options.crossover_probability) {
            trial_individual.encoded_feature_ranges[i].bound2 = CrossOverForBest1Exp(best_individual.encoded_feature_ranges[i].bound2,
                                                          sample1.encoded_feature_ranges[i].bound2,
                                                          sample2.encoded_feature_ranges[i].bound2,
                                                          sample3.encoded_feature_ranges[i].bound2,
                                                          options);
        }
        if (RNG().Next() < options.crossover_probability) {
            trial_individual.encoded_feature_ranges[i].permutation = CrossOverForBest1Exp(best_individual.encoded_feature_ranges[i].permutation,
                                                          sample1.encoded_feature_ranges[i].permutation,
                                                          sample2.encoded_feature_ranges[i].permutation,
                                                          sample3.encoded_feature_ranges[i].permutation,
                                                          options);
        }
        if (RNG().Next() < options.crossover_probability) {
            trial_individual.encoded_feature_ranges[i].threshold = CrossOverForBest1Exp(best_individual.encoded_feature_ranges[i].threshold,
                                                          sample1.encoded_feature_ranges[i].threshold,
                                                          sample2.encoded_feature_ranges[i].threshold,
                                                          sample3.encoded_feature_ranges[i].threshold,
                                                          options);
        }
    }
    Counter().Next();
    return trial_individual;
}

typedef EncodedNAR (*MutationFunction)(std::vector<EncodedNAR> population,
                                       EncodedNAR best_individual,
                                       size_t candidate_index,
                                       DifferentialOptions options);

MutationFunction EnumToMutationStrategy(DifferentialStrategy strategy) {
    switch (strategy)
    {
    case DifferentialStrategy::rand1Bin:
        return Rand1Bin;
        break;
    default:
        throw std::logic_error("No mutation function corresponding to DifferentialStategy.");
    }
}

} // namespace algos::des