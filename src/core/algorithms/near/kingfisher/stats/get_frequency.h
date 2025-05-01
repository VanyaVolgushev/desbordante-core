#pragma once

#include <algorithm>
#include <iostream>

#include "algorithms/near/kingfisher/node_adress.h"
#include "algorithms/near/types.h"
#include "model/transaction/transactional_data.h"

// TODO: PLI
namespace kingfisher {

std::vector<FeatureIndex> GetFeatureFrequencyOrder(
        std::shared_ptr<model::TransactionalData> transactional_data) {
    std::vector<std::pair<size_t, FeatureIndex>> feature_frequency;
    feature_frequency.reserve(transactional_data->GetUniverseSize());
    // iota
    for (size_t i = 0; i < transactional_data->GetUniverseSize(); ++i) {
        feature_frequency.emplace_back(0, i);
    }
    for (auto const& [_, itemset] : transactional_data->GetTransactions()) {
        for (auto const& item : itemset.GetItemsIDs()) {
            ++feature_frequency[item].first;
        }
    }

    std::sort(feature_frequency.begin(), feature_frequency.end(),
              [](auto const& a, auto const& b) { return a.first < b.first; });

    std::vector<FeatureIndex> feature_frequency_order;
    feature_frequency_order.reserve(feature_frequency.size());
    for (auto const& [_, index] : feature_frequency) {
        feature_frequency_order.push_back(index);
    }
    return feature_frequency_order;
}

// TODO: slow?
double GetItemsetFrequency(std::vector<FeatureIndex> const& itemset,
                           std::shared_ptr<model::TransactionalData> transactional_data) {
    size_t occurences = 0;
    for (auto const& [_, transaction] : transactional_data->GetTransactions()) {
        bool found = true;
        for (auto const& item : itemset) {
            auto const& transaction_ids = transaction.GetItemsIDs();
            if (std::find(transaction_ids.begin(), transaction_ids.end(), item) ==
                transaction_ids.end()) {
                found = false;
                break;
            }
        }
        if (found) {
            ++occurences;
        }
    }
    return static_cast<double>(occurences) / transactional_data->GetTransactions().size();
}

}  // namespace kingfisher
