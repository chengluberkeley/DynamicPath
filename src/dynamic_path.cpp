/*
Copyright 2016-2021, Cheng Lu, chenglu@berkeley.edu

Implementation of the functions in dynamic_path.h
*/

#include "dynamic_path.h"

#include <cassert>
#include <cmath>
#include <utility>

template <typename VType>
static bool close_to_zero(VType x) {
    return fabs(x) < 1e-6;
}

#pragma mark Public functions

template <typename VType>
TreeNode<VType>* dynamic_path_ops<VType>::gen_new_node(bool is_external, int node_index) const {
    TreeNode<VType>* p = new TreeNode<VType>();
    p->external = is_external;
    p->node_index = node_index;
    p->bparent = nullptr;
    p->netmin = VType(0);
    p->netcost = VType(0);
    p->bhead = nullptr;
    p->bleft = nullptr;
    p->bright = nullptr;
    p->btail = nullptr;
    p->height = 1;
    return p;
}

template <typename VType>
TreeNode<VType>* dynamic_path_ops<VType>::path(TreeNode<VType>* v) const {
    if (!v) {
        return nullptr;
    }

    while (v->bparent != nullptr) {
        v = v->bparent;
    }

    return v;
}

template <typename VType>
TreeNode<VType>* dynamic_path_ops<VType>::head(TreeNode<VType>* p) const {
    if (!p) {
        return nullptr;
    }

    // Must be a root node.
    assert(!p->bparent);

    if (p->external) {
        return p;
    }

    return p->bhead;
}

template <typename VType>
TreeNode<VType>* dynamic_path_ops<VType>::tail(TreeNode<VType>* p) const {
    if (!p) {
        return nullptr;
    }

    // Must be a root node.
    assert(!p->bparent);

    if (p->external) {
        return p;
    }

    return p->btail;
}

template <typename VType>
TreeNode<VType>* dynamic_path_ops<VType>::before(TreeNode<VType>* v) const {
    if (!v) {
        return nullptr;
    }

    // Must be an external vertex node.
    assert(v->external);

    TreeNode<VType>* w = v;
    TreeNode<VType>* w_parent = w->bparent;
    TreeNode<VType>* u = nullptr;
    while (w_parent != nullptr) {
        if (w == w_parent->bright) {
            u = w_parent->bleft;
            break;
        }
        w = w_parent;
        w_parent = w->bparent;
    }

    if (u == nullptr) {
        return nullptr;
    }

    if (u->external) {
        return u;
    }

    return u->btail;
}

template <typename VType>
TreeNode<VType>* dynamic_path_ops<VType>::after(TreeNode<VType>* v) const {
    if (!v) {
        return nullptr;
    }

    // Must be an external vertex node.
    assert(v->external);

    TreeNode<VType>* w = v;
    TreeNode<VType>* w_parent = w->bparent;
    TreeNode<VType>* u = nullptr;
    while (w_parent != nullptr) {
        if (w == w_parent->bleft) {
            u = w_parent->bright;
            break;
        }
        w = w_parent;
        w_parent = w->bparent;
    }

    if (u == nullptr) {
        return nullptr;
    }

    if (u->external) {
        return u;
    }

    return u->bhead;
}

template <typename VType>
VType dynamic_path_ops<VType>::pcost_before(TreeNode<VType>* v) const {
    if (!v) {
        return static_cast<VType>(NAN);
    }

    // Must be an external vertex node.
    assert(v->external);

    // Check if v is the head of path(v)
    if (v == head(path(v))) return static_cast<VType>(NAN);

    // Back up the nodes
    std::vector<TreeNode<VType>*> backup_nodes;
    backup_nodes.push_back(v);
    v = v->bparent;
    while (v != nullptr) {
        backup_nodes.push_back(v);
        v = v->bparent;
    }

    // Compute grossmin values
    std::vector<VType> grossmin(backup_nodes.size(), 0);
    // Root node
    grossmin[grossmin.size() - 1] = backup_nodes[backup_nodes.size() - 1]->netmin;
    for (int i = static_cast<int>(grossmin.size()) - 2; i >= 1; --i) {
        grossmin[i] = grossmin[i + 1] + backup_nodes[i]->netmin;
    }

    // Find the deepest node w that is the right child of its parent.
    // Here w is guaranteed to be non-null!
    for (std::size_t i = 0; i < backup_nodes.size() - 1; ++i) {
        if (backup_nodes[i + 1]->bright == backup_nodes[i]) {
            return backup_nodes[i + 1]->netcost + grossmin[i + 1];
        }
    }

    // Should not reach this step.
    assert(false);
    return static_cast<VType>(NAN);
}

