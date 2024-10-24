#include "des.h"

#include "config/names_and_descriptions.h"
#include "config/option_using.h"
#include "config/tabular_data/input_table/option.h"
#include "algorithms/nar/value_range.h"
#include "model/types/types.h"

namespace algos::des {
using model::ValueRange;
DES::DES() : NARAlgorithm({}) {
    using namespace config::names;
    RegisterOptions();
}

void DES::RegisterOptions() {
    DESBORDANTE_OPTION_USING;

    DifferentialStrategy default_strategy = DifferentialStrategy::rand1Bin;
    RegisterOption(Option{&population_size_, kPopulationSize, kDPopulationSize, 100});
    RegisterOption(Option{&num_evaluations_, kMaxFitnessEvaluations, kDMaxFitnessEvaluations, 1000});
    RegisterOption(Option{&differential_options_.differential_scale, kDifferentialScale, kDDifferentialScale, 0.5});
    RegisterOption(Option{&differential_options_.crossover_probability, kCrossoverProbability, kDCrossoverProbability, 0.9});
    RegisterOption(Option{&differential_options_.differential_strategy, kDifferentialStrategy, kDDifferentialStrategy, default_strategy});
}

void DES::MakeExecuteOptsAvailable() {
    using namespace config::names;
    MakeOptionsAvailable({kPopulationSize, kMaxFitnessEvaluations, kDifferentialScale,
                         kCrossoverProbability, kDifferentialStrategy});
}

FeatureDomains DES::FindFeatureDomains(TypedRelation const* typed_relation) {
    auto feature_domains = std::vector<std::shared_ptr<ValueRange>>();
    for (size_t i = 0; i < typed_relation->GetNumColumns(); i++) {
        std::shared_ptr<ValueRange> domain = CreateValueRange(typed_relation->GetColumnData(i));
        feature_domains.emplace_back(domain);
    }
    return feature_domains;
}

std::vector<EncodedNAR> DES::GetRandomPopulationInDomains(FeatureDomains domains) const {
    auto encodedNARs = std::vector<EncodedNAR>();
    for (int i = 0; i < population_size_; i++) {
        encodedNARs.emplace_back(EncodedNAR(domains, typed_relation_.get()));
    }
    auto CompareByFitness  = [](const EncodedNAR& a, const EncodedNAR& b) -> bool {
        return a.qualities.fitness > b.qualities.fitness;
    };
    std::sort(encodedNARs.begin(), encodedNARs.end(), CompareByFitness);
    return encodedNARs;
}

EncodedNAR DES::MutateIndividual(std::vector<EncodedNAR> population, EncodedNAR best_individual, size_t at) {
    MutationFunction diff_func = EnumToMutationStrategy(differential_options_.differential_strategy);
    return (*diff_func)(population, best_individual, at, differential_options_);
}

unsigned long long DES::ExecuteInternal() {
    FeatureDomains feature_domains = FindFeatureDomains(typed_relation_.get());
    std::vector<EncodedNAR> population = GetRandomPopulationInDomains(feature_domains);
    EncodedNAR best_individual = EncodedNAR(feature_domains, typed_relation_.get());
    
    for (int i = 0; i < num_evaluations_; i++) { //TODO: change num_evaluations type to something unsigned
        EncodedNAR mutant = MutateIndividual(population, best_individual, i % population_size_);
        mutant.SetQualities(feature_domains, typed_relation_.get());
        if(mutant.qualities.fitness > population[i % population_size_].qualities.fitness) {
            population[i % population_size_] = mutant;
        }
        if(mutant.qualities.fitness > best_individual.qualities.fitness) {
            best_individual = mutant;
        }
    }
    
    for (size_t i = 0; i < population.size(); i++) {
        NAR decoded = population[i].Decode(feature_domains);
        decoded.SetQualities(typed_relation_.get());
        nar_collection_.emplace_back(decoded);
    }
    auto CompareByFitness  = [](const NAR& a, const NAR& b) -> bool {
        return a.qualities.fitness > b.qualities.fitness;
    };
    std::sort(nar_collection_.begin(), nar_collection_.end(), CompareByFitness);
    return 0;
}

} // namespace algos
