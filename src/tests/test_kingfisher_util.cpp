#include <memory>

#include <gtest/gtest.h>

#include "algorithms/near/kingfisher/candidate_prefix_tree.h"
#include "algorithms/near/near.h"
#include "all_csv_configs.h"
#include "csv_config_util.h"

namespace tests {

class TestNeARUtilTypes : public ::testing::Test {};

std::vector<FeatureIndex> feat_frequency_order = {FeatureIndex{3}, FeatureIndex{4}, FeatureIndex{2},
                                                  FeatureIndex{1}};

std::string VectorToString(std::vector<FeatureIndex> const& vec) {
    std::ostringstream oss;
    oss << "[";
    for (size_t i = 0; i < vec.size(); ++i) {
        oss << vec[i];  // Convert FeatureIndex to string
        if (i < vec.size() - 1) {
            oss << ", ";
        }
    }
    oss << "]";
    return oss.str();
}

double GetLowerBound1(OFeatureIndex feature) {
    std::cout << "Lower bound 1 for feat " + std::to_string(feat_frequency_order[feature]) + " is: ";
    //double bound;
    //std::cin >> bound;
    return 0;
}

double GetLowerBound2(algos::NodeAdress const& node_addr, OFeatureIndex cons_index,
                      bool cons_positive) {
                        // TODO: проверка на то что выражения под факториалами неотрицательны
    std::cout << "Lower bound 2 for " + VectorToString(node_addr.ToFeatures(feat_frequency_order)) +
                         " at " + std::to_string(feat_frequency_order[cons_index]) +
                         " with cons positive:" + std::to_string(cons_positive) + " is: ";
//
    //double bound;
    //std::cin >> bound;
    return 0;
}

double GetLowerBound3(algos::NodeAdress const& node_addr, OFeatureIndex cons_index,
                      bool cons_positive) {
    std::cout << "Lower bound 3 for " + VectorToString(node_addr.ToFeatures(feat_frequency_order)) +
                         " at " + std::to_string(feat_frequency_order[cons_index]) +
                         " with cons positive:" + std::to_string(cons_positive) + " is: ";
//
    //double bound;
    //std::cin >> bound;
    return 0;
}

double GetFishersP(model::NeARIDs const& rule) {
    std::cout << "Goodness measure for " + rule.ToString() + "is:";
    double bound;
    std::cin >> bound;
    return bound;
}

TEST(TestNeARUtilTypes, ExampleFromPaper) {
    using namespace algos;

    CandidatePrefixTree tree = CandidatePrefixTree(4, GetLowerBound1, GetLowerBound2,
                                                   GetLowerBound3, GetFishersP, 1.2e-8);
}

}  // namespace tests
