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
struct TreeNode {
    bool external;
	int node_index;  // Valid only for "external" nodes
	TreeNode* bparent;
	double netmin;
	double netcost;
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
class dynamic_path_ops {
  public:
    /**
     * \brief Generate a new tree node.
     *
     * \param[in] is_external Whether the node is an external node (leaf node for path vertices).
     * \param[in] node_index Corresponding path vertex index of the external node.
     * \return Pointer to the created TreeNode.
     */
    TreeNode* gen_new_node(bool is_external, int node_index);

    /**
     * \brief Return the root node of the dynamic path containing the input TreeNode.
     *
     * \param[in] v TreeNode that can be either internal (for a path edge) or external (for a path vertex).
     * \return The root TreeNode of the path containing v.
     */
    TreeNode* path(TreeNode* v);

    /**
     * \brief Return the head external TreeNode (first path vertex) of the (sub-)path rooted at the input TreeNode.
     *
     * \param[in] p Root TreeNode of the corresponding (sub-)path.
     * \return External TreeNode corresponding to the head of the path.
     */
    TreeNode* head(TreeNode* p);

    /**
     * \brief Return the tail external TreeNode (last path vertex) of the (sub-)path rooted at the input TreeNode.
     *
     * \param[in] p Root TreeNode of the corresponding (sub-)path.
     * \return External TreeNode corresponding to the tail of the path.
     */
    TreeNode* tail(TreeNode* p);

    /**
     * \brief Return the TreeNode of path vertex u before TreeNode of path vertex v on `path(v)`.
     *
     * \param[in] v External TreeNode for a path vertex v.
     * \return External TreeNode that is "before" vertex v in the path. nullptr if v is the head of the path.
     */
    TreeNode* before(TreeNode* v);

    /**
     * \brief Return the TreeNode of path vertex u after TreeNode of path vertex v on `path(v)`.
     *
     * \param[in] v External TreeNode for a path vertex v.
     * \return External TreeNode that is "after" vertex v in the path. nullptr if v is the tail of the path.
     */
    TreeNode* after(TreeNode* v);

    /**
     * \brief Return the cost of edge (before(v), v), for the input external TreeNode of a path vertex v.
     *
     * \param[in] v External TreeNode for a path vertex v.
     * \return The cost of edge (before(v), v). Returns NaN (Not-A-Number) if v is the head of the path.
     */
    double pcost_before(TreeNode* v);

    /**
     * \brief Return the cost of edge (v, after(v)), for the input external TreeNode of a path vertex v.
     *
     * \param[in] v External TreeNode for a path vertex v.
     * \return The cost of edge (v, after(v)). Returns NaN (Not-A-Number) if v is the tail of the path.
     */
    double pcost_after(TreeNode* v);

    /**
     * \brief Return the external TreeNode v such that (before(v), v) is the minimum cost edge closest to head(root) of the path rooted at the input TreeNode.
     *
     * \param[in] root Root TreeNode of the corresponding (sub-)path.
     * \return External TreeNode v such that (before(v), v) is the minimum cost edge closest to head(root). nullptr if the input root TreeNode is external (for path vertices).
     */
    TreeNode* pmincost_before(TreeNode* root);

    /**
     * \brief Return the external TreeNode v such that (v, after(v)) is the minimum cost edge closest to tail(root) of the path rooted at the input TreeNode.
     *
     * \param[in] root Root TreeNode of the corresponding (sub-)path.
     * \return External TreeNode v such that (v, after(v)) is the minimum cost edge closest to tail(root). nullptr if the input root TreeNode is external (for path vertices).
     */
    TreeNode* pmincost_after(TreeNode* root);

    /**
     * \brief Add a constant value to every edge of a (sub-)path rooted at the input TreeNode.
     *
     * \note The (sub-)path must have at least one edge.
     *
     * \param[in] root Root TreeNode of the corresponding (sub-)path.
     * \param[in] x Constant (no restriction in sign) to be added to every edge of the (sub-)path.
     */
    void pupdate(TreeNode* root, double x);

    /**
     * \brief Concatenate paths p (rooted at TreeNode p) and q (rooted at TreeNode q) by adding the edge (tail(p), head(q)) of cost x.
     *
     * \param[in] p Root TreeNode of the corresponding (sub-)path p. It can be a singleton path vertex.
     * \param[in] q Root TreeNode of the corresponding (sub-)path q. It can be a singleton path vertex.
     * \param[in] x Cost of edge (tail(p), head(q)).
     * \return Root TreeNode of the concatenated new path. If the input q is nullptr, returns p; if the input p is nullptr, returns q.
     */
    TreeNode* concatenate(TreeNode* p, TreeNode* q, double x);

    /**
     * \brief Split `path(v)` into (up to) two parts by deleting the edge (before(v), v).
     *
     * \note If v == head(path(v)), p is nullptr and x is NaN (Not-A-Number).
     *
     * \param[in] v External TreeNode (path vertex) pivoted for the split.
     * \param[out] p (Sub-)path consisting of all vertices from head(path(v)) to before(v).
     * \param[out] q (Sub-)path consisting of all vertices from v to tail(path(v)).
     * \param[out] x Cost of the deleted edge (before(v), v).
     */
    void split_before(TreeNode* v, TreeNode*& p, TreeNode*& q, double& x);

    /**
     * \brief Split `path(v)` into (up to) two parts by deleting the edge (v, after(v)).
     *
     * \note If v == tail(path(v)), q is nullptr and x is NaN (Not-A-Number).
     *
     * \param[in] v External TreeNode (path vertex) pivoted for the split.
     * \param[out] p (Sub-)path consisting of all vertices from head(path(v)) to v.
     * \param[out] q (Sub-)path consisting of all vertices from after(v) to tail(path(v)).
     * \param[out] y Cost of the deleted edge (v, after(v)).
     */
    void split_after(TreeNode* v, TreeNode*& p, TreeNode*& q, double& y);

    /**
     * \brief Inorder traversal of the (sub-)tree rooted at the input TreeNode to generate the corresponding (sub-)path.
     *
     * \param[in] root Root TreeNode for the (sub-)tree.
     * \param[out] vector_path Vectorized (sub-)path corresponding to the (sub-)tree. It is expected to be initially empy.
     */
    void vectorize(TreeNode* root, std::vector<double>& vector_path);

    /**
     * \brief Clear all TreeNodes of the (sub-)path rooted at the input TreeNode.
     */
    void clearall(TreeNode* p);

  private:
	  TreeNode* construct_(TreeNode*, TreeNode*, double);
	  void destroy_(TreeNode*, TreeNode*&, TreeNode*&, double&);
	  TreeNode* rotateleft_(TreeNode*);
	  TreeNode* rotateright_(TreeNode*);
	  TreeNode* top_down_balance_(TreeNode*);
};
