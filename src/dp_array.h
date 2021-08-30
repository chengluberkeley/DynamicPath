/*
Copyright 2016-2018, Cheng Lu, chenglu@berkeley.edu

Header file for dynamic path implementation of arrays

Author: Cheng Lu
Email: chenglu@berkeley.edu
Date: Oct. 27, 2016.
*/

#pragma once

#include "dynamic_path.h"
#include <vector>

using namespace std;

class dp_array {
  public:
	  ~dp_array();
	  void init_dynamic_path(const vector<double>& input);
	  void update_constant(int i_k, double w);
	  void print_height();

	  // Data field
	  vector<TreeNode*> external_nodes;
	  TreeNode* root;
	  dynamic_paths dp_ops;
};
