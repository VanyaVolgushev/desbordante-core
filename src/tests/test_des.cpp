#include "algorithms/algo_factory.h"

#include "algorithms/nar/des/des.h"
#include <gtest/gtest.h>
#include "all_csv_configs.h"
#include "config/names.h"

namespace tests {

class DESTest : public ::testing::Test {
protected:
    static algos::StdParamsMap GetParamMap(CSVConfig const& csv_config, double minsup,
                                           double minconf, int popSize, int evalNum,
                                           double crossProb, double diffScale,
                                           algos::DifferentialStrategy diffStrategy) {
        using namespace config::names;
        return {{kCsvConfig,             csv_config}, {kMinimumSupport, minsup},
                {kMinimumConfidence,     minconf},    {kPopulationSize, popSize},
                {kMaxFitnessEvaluations, evalNum},    {kCrossoverProbability, crossProb},
                {kDifferentialScale,     diffScale},  {kDifferentialStrategy, diffStrategy}};
    }

    template <typename... Args>
    static std::unique_ptr<algos::DES> CreateAlgorithmInstance(Args&&... args) {
        return algos::CreateAndLoadAlgorithm<algos::DES>(
                GetParamMap(std::forward<Args>(args)...));
    }
};

TEST_F(DESTest, LaunchTest) {
    auto algorithm = CreateAlgorithmInstance(kAbalone, 0.5, 0.6, 100,
                                                             1000, 0.9, 0.8,
                                                             algos::DifferentialStrategy::best1Exp);
    algorithm->Execute();
    SUCCEED();
}

}  // namespace tests
