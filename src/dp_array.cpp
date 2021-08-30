/*
Copyright 2016-2018, Cheng Lu, chenglu@berkeley.edu

Implementation of the functions in dp_array.h
*/

#include <cmath>
#include "dp_array.h"
#include <iostream>

using namespace std;

// Public functions
dp_array::~dp_array() {
	dp_ops.clearall(root);
}

void dp_array::init_dynamic_path(const vector<double>& input) {
	external_nodes.push_back(dp_ops.gen_new_node(true, 0));
	root = external_nodes[0];
	for (int i = 0; i < input.size(); ++i) {
		external_nodes.push_back(dp_ops.gen_new_node(true, i+1));
		root = dp_ops.concatenate(root, external_nodes[i + 1], input[i]);
	}

	print_height();
}

void dp_array::update_constant(int i_k, double w) {
	TreeNode* p = nullptr;
	TreeNode* q = nullptr;
	double x;
	dp_ops.split_before(external_nodes[i_k], p, q, x);
	dp_ops.pupdate(q, w);
	if (p != nullptr)
		root = dp_ops.concatenate(p, q, x);
	else
		root = q;

	print_height();
}

void dp_array::print_height() {
	// Output the height of the root
	// cout << "Height: " << root->height << "|||";
	if (root->height > 2 * log2(external_nodes.size())) {
		cout << "ERROR! Height: " << root->height << endl;
	}
}
