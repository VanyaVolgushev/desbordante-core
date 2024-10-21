#pragma once

#include "encoded_nar.h"
#include "config/names.h"
#include "algorithms/nar/nar_algorithm.h"
#include "enums.h"

namespace algos::des {
    using FeatureDomains = const std::vector<std::shared_ptr<model::ValueRange>>;
    using TypedRelation = model::ColumnLayoutTypedRelationData; 

class DES : public NARAlgorithm {
private:
    int population_size_;
    int num_evaluations_;
    double differential_scale_;
    double crossover_probability_;
    DifferentialStrategy differential_strategy_ = DifferentialStrategy::rand1Exp;

    void RegisterOptions();
    void Test();

    static const FeatureDomains FindFeatureDomains(TypedRelation const* typed_relation);
    std::vector<EncodedNAR> GetRandomPopulationInDomains(FeatureDomains domains) const;
    void EvolvePopulation(std::vector<EncodedNAR>& population);

protected:
    void MakeExecuteOptsAvailable() override;
    unsigned long long ExecuteInternal() override;

public:
    DES();
};

} // namespace algos
