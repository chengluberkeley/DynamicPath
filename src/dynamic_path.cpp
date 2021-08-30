/*
Copyright 2016-2018, Cheng Lu, chenglu@berkeley.edu

Implementation of the functions in dynamic_path.h
*/

#include "dynamic_path.h"

#include <cmath>
#include <iostream>
#include <utility>

using namespace std;

typedef pair<TreeNode*, int> status_tuple;

bool close_to_zero(double x) {
	if (fabs(x) < 1e-6)
		return true;
	else
		return false;
}

int max(int i, int j) {
	if (i >= j)
		return i;
	else
		return j;
}


// Constructor
dynamic_paths::dynamic_paths() {
	// dps = nullptr;
}

// Destructor
dynamic_paths::~dynamic_paths() {
	// clearall_(dps);
}

// Public functions
TreeNode* dynamic_paths::gen_new_node(bool is_external) {
	TreeNode* p = new TreeNode();
	// p->reversed = 0;
	p->external = is_external;
	p->node_index = 0;
	p->bparent = nullptr;
	p->netmin = 0;
	p->netcost = 0;
	p->bhead = nullptr;
	p->bleft = nullptr;
	p->bright = nullptr;
	p->btail = nullptr;
	p->height = 1;
	return p;
}

TreeNode* dynamic_paths::gen_new_node(bool is_external, int node_index) {
	TreeNode* p = new TreeNode();
	// p->reversed = 0;
	p->external = is_external;
	p->node_index = node_index;
	p->bparent = nullptr;
	p->netmin = 0;
	p->netcost = 0;
	p->bhead = nullptr;
	p->bleft = nullptr;
	p->bright = nullptr;
	p->btail = nullptr;
	p->height = 1;
	return p;
}

TreeNode* dynamic_paths::path(TreeNode* v) {
	while (v->bparent != nullptr) {
		v = v->bparent;
	}

	return v;
}

TreeNode* dynamic_paths::head(TreeNode* p) {
	/* if (p->reversed == 1) {
		return p->btail;
	}
	else {
		return p->bhead;
	} */

	return p->bhead;
}

TreeNode* dynamic_paths::tail(TreeNode* p) {
	/* if (p->reversed == 1) {
		return p->bhead;
	}
	else {
		return p->btail;
	} */

	return p->btail;
}

TreeNode* dynamic_paths::before(TreeNode* v) {
	// Back up the nodes
	/* vector<status_tuple> status;
	status.push_back(make_pair(v, v->reversed));
	v = v->bparent;
	while (v != nullptr) {
		status.push_back(make_pair(v, v->reversed));
		v = v->bparent;
	}

	// Compute node reversal states
	int status_var = status[status.size() - 1].second;
	for (int i = status.size() - 2; i >= 0; --i) {
		status_var = status_var ^ (status[i].second);
		status[i].second = status_var;
	} */

	// Find the deepest node w
	// Find node u
	/* TreeNode* w = nullptr;
	TreeNode* u = nullptr;
	for (int i = 0; i < status.size() - 1; ++i) {
		if (status[i + 1].second == 0) {
			if (status[i + 1].first->bright == status[i].first) {
				w = status[i].first;
				u = status[i + 1].first->bleft;
				break;
			}
		}
		else {
			if (status[i + 1].first->bleft == status[i].first) {
				w = status[i].first;
				u = status[i + 1].first->bright;
				break;
			}
		}
	}

	if (w == nullptr)
		return nullptr;

	if (u->external)
		return u;
	else {
		// Compute the reversal state of u
		TreeNode* temp_u = u;
		status_var = temp_u->reversed;
		while (temp_u->bparent != nullptr) {
			temp_u = temp_u->bparent;
			status_var = status_var ^ (temp_u->reversed);
		}

		if (status_var == 0) {
			return u->btail;
		}
		else {
			return u->bhead;
		}
	} */

	TreeNode* w = v;
	TreeNode* w_parent = w->bparent;
	TreeNode* u = nullptr;
	while (w_parent != nullptr) {
		if (w == w_parent->bright) {
			u = w_parent->bleft;
			break;
		}
		w = w_parent;
		w_parent = w->bparent;
	}

	if (u == nullptr)
		return nullptr;

	if (u->external)
		return u;

	return u->btail;
}

