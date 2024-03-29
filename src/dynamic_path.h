/*
Copyright 2016-2021, Cheng Lu, chenglu@berkeley.edu

Header file for the dynamic path data structure
Reference: "A data structure for dynamic trees"
by D. D. Sleator and R. E. Tarjan.

Author: Cheng Lu
Email: chenglu@berkeley.edu
Date Created: Oct. 24, 2016.
*/

#pragma once

#include <vector>

// Tree node structure for dynamic path
template <typename VType>
struct TreeNode {
    bool external = false;
    int node_index;  // Valid only for "external" nodes
    TreeNode* bparent;
    VType netmin;
    VType netcost;
    TreeNode* bhead;
    TreeNode* bleft;
    TreeNode* bright;
    TreeNode* btail;
    // For tree balance
    int height;
};

/**
 * \brief Interface of dynamic path operations.
 *
 * \note This interface does not hold any dynamic path states.
 */
template <typename VType>
class dynamic_path_ops {
  public:
    /**
     * \brief Generate a new tree node.
     *
     * \param[in] is_external Whether the node is an external node (leaf node for path vertices).
     * \param[in] node_index Path vertex index of the external node.
     * \return Pointer to the created TreeNode.
     */
    TreeNode<VType>* gen_new_node(bool is_external, int node_index) const;

    /**
     * \brief Return the root node of the dynamic path containing the input TreeNode.
     *
     * \param[in] v TreeNode that can be either internal (for a path edge) or external (for a path vertex).
     * \return The root TreeNode of the path containing v.
     */
    TreeNode<VType>* path(TreeNode<VType>* v) const;

    /**
     * \brief Return the head external TreeNode (first path vertex) of a path.
     *
     * \param[in] p Root TreeNode of the path.
     * \return External TreeNode corresponding to the head of the path.
     */
    TreeNode<VType>* head(TreeNode<VType>* p) const;

    /**
     * \brief Return the tail external TreeNode (last path vertex) of a path.
     *
     * \param[in] p Root TreeNode of the path.
     * \return External TreeNode corresponding to the tail of the path.
     */
    TreeNode<VType>* tail(TreeNode<VType>* p) const;

    /**
     * \brief Return the TreeNode of path vertex u before TreeNode of path vertex v on `path(v)`.
     *
     * \param[in] v External TreeNode for a path vertex v.
     * \return External TreeNode that is "before" vertex v in the path. nullptr if v is the head of the path.
     */
    TreeNode<VType>* before(TreeNode<VType>* v) const;

    /**
     * \brief Return the TreeNode of path vertex u after TreeNode of path vertex v on `path(v)`.
     *
     * \param[in] v External TreeNode for a path vertex v.
     * \return External TreeNode that is "after" vertex v in the path. nullptr if v is the tail of the path.
     */
    TreeNode<VType>* after(TreeNode<VType>* v) const;

    /**
     * \brief Return the cost of edge (before(v), v), for the input external TreeNode of a path vertex v.
     *
     * \param[in] v External TreeNode for a path vertex v.
     * \return The cost of edge (before(v), v). Returns NaN (Not-A-Number) if v is the head of the path.
     */
    VType pcost_before(TreeNode<VType>* v) const;

    /**
     * \brief Return the cost of edge (v, after(v)), for the input external TreeNode of a path vertex v.
     *
     * \param[in] v External TreeNode for a path vertex v.
     * \return The cost of edge (v, after(v)). Returns NaN (Not-A-Number) if v is the tail of the path.
     */
    VType pcost_after(TreeNode<VType>* v) const;

    /**
     * \brief Return the external TreeNode v in p such that (before(v), v) is the minimum cost edge closest to head(p).
     *
     * \param[in] p Root TreeNode of the path.
     * \return External TreeNode v such that (before(v), v) is the minimum cost edge closest to head(p). nullptr if the input root TreeNode is external (for path vertices).
     */
    TreeNode<VType>* pmincost_before(TreeNode<VType>* p) const;

    /**
     * \brief Return the external TreeNode v in p such that (v, after(v)) is the minimum cost edge closest to tail(p).
     *
     * \param[in] p Root TreeNode of the path.
     * \return External TreeNode v such that (v, after(v)) is the minimum cost edge closest to tail(p). nullptr if the input root TreeNode is external (for path vertices).
     */
    TreeNode<VType>* pmincost_after(TreeNode<VType>* p) const;

