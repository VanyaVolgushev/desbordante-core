#pragma once

#include <vector>

#include "algorithms/near/near_discovery.h"
#include "candidate_prefix_tree.h"

namespace algos {

class Kingfisher : public NeARDiscovery {
private:
    double max_p_;
    unsigned max_rules_;
    std::vector<FeatureIndex> feature_frequency_order_;

    template<typename T>
    std::string VectorToString(std::vector<T> const& vec);
    double GetLowerBound1(OFeatureIndex feature);
    double GetLowerBound2(kingfisher::NodeAdress const& node_addr, OFeatureIndex cons_index,
                          bool cons_positive);
    double GetLowerBound3(kingfisher::NodeAdress const& node_addr, OFeatureIndex cons_index,
                          bool cons_positive);

    // Rule is provided with indices ordered by frequency
    double GetFishersP(model::NeARIDs const& o_rule);
    double GetItemsetFrequency(kingfisher::NodeAdress node_addr);
    
    std::vector<FeatureIndex> GetFeatureFrequency() const;
    void RegisterOptions();
    void ResetState() override;
    void MakeExecuteOptsAvailable();
    unsigned long long ExecuteInternal() override;

public:
    Kingfisher();
};

}  // namespace algos