TreeNode* dynamic_paths::after(TreeNode* v) {
	// Back up the nodes
	/* vector<status_tuple> status;
	status.push_back(make_pair(v, v->reversed));
	v = v->bparent;
	while (v != nullptr) {
		status.push_back(make_pair(v, v->reversed));
		v = v->bparent;
	}

	// Compute node reversal states
	int status_var = status[status.size() - 1].second;
	for (int i = status.size() - 2; i >= 0; --i) {
		status_var = status_var ^ (status[i].second);
		status[i].second = status_var;
	}

	// Find the deepest node w
	// Find node u
	TreeNode* w = nullptr;
	TreeNode* u = nullptr;
	for (int i = 0; i < status.size() - 1; ++i) {
		if (status[i + 1].second == 0) {
			if (status[i + 1].first->bleft == status[i].first) {
				w = status[i].first;
				u = status[i + 1].first->bright;
				break;
			}
		}
		else {
			if (status[i + 1].first->bright == status[i].first) {
				w = status[i].first;
				u = status[i + 1].first->bleft;
				break;
			}
		}
	}

	if (w == nullptr)
		return nullptr;

	if (u->external)
		return u;
	else {
		// Compute the reversal state of u
		TreeNode* temp_u = u;
		status_var = temp_u->reversed;
		while (temp_u->bparent != nullptr) {
			temp_u = temp_u->bparent;
			status_var = status_var ^ (temp_u->reversed);
		}

		if (status_var == 0) {
			return u->bhead;
		}
		else {
			return u->btail;
		}
	} */

	TreeNode* w = v;
	TreeNode* w_parent = w->bparent;
	TreeNode* u = nullptr;
	while (w_parent != nullptr) {
		if (w == w_parent->bleft) {
			u = w_parent->bright;
			break;
		}
		w = w_parent;
		w_parent = w->bparent;
	}

	if (u == nullptr)
		return nullptr;

	if (u->external)
		return u;

	return u->bhead;
}

double dynamic_paths::pcost_before(TreeNode* v) {
	// Check if v is the head of path(v)
	if (v == head(path(v))) return 0;

	// Back up the nodes
	vector<TreeNode*> backup_nodes;
	backup_nodes.push_back(v);
	v = v->bparent;
	while (v != nullptr) {
		backup_nodes.push_back(v);
		v = v->bparent;
	}

	// Compute grossmin values
	vector<double> grossmin(backup_nodes.size(), 0);
	grossmin[grossmin.size() - 1] = backup_nodes[backup_nodes.size() - 1]->netmin;
	for (int i = grossmin.size() - 2; i >= 0; --i) {
		grossmin[i] = grossmin[i + 1] + backup_nodes[i]->netmin;
	}

	// Find the deepest node w
	// Here w is guaranteed to be non-null!
	for (int i = 0; i < backup_nodes.size() - 1; ++i) {
		if (backup_nodes[i + 1]->bright == backup_nodes[i])
			return backup_nodes[i + 1]->netcost + grossmin[i + 1];
	}

    return 0;
}

double dynamic_paths::pcost_after(TreeNode* v) {
	// Check if v is the tail of path(v)
	if (v == tail(path(v))) return 0;

	// Back up the nodes
	vector<TreeNode*> backup_nodes;
	backup_nodes.push_back(v);
	v = v->bparent;
	while (v != nullptr) {
		backup_nodes.push_back(v);
		v = v->bparent;
	}

	// Compute grossmin values
	vector<double> grossmin(backup_nodes.size(), 0);
	grossmin[grossmin.size() - 1] = backup_nodes[backup_nodes.size() - 1]->netmin;
	for (int i = grossmin.size() - 2; i >= 0; --i) {
		grossmin[i] = grossmin[i + 1] + backup_nodes[i]->netmin;
	}

	// Find the deepest node w
	// Here w is guaranteed to be non-null!
	for (int i = 0; i < backup_nodes.size() - 1; ++i) {
		if (backup_nodes[i + 1]->bleft == backup_nodes[i])
			return backup_nodes[i + 1]->netcost + grossmin[i + 1];
	}

    return 0;
}

