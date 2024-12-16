#include <gtest/gtest.h>

#include "algorithms/algo_factory.h"
#include "algorithms/nar/des/des.h"
#include "all_csv_configs.h"
#include "config/names.h"

namespace tests {

class DESTest : public ::testing::Test {
protected:
    static algos::StdParamsMap GetParamMap(CSVConfig const& csv_config, double minsup,
                                           double minconf, unsigned int popSize,
                                           unsigned int evalNum, double crossProb, double diffScale,
                                           algos::des::DifferentialStrategy diffStrategy) {
        using namespace config::names;
        return {{kCsvConfig, csv_config},          {kMinimumSupport, minsup},
                {kMinimumConfidence, minconf},     {kPopulationSize, popSize},
                {kMaxFitnessEvaluations, evalNum}, {kCrossoverProbability, crossProb},
                {kDifferentialScale, diffScale},   {kDifferentialStrategy, diffStrategy}};
    }

    template <typename... Args>
    static std::unique_ptr<algos::des::DES> CreateAlgorithmInstance(Args&&... args) {
        return algos::CreateAndLoadAlgorithm<algos::des::DES>(
                GetParamMap(std::forward<Args>(args)...));
    }
};

TEST_F(DESTest, LaunchTest) {
    auto algorithm = CreateAlgorithmInstance(kAbalone, 0.5, 0.6, 100u, 100u, 0.9, 0.5,
                                             algos::des::DifferentialStrategy::rand1Bin);
    algorithm->Execute();
    std::vector<std::string> result;
    for (auto i : algorithm->GetNARVector()) {
        result.push_back(std::to_string(i.GetQualities().fitness));
    }
    std::vector<std::string> expected = {"0.566887", "0.549872", "0.518678", "0.518598", "0.513126",
                                         "0.481561", "0.478260", "0.462123", "0.386524", "0.333413",
                                         "0.305220", "0.301171", "0.293404", "0.274753", "0.250303",
                                         "0.238877", "0.224897", "0.215952", "0.208631", "0.187335",
                                         "0.140159", "0.131046", "0.112096"};
    ASSERT_EQ(result, expected);
}

}  // namespace tests
