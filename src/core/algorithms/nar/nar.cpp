#include "nar.h"

namespace model {
std::string NAR::ToString() const {
    std::string result;
    result += std::to_string(qualities_.fitness);
    result += " {";
    for (auto it{ante_.begin()}; it != ante_.end(); ++it) {
        if (it != ante_.begin()) {
            result += ", ";
        }
        result += std::to_string(it->first);
        result += ": ";
        result += it->second->ToString();
    }
    result += "} ===> {";
    for (auto it{cons_.begin()}; it != cons_.end(); ++it) {
        if (it != cons_.begin()) {
            result += ", ";
        }
        result += std::to_string(it->first);
        result += ": ";
        result += it->second->ToString();
    }
    result += "} s: ";
    result += std::to_string(qualities_.support);
    result += " c: ";
    result += std::to_string(qualities_.confidence);
    return result;
}

NARQualities CalcQualities(size_t num_rows_fit_ante, size_t num_rows_fit_ante_and_cons,
                           size_t included_features, size_t feature_count, size_t num_rows) {
    NARQualities result;
    if (num_rows_fit_ante == 0) {
        result.fitness = 0.0;
        result.confidence = 0.0;
        return result;
    }
    result.confidence = num_rows_fit_ante_and_cons / (double)num_rows_fit_ante;

    result.support = num_rows_fit_ante_and_cons / (double)num_rows;
    if (result.support == 0.0) {
        result.fitness = 0.0;
        result.support = 0.0;
        return result;
    }

    double inclusion = included_features / (double)feature_count;
    result.fitness = (result.confidence + result.support + inclusion) / 3.0;
    return result;
}

// TODO: this function is way too big and cluttered
void NAR::SetQualities(TypedRelation const* typed_relation) {
    if (ante_.size() == 0 || cons_.size() == 0) {
        qualities_.fitness = 0.0;
        qualities_consistent_ = true;
        return;
    }

    size_t num_rows_fit_ante = 0;
    size_t num_rows_fit_ante_and_cons = 0;
    for (size_t rowi = 0; rowi < typed_relation->GetNumRows(); rowi++) {
        bool row_fits_ante = true;
        bool row_fits_cons = true;
        for (size_t coli = 0; coli < typed_relation->GetNumColumns(); coli++) {
            model::TypedColumnData const& column = typed_relation->GetColumnData(coli);
            auto value = column.GetValue(rowi);
            row_fits_ante &= AnteFitsValue(coli, value);
            if (!row_fits_ante) {
                break;
            }
            row_fits_cons &= ConsFitsValue(coli, value);
        }
        num_rows_fit_ante += row_fits_ante;
        num_rows_fit_ante_and_cons += (row_fits_ante && row_fits_cons);
    }

    qualities_ = CalcQualities(num_rows_fit_ante, num_rows_fit_ante_and_cons,
                               ante_.size() + cons_.size(), typed_relation->GetNumColumns(),
                               typed_relation->GetNumRows());
    qualities_consistent_ = true;
}

model::NARQualities const& NAR::GetQualities() const {
    if (!qualities_consistent_) {
        throw std::logic_error("Getting uninitialized qualities from NAR.");
    }
    return qualities_;
}

void NAR::InsertInAnte(size_t feature_index, std::shared_ptr<ValueRange> range) {
    qualities_consistent_ = false;
    ante_.insert({feature_index, range});
}

void NAR::InsertInCons(size_t feature_index, std::shared_ptr<ValueRange> range) {
    qualities_consistent_ = false;
    cons_.insert({feature_index, range});
}

bool NAR::MapFitsValue(std::map<size_t, std::shared_ptr<ValueRange>> map, size_t feature_index,
                       std::byte const* value) {
    bool map_binds_feature = map.find(feature_index) != map.end();
    if (!map_binds_feature) {
        return true;
    } else if (map.at(feature_index)->Includes(value)) {
        return true;
    } else {
        return false;
    }
}

}  // namespace model
