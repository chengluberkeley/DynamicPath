/*
Copyright 2016-2021, Cheng Lu, chenglu@berkeley.edu

Implementation of the functions in dp_array.h
*/

#include "dp_array.h"

#include <cmath>

#pragma mark Public functions

template <typename VType>
dp_array<VType>::dp_array(const std::vector<VType>& input) {
    if (input.empty()) {
        return;
    }

    // Compute re-balance interval.
    std::size_t reBalanceInterval = 1000;
    if (input.size() / 10 < reBalanceInterval) {
        reBalanceInterval = input.size() / 10;
    }

    m_external_nodes.push_back(m_dp_ops.gen_new_node(true, 0));
    m_root = m_external_nodes[0];
    bool reBalance = false;
    for (std::size_t i = 0; i < input.size() - 1; ++i) {
        m_external_nodes.push_back(m_dp_ops.gen_new_node(true, static_cast<int>(i + 1)));
        if (i % reBalanceInterval == 0) {
            reBalance = true;
        } else {
            reBalance = false;
        }
        m_root = m_dp_ops.concatenate(m_root, m_external_nodes[i + 1], input[i], reBalance);
    }
    m_external_nodes.push_back(m_dp_ops.gen_new_node(true, static_cast<int>(input.size())));
    m_root = m_dp_ops.concatenate(m_root, m_external_nodes[input.size()], input[input.size() - 1]);
}

template <typename VType>
dp_array<VType>::~dp_array() {
    m_dp_ops.clearall(m_root);
}

template <typename VType>
VType dp_array<VType>::edge_cost(int i_k) const {
    if (!m_root || i_k < 0 || i_k >= m_external_nodes.size() - 1) {
        return static_cast<VType>(NAN);
    }

    return m_dp_ops.pcost_after(m_external_nodes[i_k]);
}

template <typename VType>
void dp_array<VType>::update_constant(int i_k, VType w) {
    if (!m_root || i_k < 0 || i_k >= m_external_nodes.size() - 1) {
        return;
    }

    TreeNode<VType>* p = nullptr;
    TreeNode<VType>* q = nullptr;
    VType x;
    m_dp_ops.split_before(m_external_nodes[i_k], p, q, x);
    m_dp_ops.pupdate(q, w);
    m_root = m_dp_ops.concatenate(p, q, x);
}

template <typename VType>
void dp_array<VType>::update_constant(int i_k, int i_l, VType w) {
    if (!m_root || i_k >= i_l || i_k < 0 || i_l >= m_external_nodes.size()) {
        return;
    }

    TreeNode<VType>* p1 = nullptr;
    TreeNode<VType>* p2 = nullptr;
    TreeNode<VType>* p3 = nullptr;
    VType x12;
    VType x23;
    m_dp_ops.split_before(m_external_nodes[i_k], p1, p2, x12);
    m_dp_ops.split_after(m_external_nodes[i_l], p2, p3, x23);
    m_dp_ops.pupdate(p2, w);
    m_root = m_dp_ops.concatenate(p1, p2, x12);
    m_root = m_dp_ops.concatenate(m_root, p3, x23);
}

template <typename VType>
VType dp_array<VType>::min_cost_first(int i_k, int& min_index) {
    if (!m_root || i_k < 0 || i_k >= m_external_nodes.size() - 1) {
        return static_cast<VType>(NAN);
    }

    TreeNode<VType>* p = nullptr;
    TreeNode<VType>* q = nullptr;
    VType x;
    m_dp_ops.split_before(m_external_nodes[i_k], p, q, x);
    VType cost;
    assert(q);
    TreeNode<VType>* minNode = m_dp_ops.pmincost_before(q);
    cost = m_dp_ops.pcost_before(minNode);
    m_root = m_dp_ops.concatenate(p, q, x);
    min_index = minNode->node_index - 1;

    return cost;
}

template <typename VType>
VType dp_array<VType>::min_cost_first(int i_k, int i_l, int& min_index) {
    if (!m_root || i_k >= i_l || i_k < 0 || i_l >= m_external_nodes.size()) {
        return static_cast<VType>(NAN);
    }

    TreeNode<VType>* p1 = nullptr;
    TreeNode<VType>* p2 = nullptr;
    TreeNode<VType>* p3 = nullptr;
    VType x12;
    VType x23;
    m_dp_ops.split_before(m_external_nodes[i_k], p1, p2, x12);
    m_dp_ops.split_after(m_external_nodes[i_l], p2, p3, x23);
    VType cost;
    assert(p2);
    TreeNode<VType>* minNode = m_dp_ops.pmincost_before(p2);
    cost = m_dp_ops.pcost_before(minNode);
    m_root = m_dp_ops.concatenate(p2, p3, x23);
    m_root = m_dp_ops.concatenate(p1, m_root, x12);
    min_index = minNode->node_index - 1;

    return cost;
}

template <typename VType>
VType dp_array<VType>::min_cost_last(int i_k, int& min_index) {
    if (!m_root || i_k < 0 || i_k >= m_external_nodes.size() - 1) {
        return static_cast<VType>(NAN);
    }

    TreeNode<VType>* p = nullptr;
    TreeNode<VType>* q = nullptr;
    VType x;
    m_dp_ops.split_before(m_external_nodes[i_k], p, q, x);
    VType cost;
    assert(q);
    TreeNode<VType>* minNode = m_dp_ops.pmincost_after(q);
    cost = m_dp_ops.pcost_after(minNode);
    m_root = m_dp_ops.concatenate(p, q, x);
    min_index = minNode->node_index;

    return cost;
}

template <typename VType>
VType dp_array<VType>::min_cost_last(int i_k, int i_l, int& min_index) {
    if (!m_root || i_k >= i_l || i_k < 0 || i_l >= m_external_nodes.size()) {
        return static_cast<VType>(NAN);
    }

    TreeNode<VType>* p1 = nullptr;
    TreeNode<VType>* p2 = nullptr;
    TreeNode<VType>* p3 = nullptr;
    VType x12;
    VType x23;
    m_dp_ops.split_before(m_external_nodes[i_k], p1, p2, x12);
    m_dp_ops.split_after(m_external_nodes[i_l], p2, p3, x23);
    VType cost;
    assert(p2);
    TreeNode<VType>* minNode = m_dp_ops.pmincost_after(p2);
    cost = m_dp_ops.pcost_after(minNode);
    m_root = m_dp_ops.concatenate(p2, p3, x23);
    m_root = m_dp_ops.concatenate(p1, m_root, x12);
    min_index = minNode->node_index;

    return cost;
}

template <typename VType>
bool dp_array<VType>::vectorize(std::vector<VType>& output) const {
    if (!m_root) {
        return false;
    }

    output.clear();
    m_dp_ops.vectorize(m_root, output);

    return true;
}

template <typename VType>
std::size_t dp_array<VType>::edge_num() const {
    if (m_external_nodes.empty()) {
        return 0;
    }

    return m_external_nodes.size() - 1;
}

template <typename VType>
std::size_t dp_array<VType>::vertex_num() const {
    return m_external_nodes.size();
}

#pragma mark Instantiations

template class dp_array<double>;
template class dp_array<float>;
template class dp_array<uint32_t>;
template class dp_array<int>;
