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

/**
 * \brief Concrete dynamic path class containing both states and operations.
 *
 * \note This is just one exemplary implementation of a concrete dynamic path class to illustrate the use of the operations.
 */
class dp_array {
  public:
    /**
     * \brief Initialize a dynamic path data structure from the raw input vector.
     * The generated dynamic path is (0, 1, ..., input.size()), where edge (i, i+1) has cost input[i].
     *
     * \param[in] input Raw input vector to initialize the dynamic path data structure from.
     */
    dp_array(const std::vector<double>& input);

    /**
     * \brief Destructor to release all memory.
     */
    ~dp_array();

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
     * \brief Get the minimum edge cost of all edges in the (sub-)path (i_k, tail),
     * and the first edge (closest to path head) achieving the minimum.
     *
     * \param[in] i_k Index of the head vertex of the (sub-)path.
     * \param[out] min_index Index of the first edge, closest to path head,
     * such that (min_index, min_index + 1) achieving the minimum.
     * \return Minimum edge cost of all edges in the (sub-)path (i_k, tail).
     * NaN (Not-A-Number) if input i_k is not valid.
     */
    double min_cost_first(int i_k, int& min_index);

    /**
     * \brief Get the minimum edge cost of all edges in the (sub-)path (i_k, i_l),
     * and the first edge (closes to path head) achieving the minimum.
     *
     * \param[in] i_k Index of the head vertex of the (sub-)path.
     * \param[in] i_l Index of the tail vertex of the (sub-)path.
     * \param[out] min_index Index of the first edge, closest to path head,
     * such that (min_index, min_index + 1) achieving the minimum.
     * \return Minimum edge cost of all edges in the (sub-)path (i_k, i_l).
     * NaN (Not-A-Number) if input (sub-)path (i_k, i_l) is not valid.
     */
    double min_cost_first(int i_k, int i_l, int& min_index);

    /**
     * \brief Get the minimum edge cost of all edges in the (sub-)path (i_k, tail),
     * and the last edge (closest to path tail) achieving the minimum.
     *
     * \param[in] i_k Index of the head vertex of the (sub-)path.
     * \param[out] min_index Index of the last edge, closest to path tail,
     * such that (min_index, min_index + 1) achieving the minimum.
     * \return Minimum edge cost of all edges in the (sub-)path (i_k, tail).
     * NaN (Not-A-Number) if input i_k is not valid.
     */
    double min_cost_last(int i_k, int& min_index);

    /**
     * \brief Get the minimum edge cost of all edges in the (sub-)path (i_k, i_l),
     * and the last edge (closes to path tail) achieving the minimum.
     *
     * \param[in] i_k Index of the head vertex of the (sub-)path.
     * \param[in] i_l Index of the tail vertex of the (sub-)path.
     * \param[out] min_index Index of the last edge, closest to path tail,
     * such that (min_index, min_index + 1) achieving the minimum.
     * \return Minimum edge cost of all edges in the (sub-)path (i_k, i_l).
     * NaN (Not-A-Number) if input (sub-)path (i_k, i_l) is not valid.
     */
    double min_cost_last(int i_k, int i_l, int& min_index);

    /**
     * \brief Vectorize the internal dynamic path data structure to an std::vector.
     *
     * \param[out] output std::vector to hold the vectorized results.
     * \return True if the vectorization is successful, False otherwise.
     */
    bool vectorize(std::vector<double>& output) const;

    /**
     * \brief Get number of edges in the dynamic path.
     *
     * \return Number of edges in the dynamic path.
     */
    std::size_t edge_num() const;

    /**
     * \brief Get the number of vertices in the dynamic path.
     *
     * \return Number of vertices in the dynamic path.
     */
    std::size_t vertex_num() const;

  private:
    // Data field
    std::vector<TreeNode*> m_external_nodes;
    TreeNode* m_root = nullptr;
    dynamic_path_ops m_dp_ops;
};
