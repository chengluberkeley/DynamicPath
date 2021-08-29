/*
Copyright 2016-2018, Cheng Lu, chenglu@berkeley.edu

Header file for the dynamic path data structure
Reference: "A data structure for dynamic trees"
by D. D. Sleator and R. E. Tarjan.

Author: Cheng Lu
Email: chenglu@berkeley.edu
Date: Oct. 24, 2016.
*/

#pragma once

#include <vector>

using namespace std;

// Tree node structure
struct TreeNode {
	// We remove the reverse field to avoid confusion!
	// int reversed;  // Only 1 or 0
	bool external;
	int node_index;  // Only for external nodes! 
	TreeNode* bparent;
	double netmin;
	double netcost;
	TreeNode* bhead;
	TreeNode* bleft;
	TreeNode* bright;
	TreeNode* btail;
	// For balance
	int height;
};

/*
Interface of set of dynamic path operations
*/
class dynamic_paths {
  public:
	  dynamic_paths();
	  ~dynamic_paths();
	  TreeNode* gen_new_node(bool is_external);
	  TreeNode* gen_new_node(bool is_external, int node_index);
	  TreeNode* path(TreeNode*);
	  TreeNode* head(TreeNode*);
	  TreeNode* tail(TreeNode*);
	  TreeNode* before(TreeNode*);
	  TreeNode* after(TreeNode*);
	  double pcost_before(TreeNode*);
	  double pcost_after(TreeNode*);
	  TreeNode* pmincost_before(TreeNode*);
	  TreeNode* pmincost_after(TreeNode*);
	  void pupdate(TreeNode*, double);
	  TreeNode* concatenate(TreeNode*, TreeNode*, double);
	  // head(path(v)) to before(v); v to tail(path(v))
	  void split_before(TreeNode*, TreeNode* &, TreeNode* &, double&);
	  // head(path(v)) to v; after(v) to tail(path(v))
	  void split_after(TreeNode*, TreeNode* &, TreeNode* &, double&);

	  // Print inorder traversal result of the tree as a sanity check
	  void print(TreeNode*, double);
	  void clearall(TreeNode*);
  private:
	  // void clearall_(TreeNode*);
	  TreeNode* construct_(TreeNode*, TreeNode*, double);
	  void destroy_(TreeNode*, TreeNode* &, TreeNode* &, double&);
	  TreeNode* rotateleft_(TreeNode*);
	  TreeNode* rotateright_(TreeNode*);
	  TreeNode* top_down_balance_(TreeNode*);

	  // Data field
	  // TreeNode* dps;
};