bool pmincost_condition_before(TreeNode* u) {
	if (!close_to_zero(u->netcost)) return false;
	if ((u->bleft->external) || (u->bleft->netmin > 0))
		return true;
	else
		return false;
}

TreeNode* dynamic_paths::pmincost_before(TreeNode* root) {
	if (root->external) return nullptr;

	TreeNode* u = root;
	while (!pmincost_condition_before(u)) {
		if ((!u->bleft->external) && (close_to_zero(u->bleft->netmin))) {
			u = u->bleft;
		}
		else if (u->netcost > 0) {
			u = u->bright;
		}
	}

	if (u->bright->external)
		return u->btail;
	else
		return u->bright->bhead;
}

bool pmincost_condition_after(TreeNode* u) {
	if (!close_to_zero(u->netcost)) return false;
	if ((u->bright->external) || (u->bright->netmin > 0))
		return true;
	else
		return false;
}

TreeNode* dynamic_paths::pmincost_after(TreeNode* root) {
	if (root->external) return nullptr;

	TreeNode* u = root;
	while (!pmincost_condition_after(u)) {
		if ((!u->bright->external) && (close_to_zero(u->bright->netmin))) {
			u = u->bright;
		}
		else if (u->netcost > 0) {
			u = u->bleft;
		}
	}

	if (u->bleft->external)
		return u->bhead;
	else
		return u->bleft->btail;
}

void dynamic_paths::pupdate(TreeNode* root, double x) {
	root->netmin = root->netmin + x;
}

TreeNode* dynamic_paths::concatenate(TreeNode* p, TreeNode* q, double x) {
	if ((p == nullptr) || (q == nullptr)) return nullptr;

	TreeNode* root = construct_(p, q, x);
	root = top_down_balance_(root);
	return root;
}

// head(path(v)) to before(v); v to tail(path(v))
void dynamic_paths::split_before(TreeNode* v, TreeNode*& p, TreeNode*&  q, double& x) {
	// Similar to the code of pcost_before, we need to first locate the tree node corresponding to the edge [head(v), v]
	if (v == head(path(v))) {
		p = nullptr;
		q = path(v);
		x = 0;
		return;
	}

	// Find the edge [before(v), v]
	// Back up the nodes
	vector<TreeNode*> backup_nodes;
	backup_nodes.push_back(v);
	v = v->bparent;
	while (v != nullptr) {
		backup_nodes.push_back(v);
		v = v->bparent;
	}

	// Find the deepest node w
	// Here w is guaranteed to be non-null!
	int edge_index;
	for (int i = 0; i < backup_nodes.size() - 1; ++i) {
		if (backup_nodes[i + 1]->bright == backup_nodes[i]) {
			edge_index = i + 1;
			break;
		}
	}

	// Start to split the path
	// Initialization
	p = nullptr;
	q = nullptr;

	vector<TreeNode*> p_list;
	vector<double> p_cost_list;
	vector<TreeNode*> q_list;
	vector<double> q_cost_list;

	TreeNode* temp_v;
	TreeNode* temp_w;
	double temp_x;
	// From root to the parent of the edge
	for (int i = backup_nodes.size() - 1; i >= edge_index + 1; --i) {
		if (backup_nodes[i]->bleft == backup_nodes[i - 1]) {
			// Destroy the tree
			destroy_(backup_nodes[i], temp_v, temp_w, temp_x);
			q_list.push_back(temp_w);
			q_cost_list.push_back(temp_x);
		}
		else {
			// Destroy the tree
			destroy_(backup_nodes[i], temp_v, temp_w, temp_x);
			p_list.push_back(temp_v);
			p_cost_list.push_back(temp_x);
		}
	}

	destroy_(backup_nodes[edge_index], temp_v, temp_w, temp_x);
	x = temp_x;
	p_list.push_back(temp_v);
	q_list.push_back(temp_w);

	// Generate p
	p = p_list[0];
	for (int i = 1; i < p_list.size(); ++i) {
		p = concatenate(p, p_list[i], p_cost_list[i - 1]);
	}

	// Generate q
	q = q_list[0];
	for (int i = 1; i < q_list.size(); ++i) {
		q = concatenate(q_list[i], q, q_cost_list[i - 1]);
	}
}

