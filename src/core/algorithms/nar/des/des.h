#pragma once

#include "encoded_nar.h"
#include "config/names.h"
#include "algorithms/nar/nar_algorithm.h"
#include "enums.h"
#include "differential_functions.h"

namespace algos::des {
    using FeatureDomains = const std::vector<std::shared_ptr<model::ValueRange>>;
    using TypedRelation = model::ColumnLayoutTypedRelationData; 

class DES : public NARAlgorithm {
private:
    int population_size_;
    int num_evaluations_;
    DifferentialOptions differential_options_;

    void RegisterOptions();

    static FeatureDomains FindFeatureDomains(TypedRelation const* typed_relation);
    std::vector<EncodedNAR> GetRandomPopulationInDomains(FeatureDomains domains) const;
    void EvolvePopulation(std::vector<EncodedNAR>& population) const;
    EncodedNAR MutateIndividual(std::vector<EncodedNAR> population, EncodedNAR best_individual, size_t at);

protected:
    void MakeExecuteOptsAvailable() override;
    unsigned long long ExecuteInternal() override;

public:
    DES();
};

} // namespace algos
