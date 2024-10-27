#pragma once

#include <vector>
#include "value_range.h"
#include "model/types/types.h"
#include "model/table/column_layout_typed_relation_data.h"

namespace model {

struct NARQualities {
    double fitness = -1.0;
    double support = -1.0;
    double confidence = -1.0;
};

class NAR {
    using TypedRelation = model::ColumnLayoutTypedRelationData; 
public:
    std::string ToString() const;
    void SetQualities(TypedRelation const* typed_relation);
    const model::NARQualities& GetQualities() const;
    void InsertInAnte(size_t feature_index, std::shared_ptr<ValueRange> range);
    void InsertInCons(size_t feature_index, std::shared_ptr<ValueRange> range);


private:
    NARQualities qualities_;
    bool qualities_consistent_ = false;

    std::map<size_t, std::shared_ptr<ValueRange>> ante_ = std::map<size_t, std::shared_ptr<ValueRange>>();
    std::map<size_t, std::shared_ptr<ValueRange>> cons_ = std::map<size_t, std::shared_ptr<ValueRange>>();

    bool AnteFitsValue(size_t feature_index, const std::byte* value_of_feature) const {
        return MapFitsValue(ante_, feature_index, value_of_feature);
    }

    bool ConsFitsValue(size_t feature_index, const std::byte* value_of_feature) const {
        return MapFitsValue(cons_, feature_index, value_of_feature);
    }

    //TODO: name std::map<size_t, std::shared_ptr<ValueRange>> something
    //std::tuple<bool, bool> 
    static bool MapFitsValue(std::map<size_t, std::shared_ptr<ValueRange>> map, size_t feature_index, const std::byte* value);
};

} // namespace model
