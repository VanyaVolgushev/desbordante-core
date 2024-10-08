#include "des.h"

#include "config/names_and_descriptions.h"
#include "config/option_using.h"
#include "config/tabular_data/input_table/option.h"
#include "algorithms/nar/create_feature_bounds.h"

namespace algos::des {

    using FeatureDomain = std::shared_ptr<const FeatureBounds>;

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
    std::shared_ptr<FeatureBounds> string_feat = CreateFeatureBounds(string_column);
    std::shared_ptr<FeatureBounds> double_feat = CreateFeatureBounds(double_column);
    std::shared_ptr<FeatureBounds> int_feat = CreateFeatureBounds(int_column);
    std::cout << double_feat->GetTypeId();

    std::cout << "trying to create an EncodedNARGenerator\n";
    auto ngen = EncodedNARGenerator(FindFeatureDomains(typed_relation_.get()));
    //ngen.GenerateUniformRandom();
}


std::vector<FeatureDomain> DES::FindFeatureDomains(TypedRelation const* typed_relation) {
    return std::vector<FeatureDomain>();
}

unsigned long long DES::ExecuteInternal() {
    Test();
    std::vector<FeatureDomain> FeatureDomains = FindFeatureDomains(typed_relation_.get());
    

    return 0;
}

} // namespace algos
