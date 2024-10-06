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
    //std::list<model::NarIDs> nar_collection_;

    //std::list<model::ArIDs> const& GetArIDsList() const noexcept {
    //    return nar_collection_;
    //};
    void RegisterOptions();
    void Test();

protected:
    unsigned long long GenerateAllNARs() override;
    void MakeExecuteOptsAvailable() override;

public:
    unsigned long long ExecuteInternal() override;
    DES();
};

} // namespace algos