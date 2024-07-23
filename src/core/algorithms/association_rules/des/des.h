#pragma once

#include <list>
#include <queue>
#include <stack>
#include <vector>

#include "algorithms/association_rules/ar_algorithm.h"

namespace algos {

class DES;
typedef void (DES::*StrategyFunction)(int);

class DES : public ARAlgorithm {

public:
    DES();
    unsigned long long GenerateAllRules() override;
    void ResetState() final;
    unsigned long long ExecuteInternal() override;
private:
    void RegisterOptions();
};

}  // namespace algos
