#pragma once

#include "algorithms/algorithm.h"
#include "model/table/column_layout_typed_relation_data.h"
#include "config/tabular_data/input_table_type.h"


namespace algos {

class NARAlgorithm : public Algorithm {
private:
    using TypedRelation = model::ColumnLayoutTypedRelationData;

    config::InputTable input_table_;
    std::unique_ptr<TypedRelation> typed_relation_;

    void RegisterOptions();

protected:
    double minsup_;
    double minconf_;

    virtual unsigned long long GenerateAllNARs() = 0;
    void LoadDataInternal() final;
    void MakeExecuteOptsAvailable() override;

public:
    explicit NARAlgorithm(std::vector<std::string_view> phase_names);
    void ResetState() override;

    virtual ~NARAlgorithm() = default;
};

} // namespace algos