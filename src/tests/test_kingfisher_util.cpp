#include <memory>

#include <gtest/gtest.h>

#include "algorithms/near/kingfisher/candidate_prefix_tree.h"
#include "all_csv_configs.h"
#include "csv_config_util.h"

namespace tests {

class TestNeARUtilTypes : public ::testing::Test {};

TEST(TestNeARUtilTypes, TennisDataSet) {
    using namespace algos;
    CandidatePrefixTree tree = CandidatePrefixTree({FeatureIndex{3}, FeatureIndex{4}, FeatureIndex{2}, FeatureIndex{1}});
}

}  // namespace tests
