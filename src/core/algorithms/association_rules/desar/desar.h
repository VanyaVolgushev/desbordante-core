#pragma once

#include <list>
#include <queue>
#include <stack>
#include <vector>

#include "algorithms/association_rules/ar_algorithm.h"

namespace algos {

class DESAR;
typedef void (DESAR::*StrategyFunction)(int);

class DESAR : public ARAlgorithm {

public:
    DESAR();
    unsigned long long GenerateAllRules() override;
    void ResetState() final;
    unsigned long long ExecuteInternal() override;
private:
    void RegisterOptions();
};

}  // namespace algos
