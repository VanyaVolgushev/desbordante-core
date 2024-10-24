#include "nar.h"

namespace model {
std::string NAR::ToString() const {
    std::string result;
    result += std::to_string(qualities.fitness);
    result += " {";
    size_t antecounter = 0;
    for (const auto & [key, value]: ante) {
        if(antecounter > 0) {
            result += ", ";
        }
        result += std::to_string(key);
        result += ": ";
        result += value->ToString();
        antecounter++;
    }
    result += "} ===> {";
    size_t conscounter = 0;
    for (const auto & [key, value]: cons) {
        if(conscounter > 0) {
            result += ", ";
        }
        result += std::to_string(key);
        result += ": ";
        result += value->ToString();
        conscounter++;
    }
    result += "} s: ";
    result += std::to_string(qualities.support);
    result += " c: ";
    result += std::to_string(qualities.confidence);
    return result;
}

//TODO: this function is way too big and cluttered
NARQualities NAR::SetQualities(TypedRelation const* typed_relation) {
    NARQualities result;
    
    if(ante.size() == 0 || cons.size() == 0) {
        result.fitness = 0.0;
        qualities = result;
        return result;
    }

    uint num_rows_fit_ante = 0;
    uint num_rows_fit_ante_and_cons = 0;
    for (size_t rowi = 0; rowi < typed_relation->GetNumRows(); rowi++) {
        bool row_fits_ante = true;
        bool row_fits_cons = true;
        for(size_t coli = 0; coli < typed_relation->GetNumColumns(); coli++) {
            const model::TypedColumnData& column = typed_relation->GetColumnData(coli);
            auto value = column.GetValue(rowi);
            row_fits_ante &= AnteFitsValue(coli, value);
            if(!row_fits_ante) {break;}
            row_fits_cons &= ConsFitsValue(coli, value);
        }
        num_rows_fit_ante += row_fits_ante;
        num_rows_fit_ante_and_cons += (row_fits_ante && row_fits_cons);
    }
    
    if (num_rows_fit_ante == 0) {
        result.confidence = 0.0;
    } else {
        result.confidence = num_rows_fit_ante_and_cons / (double)num_rows_fit_ante;
    }
    result.support = num_rows_fit_ante_and_cons / (double)typed_relation->GetNumRows();
    double inclusion = (ante.size() + cons.size()) / (double)typed_relation->GetNumColumns();
    result.fitness = (qualities.confidence + qualities.support + inclusion) / 3.0;
    qualities = result;
    return result;
}

bool NAR::MapFitsValue(std::map<size_t, std::shared_ptr<ValueRange>> map, size_t feature_index,
                      std::byte const* value) {
    bool map_binds_feature = map.find(feature_index) != map.end();
    if(!map_binds_feature) {
        return true;
    } else if (map.at(feature_index)->Includes(value)) {
        return true;
    } else {
        return false;
    }
}

}  // namespace model