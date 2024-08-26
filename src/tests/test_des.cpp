#include <gtest/gtest.h>

#include "algorithms/algo_factory.h"
#include "algorithms/association_rules/des/des.h"
#include "all_csv_configs.h"
#include "config/names.h"

namespace tests {

class DESTest : public ::testing::Test {
protected:
    static algos::StdParamsMap GetParamMap(CSVConfig const& csv_config, double minsup,
                                           double minconf, unsigned int tidColumnIndex,
                                           unsigned int itemColumnIndex) {
        using namespace config::names;
        return {{kCsvConfig, csv_config},          {kInputFormat, +algos::InputFormat::singular},
                {kMinimumSupport, minsup},         {kMinimumConfidence, minconf},
                {kTIdColumnIndex, tidColumnIndex}, {kItemColumnIndex, itemColumnIndex}};
    }

    template <typename... Args>
    static std::unique_ptr<algos::DES> CreateAlgorithmInstance(Args&&... args) {
        return algos::CreateAndLoadAlgorithm<algos::DES>(
                GetParamMap(std::forward<Args>(args)...));
    }
};

TEST_F(DESTest, BookDataset) {
auto algorithm = CreateAlgorithmInstance(kRulesBook, 0.3, 0.5, 0, 1);
algorithm->Execute();
auto const actual_rules = algorithm->GetArStringsList();
SUCCEED();
}

}  // namespace tests