// head(path(v)) to v; after(v) to tail(path(v))
void dynamic_paths::split_after(TreeNode* v, TreeNode* & p, TreeNode* & q, double& y) {
	// Similar to the code of pcost_after, we need to first locate the tree node corresponding to the edge [v, after(v)]
	if (v == tail(path(v))) {
		p = path(v);
		q = nullptr;
		y = 0;
		return;
	}

	// Find the edge [v, after(v)]
	// Back up the nodes
	vector<TreeNode*> backup_nodes;
	backup_nodes.push_back(v);
	v = v->bparent;
	while (v != nullptr) {
		backup_nodes.push_back(v);
		v = v->bparent;
	}

	// Find the deepest node w
	// Here w is guaranteed to be non-null!
	int edge_index;
	for (int i = 0; i < backup_nodes.size() - 1; ++i) {
		if (backup_nodes[i + 1]->bleft == backup_nodes[i]) {
			edge_index = i + 1;
			break;
		}
	}

	// Start to split the path
	// Initialization
	p = nullptr;
	q = nullptr;

	vector<TreeNode*> p_list;
	vector<double> p_cost_list;
	vector<TreeNode*> q_list;
	vector<double> q_cost_list;

	TreeNode* temp_v;
	TreeNode* temp_w;
	double temp_y;
	// From root to the parent of the edge
	for (int i = backup_nodes.size() - 1; i >= edge_index + 1; --i) {
		if (backup_nodes[i]->bleft == backup_nodes[i - 1]) {
			// Destroy the tree
			destroy_(backup_nodes[i], temp_v, temp_w, temp_y);
			q_list.push_back(temp_w);
			q_cost_list.push_back(temp_y);
		}
		else {
			// Destroy the tree
			destroy_(backup_nodes[i], temp_v, temp_w, temp_y);
			p_list.push_back(temp_v);
			p_cost_list.push_back(temp_y);
		}
	}

	destroy_(backup_nodes[edge_index], temp_v, temp_w, temp_y);
	y = temp_y;
	p_list.push_back(temp_v);
	q_list.push_back(temp_w);

	// Generate p
	p = p_list[0];
	for (int i = 1; i < p_list.size(); ++i) {
		p = concatenate(p, p_list[i], p_cost_list[i - 1]);
	}

	// Generate q
	q = q_list[0];
	for (int i = 1; i < q_list.size(); ++i) {
		q = concatenate(q_list[i], q, q_cost_list[i - 1]);
	}
}

void dynamic_paths::print(TreeNode* root, double min_value) {
	if ((root == nullptr) || (root->external)) return;
	print(root->bleft, root->netmin + min_value);
	double grossmin = root->netmin + min_value;
	cout << root->netcost + grossmin << " ";
	print(root->bright, root->netmin + min_value);
}

void dynamic_paths::clearall(TreeNode* p) {
	if (p == nullptr) return;

	if (p->bleft != nullptr) {
		clearall(p->bleft);
	}

	if (p->bright != nullptr) {
		clearall(p->bright);
	}

	delete p;
}

// Private functions
TreeNode* dynamic_paths::construct_(TreeNode* v, TreeNode* w, double x) {
	if ((v == nullptr) || (w == nullptr)) return nullptr;

	TreeNode* root = gen_new_node(false);
	// Compute grossmin
	double gross_min = x;
	if (!v->external) {
		if (v->netmin < gross_min)
			gross_min = v->netmin;
	}

	if (!w->external) {
		if (w->netmin < gross_min)
			gross_min = w->netmin;
	}

	root->netcost = x - gross_min;
	root->netmin = gross_min;

	root->bleft = v;
	root->bright = w;

	if (v->external) {
		root->bhead = v;
	}
	else {
		root->bhead = v->bhead;
	}

	if (w->external) {
		root->btail = w;
	}
	else {
		root->btail = w->btail;
	}

	// Update fields of v and w
	v->bparent = root;
	if (!v->external) {
		v->netmin = v->netmin - gross_min;
	}
	w->bparent = root;
	if (!w->external) {
		w->netmin = w->netmin - gross_min;
	}

	// Update the height
	root->height = max(v->height, w->height) + 1;

	return root;
}

