#include "ar_algorithm.h"

#include <algorithm>
#include <cassert>


#include "config/names_and_descriptions.h"
#include "config/option_using.h"
#include "config/tabular_data/input_table/option.h"

namespace algos {

ARAlgorithm::ARAlgorithm(std::vector<std::string_view> phase_names)
    : Algorithm(std::move(phase_names)) {
    using namespace config::names;
    RegisterOptions();
    MakeOptionsAvailable({kTable, kInputFormat});
}

void ARAlgorithm::RegisterOptions() {
    DESBORDANTE_OPTION_USING;

    auto sing_eq = [](InputFormat input_format) { return input_format == +InputFormat::singular; };
    auto tab_eq = [](InputFormat input_format) { return input_format == +InputFormat::tabular; };

    RegisterOption(config::kTableOpt(&input_table_));
    RegisterOption(Option{&first_column_tid_, kFirstColumnTId, kDFirstColumnTId, false});
    RegisterOption(Option{&item_column_index_, kItemColumnIndex, kDItemColumnIndex, 1u});
    RegisterOption(Option{&minconf_, kMinimumConfidence, kDMinimumConfidence, 0.0});
    RegisterOption(Option{&minsup_, kMinimumSupport, kDMinimumSupport, 0.0});
    RegisterOption(Option{&tid_column_index_, kTIdColumnIndex, kDTIdColumnIndex, 0u});
    RegisterOption(Option{&input_format_, kInputFormat, kDInputFormat}.SetConditionalOpts(
            {{sing_eq, {kTIdColumnIndex, kItemColumnIndex}}, {tab_eq, {kFirstColumnTId}}}));
}

void ARAlgorithm::MakeExecuteOptsAvailable() {
    using namespace config::names;
    MakeOptionsAvailable({kMinimumSupport, kMinimumConfidence});
}

void ARAlgorithm::LoadDataInternal() {
    switch (input_format_) {
        case InputFormat::singular:
            transactional_data_ = model::TransactionalData::CreateFromSingular(
                    *input_table_, tid_column_index_, item_column_index_);
            break;
        case InputFormat::tabular:
            transactional_data_ =
                    model::TransactionalData::CreateFromTabular(*input_table_, first_column_tid_);
            break;
        default:
            assert(0);
    }
    if (transactional_data_->GetNumTransactions() == 0) {
        throw std::runtime_error("Got an empty dataset: AR mining is meaningless.");
    }
}

std::list<model::ARStrings> ARAlgorithm::GetArStringsList() const {
    std::list<model::ARStrings> ar_strings;
    for (auto const& ar : ar_collection_) {
        ar_strings.emplace_back(ar, transactional_data_.get());
    }
    return ar_strings;
}

}  // namespace algos
