#pragma once

#include <list>
#include <set>
#include <stack>
#include <vector>

#include <boost/any.hpp>

#include "algorithms/algorithm.h"
#include "ar.h"
#include "ar_algorithm_enums.h"
#include "config/tabular_data/input_table_type.h"
#include "model/transaction/transactional_data.h"

namespace algos {

class ARAlgorithm : public Algorithm {
private:
    config::InputTable input_table_;

    InputFormat input_format_ = InputFormat::singular;
    unsigned int tid_column_index_;
    unsigned int item_column_index_;
    bool first_column_tid_;

    void RegisterOptions();

protected:
    std::shared_ptr<model::TransactionalData> transactional_data_;
    double minsup_;
    std::list<model::ArIDs> ar_collection_;
    double minconf_;

    virtual unsigned long long GenerateAllRules() = 0;
    void LoadDataInternal() final;
    void MakeExecuteOptsAvailable() final;


public:
    explicit ARAlgorithm(std::vector<std::string_view> phase_names);

    std::list<model::ArIDs> const& GetArIDsList() const noexcept {
        return ar_collection_;
    };

    std::vector<std::string> const& GetItemNamesVector() const noexcept {
        return transactional_data_->GetItemUniverse();
    }

    std::list<model::ARStrings> GetArStringsList() const;

    virtual ~ARAlgorithm() = default;
};

}  // namespace algos
