#pragma once

#include <vector>
#include "value_range.h"
#include "model/types/types.h"

namespace model {

class NAR {
public:

    double fitness = -1;
    double support = -1;
    double confidence = -1;

    std::map<size_t, std::shared_ptr<ValueRange>> ante = std::map<size_t, std::shared_ptr<ValueRange>>();
    std::map<size_t, std::shared_ptr<ValueRange>> cons = std::map<size_t, std::shared_ptr<ValueRange>>();

    bool IncludesInAnte(size_t feature_index, const std::byte* value) const {
        return MapIncludes(ante, feature_index, value);
    }

    bool IncludesInCons(size_t feature_index, const std::byte* value) const {
        return MapIncludes(cons, feature_index, value);
    }

    std::string ToString() const {
        std::string result;
        result += std::to_string(fitness);
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
        result += std::to_string(support);
        result += " c: ";
        result += std::to_string(confidence);
        return result;
    }

private:
    //TODO: name std::map<size_t, std::shared_ptr<ValueRange>> something
    static bool MapIncludes(std::map<size_t, std::shared_ptr<ValueRange>> map, size_t feature_index, const std::byte* value) {
        for(auto const& iterator: map) {
            if (iterator.first != feature_index) {
                continue;
            }
            if (iterator.second->Includes(value)) {
                return true;
            }
        }
        return false;
    }
};

} // namespace model
