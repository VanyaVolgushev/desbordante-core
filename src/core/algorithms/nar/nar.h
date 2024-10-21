#pragma once

#include <vector>
#include "value_range.h"
#include "model/types/types.h"
#include "model/table/column_layout_typed_relation_data.h"

namespace model {

struct NARQualities {
    double fitness;
    double support;
    double confidence;
};

class NAR {
public:
    using TypedRelation = model::ColumnLayoutTypedRelationData; 
    NARQualities qualities;

    std::map<size_t, std::shared_ptr<ValueRange>> ante = std::map<size_t, std::shared_ptr<ValueRange>>();
    std::map<size_t, std::shared_ptr<ValueRange>> cons = std::map<size_t, std::shared_ptr<ValueRange>>();

    bool IncludesInAnte(size_t feature_index, const std::byte* value) const {
        return MapIncludes(ante, feature_index, value);
    }

    bool IncludesInCons(size_t feature_index, const std::byte* value) const {
        return MapIncludes(cons, feature_index, value);
    }

    std::string ToString() const;
    NARQualities SetQualities(TypedRelation const* typed_relation);
private:
    //TODO: name std::map<size_t, std::shared_ptr<ValueRange>> something
    static bool MapIncludes(std::map<size_t, std::shared_ptr<ValueRange>> map, size_t feature_index, const std::byte* value);
};

} // namespace model