    /**
     * \brief Add a constant value to every edge of a path.
     *
     * \note The path must have at least one edge.
     *
     * \param[in] p Root TreeNode of the path.
     * \param[in] x Constant (no restriction in sign) to be added to every edge of the path.
     */
    void pupdate(TreeNode<VType>* p, VType x) const;

    /**
     * \brief Concatenate paths p and q by adding the edge (tail(p), head(q)) of cost x.
     *
     * \param[in] p Root TreeNode of path p. It can be a singleton vertex (external TreeNode).
     * \param[in] q Root TreeNode of path q. It can be a singleton vertex (external TreeNode).
     * \param[in] x Cost of edge (tail(p), head(q)).
     * \return Root TreeNode of the concatenated new path. If q is nullptr, returns p; if p is nullptr, returns q.
     */
    TreeNode<VType>* concatenate(TreeNode<VType>* p, TreeNode<VType>* q, VType x, bool reBalance = true) const;

    /**
     * \brief Split `path(v)` into (up to) two parts by deleting the edge (before(v), v).
     *
     * \note If v == head(path(v)), p is nullptr and x is NaN (Not-A-Number).
     *
     * \param[in] v External TreeNode (vertex) pivoted for the split.
     * \param[out] p Sub-path consisting of all vertices from head(path(v)) to before(v).
     * \param[out] q Sub-path consisting of all vertices from v to tail(path(v)).
     * \param[out] x Cost of the deleted edge (before(v), v).
     */
    void split_before(TreeNode<VType>* v, TreeNode<VType>*& p, TreeNode<VType>*& q, VType& x) const;

    /**
     * \brief Split `path(v)` into (up to) two parts by deleting the edge (v, after(v)).
     *
     * \note If v == tail(path(v)), q is nullptr and x is NaN (Not-A-Number).
     *
     * \param[in] v External TreeNode (vertex) pivoted for the split.
     * \param[out] p Sub-path consisting of all vertices from head(path(v)) to v.
     * \param[out] q Sub-path consisting of all vertices from after(v) to tail(path(v)).
     * \param[out] y Cost of the deleted edge (v, after(v)).
     */
    void split_after(TreeNode<VType>* v, TreeNode<VType>*& p, TreeNode<VType>*& q, VType& y) const;

    /**
     * \brief Inorder traversal of a (sub-)tree to serialize the respective (sub-)path, and the edge costs are surfaced.
     *
     * \param[in] p Root TreeNode of the (sub-)tree.
     * \param[out] vector_path Serialized (sub-)path edge costs of the (sub-)tree.
     */
    void vectorize(TreeNode<VType>* p, std::vector<VType>& vector_path) const;

    /**
     * \brief Inorder traversal of a (sub)-tree to serialize the respective (sub-)path, and the vertex indices are surfaced.
     *
     * \param[in] p Root TreeNode of the (sub-)tree.
     * \param[out] vector_vertices Serialized (sub-)path vertex indices of the (sub-)tree.
     */
    void vectorizeVertex(TreeNode<VType>* p, std::vector<int>& vector_vertices) const;

    /**
     * \brief Clear all TreeNodes of the (sub-)path.
     *
     * \param[in] p Root TreeNode of the (sub-)tree.
     */
    void clearall(TreeNode<VType>* p) const;

  private:
    // Both input trees must be non-empty.
    TreeNode<VType>* construct_(TreeNode<VType>*, TreeNode<VType>*, VType) const;
    // Split a non-empty tree.
    void destroy_(TreeNode<VType>*, TreeNode<VType>*&, TreeNode<VType>*&, VType&) const;
    // The input TreeNode may not be a root node. Additional assumption applies though, see comment inside.
    TreeNode<VType>* rotateleft_(TreeNode<VType>*) const;
    // The input TreeNode may not be a root node. Additional assumption applies though, see comment inside.
    TreeNode<VType>* rotateright_(TreeNode<VType>*) const;
    TreeNode<VType>* top_down_balance_(TreeNode<VType>*) const;
};