template <typename VType>
VType dynamic_path_ops<VType>::pcost_after(TreeNode<VType>* v) const {
    if (!v) {
        return static_cast<VType>(NAN);
    }

    // Must be an external vertex node.
    assert(v->external);

    // Check if v is the tail of path(v)
    if (v == tail(path(v))) return static_cast<VType>(NAN);

    // Back up the nodes
    std::vector<TreeNode<VType>*> backup_nodes;
    backup_nodes.push_back(v);
    v = v->bparent;
    while (v != nullptr) {
        backup_nodes.push_back(v);
        v = v->bparent;
    }

    // Compute grossmin values
    std::vector<VType> grossmin(backup_nodes.size(), 0);
    // Root node
    grossmin[grossmin.size() - 1] = backup_nodes[backup_nodes.size() - 1]->netmin;
    for (int i = static_cast<int>(grossmin.size()) - 2; i >= 1; --i) {
        grossmin[i] = grossmin[i + 1] + backup_nodes[i]->netmin;
    }

    // Find the deepest node w that is the left child of its parent.
    // Here w is guaranteed to be non-null!
    for (std::size_t i = 0; i < backup_nodes.size() - 1; ++i) {
        if (backup_nodes[i + 1]->bleft == backup_nodes[i]) {
            return backup_nodes[i + 1]->netcost + grossmin[i + 1];
        }
    }

    // Should not reach this step.
    assert(false);
    return static_cast<VType>(NAN);
}

template <typename VType>
static bool pmincost_condition_before(TreeNode<VType>* u) {
    if (!close_to_zero(u->netcost)) return false;
    if ((u->bleft->external) || (u->bleft->netmin > 0)) {
        return true;
    } else {
        return false;
    }
}

template <typename VType>
TreeNode<VType>* dynamic_path_ops<VType>::pmincost_before(TreeNode<VType>* p) const {
    if (!p || p->external) return nullptr;

    // Must be a root node.
    assert(!p->bparent);

    TreeNode<VType>* u = p;
    while (!pmincost_condition_before(u)) {
        if ((!u->bleft->external) && (close_to_zero(u->bleft->netmin))) {
            u = u->bleft;
        } else { // u->netcost > 0
            assert(u->netcost > 0);
            u = u->bright;
        }
    }

    if (u->bright->external) {
        return u->btail;
    } else {
        return u->bright->bhead;
    }
}

template <typename VType>
static bool pmincost_condition_after(TreeNode<VType>* u) {
    if (!close_to_zero(u->netcost)) return false;
    if ((u->bright->external) || (u->bright->netmin > 0)) {
        return true;
    } else {
        return false;
    }
}

template <typename VType>
TreeNode<VType>* dynamic_path_ops<VType>::pmincost_after(TreeNode<VType>* p) const {
    if (!p || p->external) return nullptr;

    // Must be a root node.
    assert(!p->bparent);

    TreeNode<VType>* u = p;
    while (!pmincost_condition_after(u)) {
        if ((!u->bright->external) && (close_to_zero(u->bright->netmin))) {
            u = u->bright;
        } else { // u->netcost > 0
            assert(u->netcost > 0);
            u = u->bleft;
        }
    }

    if (u->bleft->external) {
        return u->bhead;
    } else {
        return u->bleft->btail;
    }
}

template <typename VType>
void dynamic_path_ops<VType>::pupdate(TreeNode<VType>* p, VType x) const {
    if (!p) {
        return;
    }

    // Must be a root node.
    assert(!p->bparent);
    // Must not be an external (vertex) node.
    assert(!p->external);

    p->netmin = p->netmin + x;
}

template <typename VType>
TreeNode<VType>* dynamic_path_ops<VType>::concatenate(TreeNode<VType>* p, TreeNode<VType>* q, VType x) const {
    if (p == nullptr) {
        return q;
    } else if (q == nullptr) {
        return p;
    }

    TreeNode<VType>* root = construct_(p, q, x);
    root = top_down_balance_(root);
    return root;
}

