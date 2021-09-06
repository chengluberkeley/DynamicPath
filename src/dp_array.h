/*
Copyright 2016-2021, Cheng Lu, chenglu@berkeley.edu

Header file for dynamic path implementation of arrays

Author: Cheng Lu
Email: chenglu@berkeley.edu
Date Created: Oct. 27, 2016.
*/

#pragma once

#include "dynamic_path.h"
#include <vector>

using namespace std;

/**
 * \brief Concrete dynamic path class containing both states and operations.
 *
 * \note This is just one exemplary implementation of a concrete dynamic path class to illustrate the use of the operations.
 */
class dp_array {
  public:
    /**
     * \brief Destructor to release all memory.
     */
    ~dp_array();

    /**
     * \brief Initialize a dynamic path data structure from the raw input vector.
     * The root TreeNode of the constructed dynamic path data structure is assigned to its member variable, `m_root`.
     *
     * \param[in] input Raw input vector to initialize the dynamic path data structure from.
     */
    void init_dynamic_path(const vector<double>& input);

    /**
     * \brief Update costs of all edges in the (sub-)path (i_k, tail) by a constant w.
     *
     * \param[in] i_k Index of the head vertex of the (sub-)path.
     * \param[in] w Constant (no restriction in sign) to be added to every edge of the (sub-)path.
     */
    void update_constant(int i_k, double w);

    /**
     * \brief Update costs of all edges in the (sub-)path (i_k, i_l) by a constant w.
     *
     * \param[in] i_k Index of the head vertex of the (sub-)path.
     * \param[in] i_l Index of the tail vertex of the (sub-)path.
     * \param[in] w Constant (no restriction in sign) to be added to every edge of the (sub-)path.
     */
    void update_constant(int i_k, int i_l, double w);

    /**
     * \brief Get the minimum edge cost of all edges in the (sub-)path (i_k, tail).
     *
     * \param[in] i_k Index of the head vertex of the (sub-)path.
     * \return Minimum edge cost of all edges in the (sub-)path (i_k, tail).
     */
    double min_cost(int i_k);

    /**
     * \brief Get the minimum edge cost of all edges in the (sub-)path (i_k, i_l).
     *
     * \param[in] i_k Index of the head vertex of the (sub-)path.
     * \param[in] i_l Index of the tail vertex of the (sub-)path.
     * \return Minimum edge cost of all edges in the (sub-)path (i_k, i_l).
     */
    double min_cost(int i_k, int i_l);

    /**
     * \brief Print the height of the generated binary tree of the dynamic path to verify tree balance.
     */
    void print_height();

    /**
     * \brief Vectorize the internal dynamic path data structure to an std::vector.
     *
     * \param[out] output Vector to hold the vectorized results.
     * \return True if the vectorization is successful, False otherwise.
     */
    bool vectorize(vector<double>& output);

    // TODO: Add more API functions to completely hide (private) the below data fields and dynamic path binary tree operations.

    // Data field
    vector<TreeNode*> m_external_nodes;
    TreeNode* m_root = nullptr;
    dynamic_path_ops m_dp_ops;
};
