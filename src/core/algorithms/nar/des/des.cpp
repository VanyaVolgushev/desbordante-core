#include "des.h"

#include "config/names_and_descriptions.h"
#include "config/option_using.h"
#include "config/tabular_data/input_table/option.h"
#include "algorithms/nar/feature_bounds.h"
#include "model/types/types.h"

namespace algos::des {
using model::FeatureBounds;
DES::DES() : NARAlgorithm({}) {
    using namespace config::names;
    RegisterOptions();
}

void DES::RegisterOptions() {
    DESBORDANTE_OPTION_USING;

    DifferentialStrategy default_strategy = DifferentialStrategy::rand1Exp;
    RegisterOption(Option{&population_size_, kPopulationSize, kDPopulationSize, 100});
    RegisterOption(Option{&num_evaluations_, kMaxFitnessEvaluations, kDMaxFitnessEvaluations, 1000});
    RegisterOption(Option{&differential_scale_, kDifferentialScale, kDDifferentialScale, 0.5});
    RegisterOption(Option{&crossover_probability_, kCrossoverProbability, kDCrossoverProbability, 0.9});
    RegisterOption(Option{&differential_strategy_, kDifferentialStrategy, kDDifferentialStrategy, default_strategy});
}

void DES::MakeExecuteOptsAvailable() {
    using namespace config::names;
    MakeOptionsAvailable({kPopulationSize, kMaxFitnessEvaluations, kDifferentialScale,
                         kCrossoverProbability, kDifferentialStrategy});
}

unsigned long long DES::GenerateAllNARs() {
    //TODO
    return 0;
}

void DES::Test() {
    for (uint columnIndex = 0; columnIndex < typed_relation_->GetNumColumns(); ++columnIndex) {
        size_t num_rows = typed_relation_->GetColumnData(columnIndex).GetNumRows();
        size_t row_index = (columnIndex + 14) % num_rows;
        model::TypedColumnData const& column = typed_relation_->GetColumnData(columnIndex);
        const std::byte* value;

        if(!column.IsNullOrEmpty(row_index))
        {
            value = column.GetValue(row_index);
        }
        int integer_value = model::Type::GetValue<int>(value);
        std::cout << "column " << columnIndex << " " << "\n";
        std::cout << "type: " << column.GetType().ToString();
        std::cout << "example of data at " << row_index << ": ";
        std::cout <<  integer_value;
        std::cout << "\n";
    }

    std::cout << "trying to create a CategoricalAttribute\n";
    size_t string_column_num = 0;
    size_t double_column_num = 1;
    size_t integer_column_num = 2;
    model::TypedColumnData const& string_column = typed_relation_->GetColumnData(string_column_num);
    model::TypedColumnData const& double_column = typed_relation_->GetColumnData(double_column_num);
    model::TypedColumnData const& int_column = typed_relation_->GetColumnData(integer_column_num);

    using namespace model;


    //std::cout << "\ntype operations:";
    //model::DoubleType double_type;
    //model::IntType int_type;
    //std::byte const* value1 = double_type.MakeValue(-3.141);
    //std::byte const* value2 = int_type.MakeValue(-3);
//
    //std::byte* Artyom = double_type.Allocate();
    //double_type.Max(value1, value2, Artyom);
    //std::cout << "\ncouting Artyom:";
    //double_type.Free(Artyom);
}

const FeatureDomains DES::FindFeatureDomains(TypedRelation const* typed_relation) {
    auto feature_domains = std::vector<std::shared_ptr<FeatureBounds>>();
    for (size_t i = 0; i < typed_relation->GetNumColumns(); i++) {
        std::shared_ptr<FeatureBounds> domain = CreateFeatureBounds(typed_relation->GetColumnData(i), i);
        feature_domains.emplace_back(domain);
    }
    return feature_domains;
}

std::vector<EncodedNAR> DES::GetRandomPopulationInDomains(FeatureDomains domains) const {
    auto encodedNARs = std::vector<EncodedNAR>();
    for (int i = 0; i < population_size_; i++) {
        encodedNARs.emplace_back(EncodedNAR(domains, typed_relation_.get()));
    }
    auto CompareByFitness  = [](const EncodedNAR& a, const EncodedNAR& b) -> bool {
        return a.fitness_ > b.fitness_;
    };
    std::sort(encodedNARs.begin(), encodedNARs.end(), CompareByFitness);
    return encodedNARs;
}

void DES::EvolvePopulation(std::vector<EncodedNAR>& population) {
    //TODO
}

unsigned long long DES::ExecuteInternal() {
    FeatureDomains feature_domains = FindFeatureDomains(typed_relation_.get());
    std::vector<EncodedNAR> encodedNARs = GetRandomPopulationInDomains(feature_domains);
    for (size_t i = 0; i < encodedNARs.size(); i++) {
        encodedNARs[i].Evaluate(feature_domains,typed_relation_.get());
    }
    //DEBUG
    //for(EncodedNAR enc: encodedNARs) {
    //    std::cout << "NAR\n";
    //    for(std::shared_ptr<EncodedFeatureBounds> bounds: enc.encoded_feature_bounds_vec_)
    //    {
    //        switch (bounds->GetTypeId())
    //        {
    //        case FeatureTypeId::kCategorical:
    //        std::shared_ptr<EncodedCategoricalFeatureBounds> bndcat = bnd;
    //            std::cout << ((std::shared_ptr<EncodedCategoricalFeatureBounds>)bnd)->value_;
    //            break;
    //        
    //        default:
    //            break;
    //        }
    //    }
    //    std::cout
    //}
    ///DEBUG
    for (int i = 0; i < num_evaluations_; i++) { //TODO: change num_evaluations type to something unsigned
        EvolvePopulation(encodedNARs);
    }
    for (size_t i = 0; i < encodedNARs.size(); i++) {
        nar_collection_.emplace_back(encodedNARs[i].Decode(feature_domains));
    }


    Test();
    return 0;
}

} // namespace algos
