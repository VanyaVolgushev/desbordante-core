#include "node_adress.h"

#include <algorithm>
#include <iostream>
#include <sstream>

namespace algos {

NodeAdress::NodeAdress(std::vector<OFeatureIndex> vec) : feat_i_vec_(std::move(vec)) {
    if (vec.empty()) {
        return;
    }
    OFeatureIndex prev{vec[0]};
    for (size_t i = 1; i < vec.size(); ++i) {
        assert(vec[i] <= prev);
        prev = vec[i];
    }
}

std::vector<OFeatureIndex> NodeAdress::Get() const {
    return feat_i_vec_;
}

// TODO: test this function
std::vector<NodeAdress> NodeAdress::GetChildren(size_t feat_count) const {
    auto possible_branches = std::vector<OFeatureIndex>();
    size_t branch_count = feat_count - Back() - 1;
    auto children = std::vector<NodeAdress>();
    children.reserve(branch_count);
    for (size_t child_branch = Back() + 1; child_branch < feat_count;
         ++child_branch) {  // TODO: fix all preincrements/postincrements
        auto copy = *this;
        copy.EmplaceBack(child_branch);
        children.emplace_back(std::move(copy));
    }
    return children;
}

std::vector<OFeatureIndex> NodeAdress::GetExcept(size_t at) const {
    auto copy = feat_i_vec_;
    copy.erase(copy.begin() + at);
    return copy;
}

// Returns a NodeAdress without the feat, or the same NodeAdress if it didn't contain feat
std::vector<OFeatureIndex> NodeAdress::GetExceptFeat(OFeatureIndex feat) const {
    auto copy = feat_i_vec_;
    copy.erase(std::remove(copy.begin(), copy.end(), feat), copy.end());
    return copy;
}

std::vector<OFeatureIndex> NodeAdress::GetParent() const {
    auto copy = feat_i_vec_;
    copy.erase(copy.end());
    return copy;
}

std::vector<NodeAdress> NodeAdress::GetParents() const {
    std::vector<NodeAdress> parents{};
    parents.reserve(feat_i_vec_.size());
    for (size_t i = 0; i < feat_i_vec_.size(); i++) {
        parents.emplace_back(GetExcept(i));
    }
    return parents;
}

OFeatureIndex NodeAdress::PopFront() {
    auto out = feat_i_vec_[0];
    feat_i_vec_.erase(feat_i_vec_.begin());
    return out;
}

OFeatureIndex NodeAdress::Front() const {
    return feat_i_vec_.front();
}

OFeatureIndex NodeAdress::Back() const {
    return feat_i_vec_.back();
}

size_t NodeAdress::Size() const {
    return feat_i_vec_.size();
}

bool NodeAdress::Empty() const {
    return feat_i_vec_.empty();
}

std::vector<FeatureIndex> NodeAdress::ToFeatures(std::vector<FeatureIndex> const& order) const {
    std::vector<FeatureIndex> result;
    result.reserve(feat_i_vec_.size());
    for (auto i : feat_i_vec_) {
        result.emplace(result.begin(), order[i]);  // TODO: ensure this is correct
    }
    return result;
}

std::string NodeAdress::ToString() const {
    std::ostringstream oss;
    oss << "[";
    for (size_t i = 0; i < feat_i_vec_.size(); ++i) {
        oss << feat_i_vec_[i];
        if (i < feat_i_vec_.size() - 1) oss << ", ";
    }
    oss << "]";
    return oss.str();
}

}  // namespace algos