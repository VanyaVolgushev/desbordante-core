#include "nar_algorithm.h"

#include "config/names_and_descriptions.h"
#include "config/option_using.h"
#include "config/tabular_data/input_table/option.h"

namespace algos {

NARAlgorithm::NARAlgorithm(std::vector<std::string_view> phase_names)
    : Algorithm(std::move(phase_names))  {
    using namespace config::names;
    RegisterOptions();
    algos::Algorithm::MakeOptionsAvailable({kTable});
}

void NARAlgorithm::LoadDataInternal() {
    typed_relation_ = model::ColumnLayoutTypedRelationData::CreateFrom(*input_table_,
                                                                       true);
}

void NARAlgorithm::ResetState() {
    //everything = 0 and delete
}

void NARAlgorithm::RegisterOptions() {
    DESBORDANTE_OPTION_USING;

    RegisterOption(config::kTableOpt(&input_table_));
    RegisterOption(Option{&minconf_, kMinimumConfidence, kDMinimumConfidence, 0.0});
    RegisterOption(Option{&minsup_, kMinimumSupport, kDMinimumSupport, 0.0});
}

void NARAlgorithm::MakeExecuteOptsAvailable() {
    using namespace config::names;
    MakeOptionsAvailable({kMinimumSupport, kMinimumConfidence});
}

} // namespace algos