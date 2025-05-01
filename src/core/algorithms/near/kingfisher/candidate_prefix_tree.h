#pragma once

#include <queue>
#include <string>
#include <vector>

#include "model/transaction/transactional_data.h"
#include "algorithms/near/near.h"
#include "algorithms/near/types.h"
#include "node.h"
#include "node_adress.h"

namespace kingfisher {

class CandidatePrefixTree {
private:
    RoutingNode root_{true};
    size_t feat_count_;
    std::vector<OFeatureIndex> feature_frequency_order_;
    std::queue<NodeAdress> bfs_queue_;
    std::vector<model::NeARIDs> k_best_;

    struct MinCmp {
        bool operator()(model::NeARIDs const& a, model::NeARIDs const& b) const {
            return a.p_value > b.p_value;
        }
    };
    std::priority_queue<model::NeARIDs, std::vector<model::NeARIDs>, MinCmp> topk_queue_;

    double max_p_;
    unsigned max_rules_;
    double min_frequency_;
    std::shared_ptr<model::TransactionalData> transactional_data_;

    std::optional<BranchableNode> MakeBranchableFromParents(
            NodeAdress adress_of_node_to_make) const;
    std::optional<Node* const> GetNode(NodeAdress adress);
    std::optional<Node const* const> GetNode(NodeAdress adress) const;

    void AddChildrenToQueue(NodeAdress parent);
    void ConsiderRule(NodeAdress node, OFeatureIndex cons_index, bool cons_positive,
                      double parents_best);
    bool ConsPossible(NodeAdress node_addr, OFeatureIndex cons_index, bool cons_positive,
                      double best_measure) const;
    bool CheckNode(NodeAdress node);
    void CheckDepth1();
    void Propagate(NodeAdress node);
    void PerformBFS();
    void FinalizeTopK();

    double GetFishersP(model::NeARIDs const& rule) const;
    double GetLowerBound1(OFeatureIndex cons_index) const;
    double GetLowerBound2(NodeAdress const& node_addr, OFeatureIndex cons_index,
                          bool cons_positive) const;
    double GetLowerBound3(NodeAdress const& node_addr, OFeatureIndex cons_index,
                          bool cons_positive) const;
    std::vector<FeatureIndex> GetFeatureFrequency() const;
    double GetItemsetFrequency(NodeAdress node_addr) const;

public:
    std::vector<model::NeARIDs> GetNeARIDs() const;

    CandidatePrefixTree(size_t feat_count, double max_p, double min_frequency, unsigned max_rules, std::shared_ptr<model::TransactionalData> transactional_data);
    ~CandidatePrefixTree() = default;
};

}  // namespace kingfisher