void dynamic_paths::destroy_(TreeNode* root, TreeNode* &v, TreeNode* &w, double& x) {
	if ((root == nullptr) || (root->external)) return;

	v = root->bleft;
	v->bparent = nullptr;

	if (!v->external) {
		// Update netmin
		v->netmin = v->netmin + root->netmin;
	}

	w = root->bright;
	w->bparent = nullptr;
	if (!w->external) {
		// Update netmin
		w->netmin = w->netmin + root->netmin;
	}

	x = root->netcost + root->netmin;

	delete root;
}

TreeNode* dynamic_paths::rotateleft_(TreeNode* root) {
	if (root == nullptr) return nullptr;

	// Make sure the root has an internal right child
	if ((root->bright == nullptr) || (root->bright->external))
		return nullptr;

	TreeNode* new_root = root->bright;

	// Update the fields
	// Change the shape
	// Update the bleft and bright fields
	root->bright = new_root->bleft;
	new_root->bleft = root;
	TreeNode* p = root->bleft;
	TreeNode* q = root->bright;
	TreeNode* r = new_root->bright;

	// reversed
	/* q->reversed = q->reversed ^ (new_root->reversed);
	r->reversed = r->reversed ^ (new_root->reversed);
	new_root->reversed = root->reversed;
	root->reversed = 0; */

	// bparent
	root->bparent = new_root;
	new_root->bparent = nullptr;
	q->bparent = root;

	// netmin and netcost
	// compute old grosscost and grossmin
	double root_grossmin = root->netmin;
	double root_grosscost = root->netcost + root_grossmin;
	double new_root_grossmin = root_grossmin + new_root->netmin;
	double new_root_grosscost = new_root->netcost + new_root_grossmin;
	double p_grossmin = root_grossmin + p->netmin;
	double q_grossmin = new_root_grossmin + q->netmin;
	double r_grossmin = new_root_grossmin + r->netmin;
	// Compute new grosscost and grossmin
	double root_grossmin_new = root_grosscost;
	if (!p->external) {
		if (p_grossmin < root_grossmin_new)
			root_grossmin_new = p_grossmin;
	}

	if (!q->external) {
		if (q_grossmin < root_grossmin_new)
			root_grossmin_new = q_grossmin;
	}

	double new_root_grossmin_new = new_root_grossmin;
	if (root_grossmin_new < new_root_grossmin_new)
		new_root_grossmin_new = root_grossmin_new;

	new_root->netmin = new_root_grossmin_new;
	new_root->netcost = new_root_grosscost - new_root_grossmin_new;

	root->netmin = root_grossmin_new - new_root_grossmin_new;
	root->netcost = root_grosscost - root_grossmin_new;

	p->netmin = p_grossmin - root_grossmin_new;
	q->netmin = q_grossmin - root_grossmin_new;
	r->netmin = r_grossmin - new_root_grossmin_new;

	// Head and tail pointers
	if (q->external)
		root->btail = q;
	else
		root->btail = q->btail;
	if (p->external)
		new_root->bhead = p;
	else
		new_root->bhead = p->bhead;

	// Update the height
	root->height = max(p->height, q->height) + 1;
	new_root->height = max(root->height, r->height) + 1;

	return new_root;
}

