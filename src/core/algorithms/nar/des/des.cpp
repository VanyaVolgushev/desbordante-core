#include "des.h"

#include <algorithm>
#include <cstddef>
#include <memory>
#include <vector>

#include "algorithms/nar/value_range.h"
#include "config/names_and_descriptions.h"
#include "config/option_using.h"
#include "config/tabular_data/input_table/option.h"
#include "model/types/types.h"

namespace algos::des {
using model::ValueRange;

DES::DES() : NARAlgorithm({}) {
    RegisterOptions();
}

void DES::RegisterOptions() {
    DESBORDANTE_OPTION_USING;

    DifferentialStrategy default_strategy = DifferentialStrategy::rand1Bin;
    RegisterOption(Option{&population_size_, kPopulationSize, kDPopulationSize, 100u});
    RegisterOption(
            Option{&num_evaluations_, kMaxFitnessEvaluations, kDMaxFitnessEvaluations, 1000u});
    RegisterOption(Option{&differential_options_.differential_scale, kDifferentialScale,
                          kDDifferentialScale, 0.5});
    RegisterOption(Option{&differential_options_.crossover_probability, kCrossoverProbability,
                          kDCrossoverProbability, 0.9});
    RegisterOption(Option{&differential_options_.differential_strategy, kDifferentialStrategy,
                          kDDifferentialStrategy, default_strategy});
}

void DES::MakeExecuteOptsAvailable() {
    using namespace config::names;
    MakeOptionsAvailable({kPopulationSize, kMaxFitnessEvaluations, kDifferentialScale,
                          kCrossoverProbability, kDifferentialStrategy});
}

FeatureDomains DES::FindFeatureDomains(TypedRelation const* typed_relation) {
    std::vector<std::shared_ptr<ValueRange>> feature_domains;
    feature_domains.reserve(typed_relation->GetNumColumns());
    for (size_t i = 0; i < typed_relation->GetNumColumns(); ++i) {
        std::shared_ptr<ValueRange> domain = CreateValueRange(typed_relation->GetColumnData(i));
        feature_domains.push_back(std::move(domain));
    }
    return feature_domains;
}

std::vector<EncodedNAR> DES::GetRandomPopulationInDomains(FeatureDomains const& domains,
                                                          RNG& rng) const {
    std::vector<EncodedNAR> population;
    population.reserve(population_size_);
    for (unsigned i = 0; i < population_size_; ++i) {
        population.emplace_back(domains, typed_relation_.get(), rng);
    }
    auto compare_by_fitness = [](EncodedNAR const& a, EncodedNAR const& b) {
        return a.GetQualities().fitness > b.GetQualities().fitness;
    };
    std::sort(population.begin(), population.end(), compare_by_fitness);
    return population;
}

EncodedNAR DES::MutatedIndividual(std::vector<EncodedNAR> const& population, size_t at,
                                  RNG& rng) const {
    MutationFunction diff_func =
            EnumToMutationStrategy(differential_options_.differential_strategy);
    return (*diff_func)(population, at, differential_options_, rng);
}

unsigned long long DES::ExecuteInternal() {
    FeatureDomains feature_domains = FindFeatureDomains(typed_relation_.get());
    std::vector<EncodedNAR> population = GetRandomPopulationInDomains(feature_domains, rng_);

    for (unsigned i = 0; i < num_evaluations_; ++i) {
        size_t candidate_i = i % population_size_;
        EncodedNAR mutant = MutatedIndividual(population, candidate_i, rng_);
        NAR mutant_decoded = mutant.SetQualities(feature_domains, typed_relation_.get(), rng_);
        double candidate_fitness = population[candidate_i].GetQualities().fitness;

        if (mutant.GetQualities().fitness > candidate_fitness) {
            population[candidate_i] = std::move(mutant);
            nar_collection_.emplace_back(std::move(mutant_decoded));
        }
    }

    auto compare_by_fitness = [](const NAR& a, const NAR& b) -> bool {
        return a.GetQualities().fitness > b.GetQualities().fitness;
    };
    std::sort(nar_collection_.begin(), nar_collection_.end(), compare_by_fitness);
    return 0;
}

}  // namespace algos::des
