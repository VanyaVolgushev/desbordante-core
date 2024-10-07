#pragma once

#include "algorithms/algorithm.h"
#include "nar.h"
#include "model/table/column_layout_typed_relation_data.h"
#include "config/tabular_data/input_table_type.h"


namespace algos {

class NARAlgorithm : public Algorithm {
private:
    using TypedRelation = model::ColumnLayoutTypedRelationData;

    config::InputTable input_table_;
    void RegisterOptions();
    std::list<NAR> nar_collection_;

protected:
    std::unique_ptr<TypedRelation> typed_relation_;
    double minsup_;
    double minconf_;

    void LoadDataInternal() final;
    void MakeExecuteOptsAvailable() override;

public:

    //std::list<model::NarIDs> const& GetArIDsList() const noexcept {
    //    return nar_collection_;
    //};
    void ResetState() override;
    explicit NARAlgorithm(std::vector<std::string_view> phase_names);
};

} // namespace algos