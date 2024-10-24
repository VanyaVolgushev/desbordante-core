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
public:
    using TypedRelation = model::ColumnLayoutTypedRelationData; 
    NARQualities qualities;

    std::map<size_t, std::shared_ptr<ValueRange>> ante = std::map<size_t, std::shared_ptr<ValueRange>>();
    std::map<size_t, std::shared_ptr<ValueRange>> cons = std::map<size_t, std::shared_ptr<ValueRange>>();

    bool AnteFitsValue(size_t feature_index, const std::byte* value_of_feature) const {
        return MapFitsValue(ante, feature_index, value_of_feature);
    }

    bool ConsFitsValue(size_t feature_index, const std::byte* value_of_feature) const {
        return MapFitsValue(cons, feature_index, value_of_feature);
    }

    NARQualities SetQualities(TypedRelation const* typed_relation);
    std::string ToString() const;
private:
    //TODO: name std::map<size_t, std::shared_ptr<ValueRange>> something
    //std::tuple<bool, bool> 
    static bool MapFitsValue(std::map<size_t, std::shared_ptr<ValueRange>> map, size_t feature_index, const std::byte* value);
};

} // namespace model