template <typename VType>
void dynamic_path_ops<VType>::split_before(TreeNode<VType>* v, TreeNode<VType>*& p, TreeNode<VType>*& q, VType& x) const {
    if (!v) {
        return;
    }

    // Must be an external vertex node.
    assert(v->external);

    // Similar to the code of pcost_before, we need to first locate the tree node corresponding to the edge [head(v), v]
    if (v == head(path(v))) {
        p = nullptr;
        q = path(v);
        x = static_cast<VType>(NAN);
        return;
    }

    // Find the edge [before(v), v]
    // Back up the nodes
    std::vector<TreeNode<VType>*> backup_nodes;
    backup_nodes.push_back(v);
    v = v->bparent;
    while (v != nullptr) {
        backup_nodes.push_back(v);
        v = v->bparent;
    }

    // Find the deepest node w that is the right child of its parent.
    // Here w is guaranteed to be non-null!
    std::size_t edge_index = 0;
    for (std::size_t i = 0; i < backup_nodes.size() - 1; ++i) {
        if (backup_nodes[i + 1]->bright == backup_nodes[i]) {
            edge_index = i + 1;
            break;
        }
    }
    assert(edge_index > 0);

    // Start to split the path
    // Initialization: Note that we do not free existing memories pointed by p and q.
    p = nullptr;
    q = nullptr;

    std::vector<TreeNode<VType>*> p_list;
    std::vector<VType> p_cost_list;
    std::vector<TreeNode<VType>*> q_list;
    std::vector<VType> q_cost_list;

    TreeNode<VType>* temp_v;
    TreeNode<VType>* temp_w;
    VType temp_x;
    // From root to the parent of the edge
    for (int i = static_cast<int>(backup_nodes.size()) - 1; i >= edge_index + 1; --i) {
        if (backup_nodes[i]->bleft == backup_nodes[i - 1]) {
            // Destroy the tree
            destroy_(backup_nodes[i], temp_v, temp_w, temp_x);
            q_list.push_back(temp_w);
            q_cost_list.push_back(temp_x);
        } else {
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
    for (std::size_t i = 1; i < p_list.size(); ++i) {
        p = concatenate(p, p_list[i], p_cost_list[i - 1]);
    }

    // Generate q
    auto q_list_size = static_cast<int>(q_list.size());
    q = q_list[q_list_size - 1];
    for (int i = q_list_size - 2; i >= 0; --i) {
        q = concatenate(q, q_list[i], q_cost_list[i]);
    }
}

template <typename VType>
void dynamic_path_ops<VType>::split_after(TreeNode<VType>* v, TreeNode<VType>*& p, TreeNode<VType>*& q, VType& y) const {
    if (!v) {
        return;
    }

    // Must be an external vertex node.
    assert(v->external);

    // Similar to the code of pcost_after, we need to first locate the tree node corresponding to the edge [v, after(v)]
    if (v == tail(path(v))) {
        p = path(v);
        q = nullptr;
        y = static_cast<VType>(NAN);
        return;
    }

    // Find the edge [v, after(v)]
    // Back up the nodes
    std::vector<TreeNode<VType>*> backup_nodes;
    backup_nodes.push_back(v);
    v = v->bparent;
    while (v != nullptr) {
        backup_nodes.push_back(v);
        v = v->bparent;
    }

    // Find the deepest node w that is the left child of its parent.
    // Here w is guaranteed to be non-null!
    std::size_t edge_index = 0;
    for (std::size_t i = 0; i < backup_nodes.size() - 1; ++i) {
        if (backup_nodes[i + 1]->bleft == backup_nodes[i]) {
            edge_index = i + 1;
            break;
        }
    }
    assert(edge_index > 0);

    // Start to split the path
    // Initialization: Note that we do not free existing memories pointed by p and q.
    p = nullptr;
    q = nullptr;

    std::vector<TreeNode<VType>*> p_list;
    std::vector<VType> p_cost_list;
    std::vector<TreeNode<VType>*> q_list;
    std::vector<VType> q_cost_list;

    TreeNode<VType>* temp_v;
    TreeNode<VType>* temp_w;
    VType temp_y;
    // From root to the parent of the edge
    for (int i = static_cast<int>(backup_nodes.size()) - 1; i >= edge_index + 1; --i) {
        if (backup_nodes[i]->bleft == backup_nodes[i - 1]) {
            // Destroy the tree
            destroy_(backup_nodes[i], temp_v, temp_w, temp_y);
            q_list.push_back(temp_w);
            q_cost_list.push_back(temp_y);
        } else {
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
    for (std::size_t i = 1; i < p_list.size(); ++i) {
        p = concatenate(p, p_list[i], p_cost_list[i - 1]);
    }

    // Generate q
    auto q_list_size = static_cast<int>(q_list.size());
    q = q_list[q_list_size - 1];
    for (int i = q_list_size - 2; i >= 0; --i) {
        q = concatenate(q, q_list[i], q_cost_list[i]);
    }
}

template <typename VType>
static void vectorize_internal(TreeNode<VType>* p, VType basemin, std::vector<VType>& vector_path) {
    if (!p || (p->external)) return;
    vectorize_internal(p->bleft, p->netmin + basemin, vector_path);
    VType grossmin = p->netmin + basemin;
    vector_path.push_back(p->netcost + grossmin);
    vectorize_internal(p->bright, p->netmin + basemin, vector_path);
}

template <typename VType>
void dynamic_path_ops<VType>::vectorize(TreeNode<VType>* p, std::vector<VType>& vector_path) const {
    if (!p) {
        return;
    }

    vector_path.clear();

    vectorize_internal(p, VType(0), vector_path);
}

template <typename VType>
static void vectorize_internal(TreeNode<VType>* p, std::vector<int>& vector_vertices) {
    if (!p) return;

    if (p->external) {
        vector_vertices.push_back(p->node_index);
        return;
    }

    vectorize_internal(p->bleft, vector_vertices);
    vectorize_internal(p->bright, vector_vertices);
}

template <typename VType>
void dynamic_path_ops<VType>::vectorizeVertex(TreeNode<VType>* p, std::vector<int>& vector_vertices) const {
    if (!p) {
        return;
    }

    vector_vertices.clear();

    vectorize_internal(p, vector_vertices);
}

template <typename VType>
void dynamic_path_ops<VType>::clearall(TreeNode<VType>* p) const {
    if (!p) return;

    if (p->bleft) {
        clearall(p->bleft);
    }

    if (p->bright) {
        clearall(p->bright);
    }

    delete p;
}

#pragma mark Private functions

template <typename VType>
TreeNode<VType>* dynamic_path_ops<VType>::construct_(TreeNode<VType>* v, TreeNode<VType>* w, VType x) const {
    if (!v || !w) return nullptr;

    TreeNode<VType>* root = gen_new_node(false, 0);
    // Compute grossmin
    VType gross_min = x;
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
    } else {
        root->bhead = v->bhead;
    }

    if (w->external) {
        root->btail = w;
    } else {
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
    root->height = std::max(v->height, w->height) + 1;

    return root;
}

template <typename VType>
void dynamic_path_ops<VType>::destroy_(TreeNode<VType>* root, TreeNode<VType>*& v, TreeNode<VType>*& w, VType& x) const {
    if (!root || (root->external)) return;

    v = root->bleft;
    v->bparent = nullptr;

    if (!v->external) {
        // Update netmin to grossmin for new root node.
        v->netmin = v->netmin + root->netmin;
    }

    w = root->bright;
    w->bparent = nullptr;
    if (!w->external) {
        // Update netmin to grossmin for new root node.
        w->netmin = w->netmin + root->netmin;
    }

    x = root->netcost + root->netmin;

    delete root;
}

template <typename VType>
TreeNode<VType>* dynamic_path_ops<VType>::rotateleft_(TreeNode<VType>* root) const {
    if (!root) return nullptr;

    // Make sure the root has an internal right child
    if ((root->bright == nullptr) || (root->bright->external)) {
        return nullptr;
    }

    TreeNode<VType>* new_root = root->bright;

    // Update the fields
    // Change the shape
    // Update the bleft and bright fields
    root->bright = new_root->bleft;
    new_root->bleft = root;
    TreeNode<VType>* p = root->bleft;
    TreeNode<VType>* q = root->bright;
    TreeNode<VType>* r = new_root->bright;

    // bparent
    root->bparent = new_root;
    new_root->bparent = nullptr;
    q->bparent = root;

    // netmin and netcost
    // compute old grosscost and grossmin
    VType root_grossmin = root->netmin;  // Note: If not root, we assume caller already temporarily updates the netmin to grossmin.
    VType root_grosscost = root->netcost + root_grossmin;
    VType new_root_grossmin = root_grossmin + new_root->netmin;
    VType new_root_grosscost = new_root->netcost + new_root_grossmin;
    VType p_grossmin = root_grossmin + p->netmin;
    VType q_grossmin = new_root_grossmin + q->netmin;
    VType r_grossmin = new_root_grossmin + r->netmin;
    // Compute new grosscost and grossmin
    VType root_grossmin_new = root_grosscost;
    if (!p->external) {
        if (p_grossmin < root_grossmin_new) {
            root_grossmin_new = p_grossmin;
        }
    }

    if (!q->external) {
        if (q_grossmin < root_grossmin_new) {
            root_grossmin_new = q_grossmin;
        }
    }

    VType new_root_grossmin_new = new_root_grossmin;
    if (root_grossmin_new < new_root_grossmin_new) {
        new_root_grossmin_new = root_grossmin_new;
    }

    new_root->netmin = new_root_grossmin_new;
    new_root->netcost = new_root_grosscost - new_root_grossmin_new;

    root->netmin = root_grossmin_new - new_root_grossmin_new;
    root->netcost = root_grosscost - root_grossmin_new;

    p->netmin = p_grossmin - root_grossmin_new;
    q->netmin = q_grossmin - root_grossmin_new;
    r->netmin = r_grossmin - new_root_grossmin_new;

    // Head and tail pointers
    if (q->external) {
        root->btail = q;
    } else {
        root->btail = q->btail;
    }

    if (p->external) {
        new_root->bhead = p;
    } else {
        new_root->bhead = p->bhead;
    }

    // Update the height
    root->height = std::max(p->height, q->height) + 1;
    new_root->height = std::max(root->height, r->height) + 1;

    return new_root;
}

template <typename VType>
TreeNode<VType>* dynamic_path_ops<VType>::rotateright_(TreeNode<VType>* root) const {
    if (!root) return nullptr;

    // Make sure the root has an internal left child
    if ((root->bleft == nullptr) || (root->bleft->external)) {
        return nullptr;
    }

    TreeNode<VType>* new_root = root->bleft;

    // Update the fields
    // Change the shape
    // Update the bleft and bright fields
    root->bleft = new_root->bright;
    new_root->bright = root;
    TreeNode<VType>* p = new_root->bleft;
    TreeNode<VType>* q = root->bleft;
    TreeNode<VType>* r = root->bright;

    // bparent
    root->bparent = new_root;
    new_root->bparent = nullptr;
    q->bparent = root;

    // netmin and netcost
    // compute old grosscost and grossmin
    VType root_grossmin = root->netmin;  // Assumption ditto as `rotateleft_`.
    VType root_grosscost = root->netcost + root_grossmin;
    VType new_root_grossmin = root_grossmin + new_root->netmin;
    VType new_root_grosscost = new_root->netcost + new_root_grossmin;
    VType p_grossmin = new_root_grossmin + p->netmin;
    VType q_grossmin = new_root_grossmin + q->netmin;
    VType r_grossmin = root_grossmin + r->netmin;
    // Compute new grosscost and grossmin
    VType root_grossmin_new = root_grosscost;
    if (!q->external) {
        if (q_grossmin < root_grossmin_new) {
            root_grossmin_new = q_grossmin;
        }
    }

    if (!r->external) {
        if (r_grossmin < root_grossmin_new) {
            root_grossmin_new = r_grossmin;
        }
    }

    VType new_root_grossmin_new = new_root_grossmin;
    if (root_grossmin_new < new_root_grossmin_new) {
        new_root_grossmin_new = root_grossmin_new;
    }

    new_root->netmin = new_root_grossmin_new;
    new_root->netcost = new_root_grosscost - new_root_grossmin_new;

    root->netmin = root_grossmin_new - new_root_grossmin_new;
    root->netcost = root_grosscost - root_grossmin_new;

    p->netmin = p_grossmin - new_root_grossmin_new;
    q->netmin = q_grossmin - root_grossmin_new;
    r->netmin = r_grossmin - root_grossmin_new;

    // Head and tail pointers
    if (q->external) {
        root->bhead = q;
    } else {
        root->bhead = q->bhead;
    }

    if (r->external) {
        new_root->btail = r;
    } else {
        new_root->btail = r->btail;
    }

    // Update the height
    root->height = std::max(q->height, r->height) + 1;
    new_root->height = std::max(p->height, root->height) + 1;

    return new_root;
}

template <typename VType>
TreeNode<VType>* dynamic_path_ops<VType>::top_down_balance_(TreeNode<VType>* root) const {
    if (!root || root->external) {
        return root;
    }

    TreeNode<VType>* p = root->bleft;
    TreeNode<VType>* q = root->bright;

    // Exit rule
    if (abs(p->height - q->height) <= 1) {
        return root;
    }

    // Recursively balance left subtree.
    p->netmin = p->netmin + root->netmin;
    p = top_down_balance_(p);
    p->bparent = root;
    root->bleft = p;
    p->netmin = p->netmin - root->netmin;
    // Recursively balance right subtree.
    q->netmin = q->netmin + root->netmin;
    q = top_down_balance_(q);
    q->bparent = root;
    root->bright = q;
    q->netmin = q->netmin - root->netmin;

    // Balance the top level.
    if (p->height >= q->height + 2) {  // Right rotation is required.
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
        return root;
    }

    if (q->height >= p->height + 2) {  // Left rotation is required.
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
        return root;
    }

    return root;
}

#pragma mark Instantiations

template class dynamic_path_ops<double>;
template class dynamic_path_ops<float>;
template class dynamic_path_ops<uint32_t>;
template class dynamic_path_ops<int>;
