#pragma once

#include "algorithms/nar/value_range.h"
#include "model/table/column_layout_typed_relation_data.h"
#include "rng.h"
#include "model/types/numeric_type.h"

namespace algos::des {
namespace mo = model;

class EncodedValueRange {
public:
    static size_t const kFieldCount = 4;
    double permutation;
    double threshold;
    double bound1;
    double bound2;

    // TODO: remove code duplication here
    double& operator[](size_t index) {
        switch (index) {
            case 0:
                return permutation;
                break;
            case 1:
                return threshold;
                break;
            case 2:
                return bound1;
                break;
            case 3:
                return bound2;
                break;
            default:
                throw std::out_of_range("Index out of range for value range.");
        }
    }

    double const& operator[](size_t index) const {
        switch (index) {
            case 0:
                return permutation;
                break;
            case 1:
                return threshold;
                break;
            case 2:
                return bound1;
                break;
            case 3:
                return bound2;
                break;
            default:
                throw std::out_of_range("Index out of range for value range.");
        }
    }

    std::shared_ptr<mo::ValueRange> Decode(std::shared_ptr<mo::ValueRange> domain) const {
        switch (domain->GetTypeId()) {
            case mo::TypeId::kInt:
            case mo::TypeId::kDouble: {
                std::shared_ptr<mo::NumericalValueRange> dom =
                        std::static_pointer_cast<mo::NumericalValueRange>(domain);
                const std::byte * span = mo::INumericType::Sub(dom->upper_bound_, dom->lower_bound_);
                mo::Double resulting_lower = double_domain->lower_bound_ + span * this->bound1;
                mo::Double resulting_upper = double_domain->lower_bound_ + span * this->bound2;
                if (resulting_lower > resulting_upper) {
                    std::swap(resulting_lower, resulting_upper);
                }
                return std::make_shared<mo::NumericalValueRange>(
                        mo::NumericalValueRange(resulting_lower, resulting_upper));
            }
            case mo::TypeId::kString: {
                std::shared_ptr<mo::CategoricalValueRange> string_domain =
                        std::static_pointer_cast<mo::CategoricalValueRange>(domain);
                std::vector<mo::String> string_vector = string_domain->domain_;
                size_t span = string_vector.size();
                // upper_bound is not used, resulting NARs bind categorical values with a single
                // value.
                mo::String result;
                if (bound1 == 1.0) {
                    result = string_vector.back();
                } else {
                    result = string_vector[(size_t)(span * this->bound1)];
                }
                return std::make_shared<mo::CategoricalValueRange>(
                        mo::CategoricalValueRange(result));
            }
            default:
                throw std::invalid_argument(
                        std::string("ValueRange has invalid type_id in function: ") + __func__);
        }
    }

    EncodedValueRange() {
        permutation = RNG().Next();
        threshold = RNG().Next();
        bound1 = RNG().Next();
        bound2 = RNG().Next();
    }
};

}  // namespace algos::des
