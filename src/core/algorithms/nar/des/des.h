#pragma once

#include "config/names.h"
#include "algorithms/nar/nar_algorithm.h"
#include "enums.h"

namespace algos {

class DES : public NARAlgorithm {

private:
    int population_;
    int num_evaluations_;
    double differential_scale_;
    double crossover_probability_;
    DifferentialStrategy differential_strategy_ = DifferentialStrategy::rand1Exp;

    void RegisterOptions();

protected:
    unsigned long long GenerateAllNARs() override;
    void MakeExecuteOptsAvailable() override;

public:
    unsigned long long ExecuteInternal() override;
    DES();
};

} // namespace algos