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
                                           algos::des::DifferentialStrategy diffStrategy) {
        using namespace config::names;
        return {{kCsvConfig,             csv_config}, {kMinimumSupport, minsup},
                {kMinimumConfidence,     minconf},    {kPopulationSize, popSize},
                {kMaxFitnessEvaluations, evalNum},    {kCrossoverProbability, crossProb},
                {kDifferentialScale,     diffScale},  {kDifferentialStrategy, diffStrategy}};
    }

    template <typename... Args>
    static std::unique_ptr<algos::des::DES> CreateAlgorithmInstance(Args&&... args) {
        return algos::CreateAndLoadAlgorithm<algos::des::DES>(
                GetParamMap(std::forward<Args>(args)...));
    }
};

TEST_F(DESTest, LaunchTest) {


    auto algorithm = CreateAlgorithmInstance(kSimpleUltimate, 0.5, 0.6, 100,
                                                             200, 0.9, 0.5,
                                                             algos::des::DifferentialStrategy::rand1Bin);
    auto testNAR = model::NAR();

    // Abalone.csv
    //std::shared_ptr<model::ValueRange> range5 = std::make_shared<model::DoubleValueRange>(model::DoubleValueRange(0.6385, 1.4880));
    //
    //std::shared_ptr<model::ValueRange> range1 = std::make_shared<model::DoubleValueRange>(model::DoubleValueRange(0.0750, 0.8150));
    //std::shared_ptr<model::ValueRange> range0 = std::make_shared<model::StringValueRange>(model::StringValueRange("F"));
    //
    //testNAR.InsertInAnte(5, range5);
    //testNAR.InsertInCons(1, range1);
    //testNAR.InsertInCons(0, range0);
    //testNAR.SetQualities(algorithm->GetTypedRelation());
    //auto quali = testNAR.GetQualities();
    
    std::shared_ptr<model::ValueRange> range5 = std::make_shared<model::IntValueRange>(model::IntValueRange(0,1));
    std::shared_ptr<model::ValueRange> range4 = std::make_shared<model::DoubleValueRange>(model::DoubleValueRange(1.7511,3.5816));
    std::shared_ptr<model::ValueRange> range6 = std::make_shared<model::IntValueRange>(model::IntValueRange(3,4));
    
    std::shared_ptr<model::ValueRange> range9 = std::make_shared<model::IntValueRange>(model::IntValueRange(176, 181));
    std::shared_ptr<model::ValueRange> range0 = std::make_shared<model::StringValueRange>(model::StringValueRange("Boris"));
    
    //testNAR.InsertInAnte(3, range3);
    testNAR.InsertInCons(9, range9);
    testNAR.InsertInCons(0, range0);
    testNAR.SetQualities(algorithm->GetTypedRelation());
    auto quali = testNAR.GetQualities();

    algorithm->Execute();
    size_t counter = 0;
    for (model::NAR nar: algorithm->GetNARVector()) {
        counter++;
        std::cout << nar.ToString() << "\n";
        if(counter > 100) {
            break;
        }
    }
    SUCCEED();
}

}  // namespace tests
