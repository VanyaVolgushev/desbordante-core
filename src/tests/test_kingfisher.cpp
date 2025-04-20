#include <gtest/gtest.h>

#include "algorithms/algo_factory.h"
#include "algorithms/near/kingfisher/kingfisher.h"
#include "algorithms/near/near.h"
#include "algorithms/near/near_discovery.h"
#include "all_csv_configs.h"
#include "csv_config_util.h"

namespace tests {
template<typename T>
std::string VectorToString(std::vector<T> const& vec) {
    std::ostringstream oss;
    oss << "[";
    for (size_t i = 0; i < vec.size(); ++i) {
        oss << vec[i].ToString();  // Convert FeatureIndex to string
        if (i < vec.size() - 1) {
            oss << ", ";
        }
    }
    oss << "]";
    return oss.str();
}

class NeARAlgorithmTest : public ::testing::Test {
protected:
    static algos::StdParamsMap GetParamMap(CSVConfig const& csv_config, double maxP,
                                           unsigned int tidColumnIndex,
                                           unsigned int itemColumnIndex) {
        using namespace config::names;
        return {{kCsvConfig, csv_config},
                {kInputFormat, +algos::InputFormat::singular},
                {kMaxPValue, maxP},
                {kTIdColumnIndex, tidColumnIndex},
                {kItemColumnIndex, itemColumnIndex}};
    }

    static algos::StdParamsMap GetParamMap(CSVConfig const& csv_config, double maxP,
                                           bool firstColumnTid) {
        using namespace config::names;
        return {{kCsvConfig, csv_config},
                {kInputFormat, +algos::InputFormat::tabular},
                {kMaxPValue, maxP},
                {kFirstColumnTId, firstColumnTid}};
    }

    template <typename... Args>
    static std::unique_ptr<algos::NeARDiscovery> CreateAlgorithmInstance(Args&&... args) {
        return algos::CreateAndLoadAlgorithm<algos::Kingfisher>(
                GetParamMap(std::forward<Args>(args)...));
    }
};

TEST_F(NeARAlgorithmTest, PaperExampleDataset) {
    auto algorithm = CreateAlgorithmInstance(kTestNeAR1, 1.2e-8, false);
    algorithm->Execute();
    auto const rules = algorithm->GetNeARStringsVector();
    std::cout << VectorToString(rules);
}

}  // namespace tests
