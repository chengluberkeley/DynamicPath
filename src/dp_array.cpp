/*
Copyright 2016-2021, Cheng Lu, chenglu@berkeley.edu

Implementation of the functions in dp_array.h
*/

#include "dp_array.h"

#include <cmath>

#pragma mark Public functions

dp_array::dp_array(const std::vector<double>& input) {
    if (input.empty()) {
        return;
    }

    // TODO: Improve allocator to speed up.
    m_external_nodes.push_back(m_dp_ops.gen_new_node(true, 0));
    m_root = m_external_nodes[0];
    for (std::size_t i = 0; i < input.size(); ++i) {
        m_external_nodes.push_back(m_dp_ops.gen_new_node(true, static_cast<int>(i + 1)));
        m_root = m_dp_ops.concatenate(m_root, m_external_nodes[i + 1], input[i]);
    }
}

dp_array::~dp_array() {
    m_dp_ops.clearall(m_root);
}

double dp_array::edge_cost(int i_k) const {
    if (!m_root || i_k < 0 || i_k >= m_external_nodes.size() - 1) {
        return static_cast<double>(NAN);
    }

    return m_dp_ops.pcost_after(m_external_nodes[i_k]);
}

void dp_array::update_constant(int i_k, double w) {
    if (!m_root || i_k < 0 || i_k >= m_external_nodes.size() - 1) {
        return;
    }

    TreeNode* p = nullptr;
    TreeNode* q = nullptr;
    double x;
    m_dp_ops.split_before(m_external_nodes[i_k], p, q, x);
    m_dp_ops.pupdate(q, w);
    m_root = m_dp_ops.concatenate(p, q, x);
}

void dp_array::update_constant(int i_k, int i_l, double w) {
    if (!m_root || i_k >= i_l || i_k < 0 || i_l >= m_external_nodes.size()) {
        return;
    }

    TreeNode* p1 = nullptr;
    TreeNode* p2 = nullptr;
    TreeNode* p3 = nullptr;
    double x12;
    double x23;
    m_dp_ops.split_before(m_external_nodes[i_k], p1, p2, x12);
    m_dp_ops.split_after(m_external_nodes[i_l], p2, p3, x23);
    m_dp_ops.pupdate(p2, w);
    m_root = m_dp_ops.concatenate(p1, p2, x12);
    m_root = m_dp_ops.concatenate(m_root, p3, x23);
}

double dp_array::min_cost_first(int i_k, int& min_index) {
    if (!m_root || i_k < 0 || i_k >= m_external_nodes.size() - 1) {
        return static_cast<double>(NAN);
    }

    TreeNode* p = nullptr;
    TreeNode* q = nullptr;
    double x;
    m_dp_ops.split_before(m_external_nodes[i_k], p, q, x);
    double cost;
    assert(q);
    TreeNode* minNode = m_dp_ops.pmincost_before(q);
    cost = m_dp_ops.pcost_before(minNode);
    m_root = m_dp_ops.concatenate(p, q, x);
    min_index = minNode->node_index - 1;

    return cost;
}

double dp_array::min_cost_first(int i_k, int i_l, int& min_index) {
    if (!m_root || i_k >= i_l || i_k < 0 || i_l >= m_external_nodes.size()) {
        return static_cast<double>(NAN);
    }

    TreeNode* p1 = nullptr;
    TreeNode* p2 = nullptr;
    TreeNode* p3 = nullptr;
    double x12;
    double x23;
    m_dp_ops.split_before(m_external_nodes[i_k], p1, p2, x12);
    m_dp_ops.split_after(m_external_nodes[i_l], p2, p3, x23);
    double cost;
    assert(p2);
    TreeNode* minNode = m_dp_ops.pmincost_before(p2);
    cost = m_dp_ops.pcost_before(minNode);
    m_root = m_dp_ops.concatenate(p2, p3, x23);
    m_root = m_dp_ops.concatenate(p1, m_root, x12);
    min_index = minNode->node_index - 1;

    return cost;
}

double dp_array::min_cost_last(int i_k, int& min_index) {
    if (!m_root || i_k < 0 || i_k >= m_external_nodes.size() - 1) {
        return static_cast<double>(NAN);
    }

    TreeNode* p = nullptr;
    TreeNode* q = nullptr;
    double x;
    m_dp_ops.split_before(m_external_nodes[i_k], p, q, x);
    double cost;
    assert(q);
    TreeNode* minNode = m_dp_ops.pmincost_after(q);
    cost = m_dp_ops.pcost_after(minNode);
    m_root = m_dp_ops.concatenate(p, q, x);
    min_index = minNode->node_index;

    return cost;
}

double dp_array::min_cost_last(int i_k, int i_l, int& min_index) {
    if (!m_root || i_k >= i_l || i_k < 0 || i_l >= m_external_nodes.size()) {
        return static_cast<double>(NAN);
    }

    TreeNode* p1 = nullptr;
    TreeNode* p2 = nullptr;
    TreeNode* p3 = nullptr;
    double x12;
    double x23;
    m_dp_ops.split_before(m_external_nodes[i_k], p1, p2, x12);
    m_dp_ops.split_after(m_external_nodes[i_l], p2, p3, x23);
    double cost;
    assert(p2);
    TreeNode* minNode = m_dp_ops.pmincost_after(p2);
    cost = m_dp_ops.pcost_after(minNode);
    m_root = m_dp_ops.concatenate(p2, p3, x23);
    m_root = m_dp_ops.concatenate(p1, m_root, x12);
    min_index = minNode->node_index;

    return cost;
}

bool dp_array::vectorize(std::vector<double>& output) const {
    if (!m_root) {
        return false;
    }

    output.clear();
    m_dp_ops.vectorize(m_root, output);

    return true;
}

std::size_t dp_array::edge_num() const {
    if (m_external_nodes.empty()) {
        return 0;
    }

    return m_external_nodes.size() - 1;
}

std::size_t dp_array::vertex_num() const {
    return m_external_nodes.size();
}
