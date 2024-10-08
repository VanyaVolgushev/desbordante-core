#pragma once

#include "config/names.h"
#include "algorithms/nar/nar_algorithm.h"
#include "encoded_nar_generator.h"
#include "enums.h"

namespace algos::des {
    using FeatureDomain = std::shared_ptr<const FeatureBounds>;
    using TypedRelation = model::ColumnLayoutTypedRelationData; 

class DES : public NARAlgorithm {
private:
    int population_size_;
    int num_evaluations_;
    double differential_scale_;
    double crossover_probability_;
    DifferentialStrategy differential_strategy_ = DifferentialStrategy::rand1Exp;

    std::vector<EncodedNAR> population_;

    void RegisterOptions();
    void Test();

    static std::vector<FeatureDomain> FindFeatureDomains(TypedRelation const* typed_relation);
    static EncodedNAR GetEncodedVecLength (std::vector<std::unique_ptr<const FeatureBounds>> feature_domains);
    
    unsigned long long GenerateAllNARs();

protected:
    void MakeExecuteOptsAvailable() override;

public:
    unsigned long long ExecuteInternal() override;
    DES();
};

} // namespace algos
