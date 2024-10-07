#pragma once

#include "config/names.h"
#include "algorithms/nar/nar_algorithm.h"
#include "enums.h"

namespace algos::des {

class DES : public NARAlgorithm {
private:
    int population_;
    int num_evaluations_;
    double differential_scale_;
    double crossover_probability_;
    DifferentialStrategy differential_strategy_ = DifferentialStrategy::rand1Exp;

    void RegisterOptions();
    void Test();

    std::vector<std::unique_ptr<const FeatureBounds>> FindFeatureDomains() const;
    size_t GetEncodedVecLength (std::vector<std::unique_ptr<const FeatureBounds>> feature_domains) const;
    
    unsigned long long GenerateAllNARs();

protected:
    void MakeExecuteOptsAvailable() override;

public:
    unsigned long long ExecuteInternal() override;
    DES();
};

} // namespace algos
