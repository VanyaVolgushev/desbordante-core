#pragma once

#include <vector>

#include "algorithms/near/near_discovery.h"
#include "candidate_prefix_tree.h"

namespace algos {

class Kingfisher : public NeARDiscovery {
private:
    double max_p_;
    std::vector<OFeatureIndex> feature_frequency_order_;

    std::string VectorToString(std::vector<FeatureIndex> const& vec);
    double GetLowerBound1(OFeatureIndex feature);
    double GetLowerBound2(kingfisher::NodeAdress const& node_addr, OFeatureIndex cons_index,
                          bool cons_positive);
    double GetLowerBound3(kingfisher::NodeAdress const& node_addr, OFeatureIndex cons_index,
                          bool cons_positive);

    // Rule is provided with indices ordered by frequency
    double GetFishersP(model::NeARIDs const& o_rule);

    void RegisterOptions();
    void ResetState() override;
    void MakeExecuteOptsAvailable();
    unsigned long long ExecuteInternal() override;

public:
    Kingfisher();
};

}  // namespace algos
