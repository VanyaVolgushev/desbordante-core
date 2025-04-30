#pragma once

#include <functional>
#include <queue>
#include <string>
#include <vector>

#include "algorithms/near/near.h"
#include "algorithms/near/types.h"
#include "node.h"
#include "node_adress.h"

namespace kingfisher {

// Rule is provided with indices ordered by frequency
using GetFishersP = std::function<double(model::NeARIDs const& rule)>;

// Returns the best possible Fisher's p-value for a node and its children given that cons_index is
// the consequence
using GetLowerBound1 = std::function<double(OFeatureIndex cons_index)>;
using GetLowerBound2or3 = std::function<double(NodeAdress const& node_addr,
                                               OFeatureIndex cons_index, bool cons_positive)>;
using GetItemsetFrequency = std::function<double(NodeAdress node_addr)>;

// All features inside this class are indexed in order of their frequency
class CandidatePrefixTree {
private:
    RoutingNode root_{true};
    size_t feat_count_;
    // TODO: store pointers to nodes alongside their addresses to avoid repeated searches through
    // the tree
    std::queue<NodeAdress> bfs_queue_;

    // k-best storage: vector for final results, min-heap for intermediate
    std::vector<model::NeARIDs> k_best_;

    struct MinCmp {
        bool operator()(model::NeARIDs const& a, model::NeARIDs const& b) const {
            // min-heap: smallest p-value on top
            return a.p_value > b.p_value;
        }
    };

    std::priority_queue<model::NeARIDs, std::vector<model::NeARIDs>, MinCmp> topk_queue_;

    GetLowerBound1 lower_bound1_;
    GetLowerBound2or3 lower_bound2_;
    GetLowerBound2or3 lower_bound3_;
    GetFishersP get_p_;
    GetItemsetFrequency get_frequency_;
    double max_p_;        // threshold p-value
    unsigned max_rules_;  // k
    double min_frequency_;

    std::optional<BranchableNode> MakeBranchableFromParents(
            NodeAdress adress_of_node_to_make) const;

    std::optional<Node* const> GetNode(NodeAdress adress);
    std::optional<Node const* const> GetNode(NodeAdress adress) const;

    void AddChildrenToQueue(NodeAdress parent);
    void ConsiderRule(NodeAdress node, OFeatureIndex cons_index, bool cons_positive, double parents_best);

    bool ConsPossible(NodeAdress node_addr, OFeatureIndex cons_index,
                                           bool cons_positive, double best_measure) const;
    bool CheckNode(NodeAdress node);
    void CheckDepth1();
    void Propagate(NodeAdress node);
    void PerformBFS();

    void FinalizeTopK();

public:
    std::vector<model::NeARIDs> GetNeARIDs(std::vector<FeatureIndex> const& frequency_order) const;

    CandidatePrefixTree(size_t feat_count, GetLowerBound1 lower_bound1,
                        GetLowerBound2or3 lower_bound2, GetLowerBound2or3 lower_bound3,
                        GetFishersP get_p, GetItemsetFrequency get_frequency, double max_p, double min_frequency,
                        unsigned max_rules);  // TODO: rename to max_rules (no underscore)
};

}  // namespace kingfisher
