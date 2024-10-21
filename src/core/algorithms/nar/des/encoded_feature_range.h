#pragma once

#include "algorithms/nar/value_range.h"
#include "model/table/column_layout_typed_relation_data.h"
#include "rng.h"

namespace algos::des {

class EncodedFeatureRange { //TODO: rename to encoded value range
public:
    double permutation;
    double threshold;
    double lower_bound;
    double upper_bound;
    EncodedFeatureRange() {
        permutation = RNG().Next();
        threshold = RNG().Next();
        lower_bound = RNG().Next();
        upper_bound = RNG().Next();
    }
    std::shared_ptr<model::ValueRange> Decode(std::shared_ptr<model::ValueRange> domain) const {
        switch (domain->GetTypeId()) {
        case model::TypeId::kInt: {
            std::shared_ptr<model::IntValueRange> int_domain = std::static_pointer_cast<model::IntValueRange>(domain);
            model::Int span = int_domain->upper_bound_ - int_domain->lower_bound_;
            model::Int resulting_lower = int_domain->lower_bound_ + span * this->lower_bound;
            model::Int resulting_upper = int_domain->lower_bound_ + span * this->upper_bound;
            return std::make_shared<model::IntValueRange>(model::IntValueRange(resulting_lower, resulting_upper));
        }
        case model::TypeId::kDouble: {
            std::shared_ptr<model::DoubleValueRange> double_domain = std::static_pointer_cast<model::DoubleValueRange>(domain);
            model::Double span = double_domain->upper_bound_ - double_domain->lower_bound_;
            model::Double resulting_lower = double_domain->lower_bound_ + span * this->lower_bound;
            model::Double resulting_upper = double_domain->lower_bound_ + span * this->upper_bound;
            return std::make_shared<model::DoubleValueRange>(model::DoubleValueRange(resulting_lower, resulting_upper));
        }
        case model::TypeId::kString:
        case model::TypeId::kMixed: {
            std::shared_ptr<model::StringValueRange> string_domain = std::static_pointer_cast<model::StringValueRange>(domain);
            std::vector<model::String> string_vector = string_domain->domain_;
            size_t span = string_vector.size();
            // upper_bound is not used, resulting NARs bind categorical values with a single value.
            model::String result = string_vector[(size_t)(span * this->lower_bound)];
            return std::make_shared<model::StringValueRange>(model::StringValueRange(result));
        }
        default:
            throw std::invalid_argument(std::string("ValueRange has invalid type_id in function: ") + __func__);
        }
    }
};

} // namespace algos::des