TreeNode* dynamic_paths::rotateright_(TreeNode* root) {
	if (root == nullptr) return nullptr;

	// Make sure the root has an internal right child
	if ((root->bleft == nullptr) || (root->bleft->external))
		return nullptr;

	TreeNode* new_root = root->bleft;

	// Update the fields
	// Change the shape
	// Update the bleft and bright fields
	root->bleft = new_root->bright;
	new_root->bright = root;
	TreeNode* p = new_root->bleft;
	TreeNode* q = root->bleft;
	TreeNode* r = root->bright;

	// reversed
	/* p->reversed = p->reversed ^ (new_root->reversed);
	q->reversed = q->reversed ^ (new_root->reversed);
	new_root->reversed = root->reversed;
	root->reversed = 0; */

	// bparent
	root->bparent = new_root;
	new_root->bparent = nullptr;
	q->bparent = root;

	// netmin and netcost
	// compute old grosscost and grossmin
	double root_grossmin = root->netmin;
	double root_grosscost = root->netcost + root_grossmin;
	double new_root_grossmin = root_grossmin + new_root->netmin;
	double new_root_grosscost = new_root->netcost + new_root_grossmin;
	double p_grossmin = new_root_grossmin + p->netmin;
	double q_grossmin = new_root_grossmin + q->netmin;
	double r_grossmin = root_grossmin + r->netmin;
	// Compute new grosscost and grossmin
	double root_grossmin_new = root_grosscost;
	if (!q->external) {
		if (q_grossmin < root_grossmin_new)
			root_grossmin_new = q_grossmin;
	}

	if (!r->external) {
		if (r_grossmin < root_grossmin_new)
			root_grossmin_new = r_grossmin;
	}

	double new_root_grossmin_new = new_root_grossmin;
	if (root_grossmin_new < new_root_grossmin_new)
		new_root_grossmin_new = root_grossmin_new;

	new_root->netmin = new_root_grossmin_new;
	new_root->netcost = new_root_grosscost - new_root_grossmin_new;

	root->netmin = root_grossmin_new - new_root_grossmin_new;
	root->netcost = root_grosscost - root_grossmin_new;

	p->netmin = p_grossmin - new_root_grossmin_new;
	q->netmin = q_grossmin - root_grossmin_new;
	r->netmin = r_grossmin - root_grossmin_new;

	// Head and tail pointers
	if (q->external)
		root->bhead = q;
	else
		root->bhead = q->bhead;

	if (r->external)
		new_root->btail = r;
	else
		new_root->btail = r->btail;

	// Update the height
	root->height = max(q->height, r->height) + 1;
	new_root->height = max(p->height, root->height) + 1;

	return new_root;
}

TreeNode* dynamic_paths::top_down_balance_(TreeNode* root) {
	TreeNode* p = root->bleft;
	TreeNode* q = root->bright;

	// Balance the trees: Preparations
	if (p->height >= q->height + 2) {  // Right rotation is required
									   // Make sure the right sub-tree of p has a smaller height
		if (p->bleft->height < p->bright->height) {
			// First make a rotation in p
			p->netmin = p->netmin + root->netmin;  // Take it as a separate tree
			p = rotateleft_(p);
			p->bparent = root;
			root->bleft = p;
			p->netmin = p->netmin - root->netmin;
		}

		root = rotateright_(root);
		// Compute the grossmin
		root->bright->netmin = root->bright->netmin + root->netmin;
		root->bright = top_down_balance_(root->bright);
		root->bright->bparent = root;
		root->bright->netmin = root->bright->netmin - root->netmin;
		if (root->bright->external)
			root->btail = root->bright;
		else
			root->btail = root->bright->btail;

		return root;
	}

	if (q->height >= p->height + 2) {
		// Make sure the left sub-tree of q has a smaller height
		if (q->bright->height < q->bleft->height) {
			// First make a rotation in q
			q->netmin = q->netmin + root->netmin;
			q = rotateright_(q);
			q->bparent = root;
			root->bright = q;
			q->netmin = q->netmin - root->netmin;
		}

		root = rotateleft_(root);
		// Compute the grossmin
		root->bleft->netmin = root->bleft->netmin + root->netmin;
		root->bleft = top_down_balance_(root->bleft);
		root->bleft->bparent = root;
		root->bleft->netmin = root->bleft->netmin - root->netmin;
		if (root->bleft->external)
			root->bhead = root->bleft;
		else
			root->bhead = root->bleft->bhead;

		return root;
	}

	return root;
}
