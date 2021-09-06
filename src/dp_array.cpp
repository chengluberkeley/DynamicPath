/*
Copyright 2016-2021, Cheng Lu, chenglu@berkeley.edu

Implementation of the functions in dp_array.h
*/

#include "dp_array.h"

#include <cmath>
#include <iostream>

using namespace std;

#pragma mark Public functions

dp_array::~dp_array() {
	m_dp_ops.clearall(m_root);
}

void dp_array::init_dynamic_path(const vector<double>& input) {
    if (input.empty()) {
        return;
    }

    // Reset existing results.
    if (m_root) {
        m_dp_ops.clearall(m_root);
        m_root = nullptr;
        m_external_nodes.clear();
    }

	m_external_nodes.push_back(m_dp_ops.gen_new_node(true, 0));
	m_root = m_external_nodes[0];
	for (std::size_t i = 0; i < input.size(); ++i) {
        m_external_nodes.push_back(m_dp_ops.gen_new_node(true, static_cast<int>(i + 1)));
		m_root = m_dp_ops.concatenate(m_root, m_external_nodes[i + 1], input[i]);
	}

	// print_height();
}

void dp_array::update_constant(int i_k, double w) {
    if (!m_root || i_k < 0 || i_k > m_external_nodes.size()) {
        return;
    }

	TreeNode* p = nullptr;
	TreeNode* q = nullptr;
	double x;
	m_dp_ops.split_before(m_external_nodes[i_k], p, q, x);
	m_dp_ops.pupdate(q, w);
    m_root = m_dp_ops.concatenate(p, q, x);

	// print_height();
}

void dp_array::update_constant(int i_k, int i_l, double w) {
    if (!m_root || i_k >= i_l || i_k < 0 || i_l > m_external_nodes.size()) {
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

    // print_height();
}

double dp_array::min_cost(int i_k) {
    if (!m_root || i_k > m_external_nodes.size()) {
        return static_cast<double>(NAN);
    }

    TreeNode* p = nullptr;
    TreeNode* q = nullptr;
    double x;
    m_dp_ops.split_before(m_external_nodes[i_k], p, q, x);
    double cost = static_cast<double>(NAN);
    if (q != nullptr) {
        TreeNode* minNode = m_dp_ops.pmincost_before(q);
        cost = m_dp_ops.pcost_before(minNode);
    }

    m_root = m_dp_ops.concatenate(p, q, x);

    // print_height();

    return cost;
}

double dp_array::min_cost(int i_k, int i_l) {
    if (!m_root || i_k > i_l || i_k < 0 || i_l > m_external_nodes.size()) {
        return static_cast<double>(NAN);
    }

    TreeNode* p1 = nullptr;
    TreeNode* p2 = nullptr;
    TreeNode* p3 = nullptr;
    double x12;
    double x23;
    m_dp_ops.split_before(m_external_nodes[i_k], p1, p2, x12);
    m_dp_ops.split_after(m_external_nodes[i_l], p2, p3, x23);
    double cost = static_cast<double>(NAN);
    if (p2 != nullptr) {
        TreeNode* minNode = m_dp_ops.pmincost_before(p2);
        cost = m_dp_ops.pcost_before(minNode);
    }

    m_root = m_dp_ops.concatenate(p1, p2, x12);
    m_root = m_dp_ops.concatenate(m_root, p3, x23);

    // print_height();

    return cost;
}

void dp_array::print_height() {
	// Output the height of the root
	cout << "Binary tree height = " << m_root->height << endl;
    assert(m_root->height < 2 * log2(m_external_nodes.size()));
}

bool dp_array::vectorize(vector<double>& output) {
    if (!m_root) {
        return false;
    }

    output.clear();
    m_dp_ops.vectorize(m_root, output);
    return true;
}
