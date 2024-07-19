#pragma once

#include "model/transaction/transactional_data.h"
#include "itemset_node.h"

namespace algos {

class CandidateHashTree {
private:
    using NodeIterator = std::list<ItemsetNode>::iterator;

    unsigned const branching_degree_;
    unsigned const min_threshold_;
    unsigned total_row_count_ = 0;
    std::unordered_map<ItemsetNode*, std::list<ItemsetNode>>& candidates_;

    model::TransactionalData const* const transactional_data_ = nullptr;

    struct LeafRow {
        NodeIterator candidate_node;
        ItemsetNode* const parent;
        unsigned transaction_count = 0;

        LeafRow(LeafRow&& other) = default;
        LeafRow& operator=(LeafRow&& other) = delete;

        LeafRow(NodeIterator node, ItemsetNode* parent) : candidate_node(node), parent(parent) {}

        LeafRow(LeafRow const& other) = delete;
    };

    struct HashTreeNode {
        unsigned level_number;
        int last_visited_transaction_id = -1;
        std::vector<HashTreeNode> children;
        std::list<LeafRow> candidates;

        HashTreeNode() = delete;

        explicit HashTreeNode(unsigned level_number) : level_number(level_number) {}
    };

    HashTreeNode root_;
    unsigned HashFunction(LeafRow const& node_row, unsigned level_num) const;

    unsigned ItemHash(unsigned item_id) const noexcept {
        return item_id % branching_degree_;
    }

    void AppendRow(LeafRow row, HashTreeNode& subtree_root);
    void AddLevel(HashTreeNode& leaf_node);
    void FindAndVisitLeaves(HashTreeNode& subtree_root, std::vector<unsigned>::const_iterator start,
                            std::vector<unsigned> const& transaction_items, int tid);
    static void VisitLeaf(HashTreeNode& leaf, std::vector<unsigned> const& transaction_items,
                          int tid);
    void Prune(double minsup, HashTreeNode& subtree_root);
    void AddCandidates();

public:
    CandidateHashTree(model::TransactionalData const* transactional_data,
                      std::unordered_map<ItemsetNode*, std::list<ItemsetNode>>& candidates,
                      unsigned branching_degree, unsigned min_threshold)
        : branching_degree_(branching_degree),
          min_threshold_(min_threshold),
          candidates_(candidates),
          transactional_data_(transactional_data),
          root_(1) {
        AddCandidates();
    }

    void AddCandidate(NodeIterator candidate, ItemsetNode* parent);

    unsigned Size() const noexcept {
        return total_row_count_;
    };

    void PerformCounting();
    void PruneNodes(double minsup);
};

}  // namespace algos
