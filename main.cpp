//
//  main.cpp
//  DynamicPath
//
//  Created by ChengLu on 8/29/21.
//

#include <cassert>
#include <cmath>
#include "dp_array.h"
#include <iostream>
#include <random>
#include <string>
#include <vector>

bool vertex_inorder(const dynamic_path_ops& tree_ops, TreeNode* root, const std::vector<int>& reference) {
    std::vector<int> vector_vertices;
    tree_ops.vectorize(root, vector_vertices);
    if (vector_vertices.size() != reference.size()) {
        return false;
    }
    for (std::size_t i = 0; i < vector_vertices.size(); ++i) {
        if (vector_vertices[i] != reference[i]) {
            return false;
        }
    }
    return true;
}

bool cost_inorder(const dynamic_path_ops& tree_ops, TreeNode* root, const std::vector<double>& reference) {
    std::vector<double> vector_path;
    tree_ops.vectorize(root, vector_path);
    if (vector_path.size() != reference.size()) {
        return false;
    }
    for (std::size_t i = 0; i < vector_path.size(); ++i) {
        if (vector_path[i] != reference[i]) {
            return false;
        }
    }
    return true;
}

void subpathAllCorrect(const dynamic_path_ops& tree_ops, TreeNode*& root, const std::vector<TreeNode*>& external_nodes,
                       const std::vector<double>& cost_reference, const std::vector<int>& index_reference) {
    assert(external_nodes.size() == index_reference.size());
    assert(external_nodes.size() == cost_reference.size() + 1);
    TreeNode* p;
    TreeNode* q;
    TreeNode* r;
    double cost, cost2;
    // Edge case: Singleton vertex
    for (std::size_t i = 0; i < index_reference.size(); ++i) {
        tree_ops.split_before(external_nodes[i], p, q, cost);
        tree_ops.split_after(external_nodes[i], q, r, cost2);
        assert(tree_ops.pmincost_before(q) == nullptr);
        assert(tree_ops.pmincost_after(q) == nullptr);
        q = tree_ops.concatenate(q, r, cost2);
        root = tree_ops.concatenate(p, q, cost);
        assert(vertex_inorder(tree_ops, root, index_reference));
        assert(cost_inorder(tree_ops, root, cost_reference));
    }

    // Check minimum matches for each segment.
    for (std::size_t st = 0; st < index_reference.size() - 1; ++st) {
        for (std::size_t ed = st + 1; ed < index_reference.size(); ++ed) {
            double local_min = cost_reference[st];
            for (std::size_t i = st + 1; i < ed; ++i) {
                local_min = std::min(local_min, cost_reference[i]);
            }
            tree_ops.split_before(external_nodes[st], p, q, cost);
            tree_ops.split_after(external_nodes[ed], q, r, cost2);
            assert(local_min == tree_ops.pcost_before(tree_ops.pmincost_before(q)));
            assert(local_min == tree_ops.pcost_after(tree_ops.pmincost_after(q)));
            q = tree_ops.concatenate(q, r, cost2);
            root = tree_ops.concatenate(p, q, cost);
            assert(vertex_inorder(tree_ops, root, index_reference));
            assert(cost_inorder(tree_ops, root, cost_reference));
        }
    }
}

void dynamic_path_unit_tests() {
    dynamic_path_ops tree_ops;
    // 20 edges with costs {0, 1, 2, ..., 19}
    std::size_t edge_num = 20;
    std::vector<double> original_array(edge_num, 0);
    std::vector<int> original_index_array(edge_num + 1, 0);
    TreeNode* root = tree_ops.gen_new_node(true, 0);
    TreeNode* p;
    std::vector<TreeNode*> external_nodes(edge_num + 1);
    external_nodes[0] = root;
    for (std::size_t i = 0; i < edge_num; ++i) {
        original_array[i] = i;
        original_index_array[i + 1] = static_cast<int>(i + 1);
        p = tree_ops.gen_new_node(true, static_cast<int>(i) + 1);
        external_nodes[i + 1] = p;
        root = tree_ops.concatenate(root, p, static_cast<double>(i));
        assert(!root->external);
    }

    // path
    for (const auto p: external_nodes) {
        assert(tree_ops.path(p) == root);
    }

    // head
    p = tree_ops.head(root);
    assert(p->external && p->node_index == 0);

    // tail
    p = tree_ops.tail(root);
    assert(p->external && p->node_index == edge_num);

    // before
    assert(tree_ops.before(external_nodes[0]) == nullptr);
    for (std::size_t i = 1; i < external_nodes.size(); ++i) {
        p = tree_ops.before(external_nodes[i]);
        assert(p->external && p->node_index == static_cast<int>(i) - 1);
    }

    // after
    assert(tree_ops.after(external_nodes[external_nodes.size() - 1]) == nullptr);
    for (std::size_t i = 0; i < external_nodes.size() - 1; ++i) {
        p = tree_ops.after(external_nodes[i]);
        assert(p->external && p->node_index == static_cast<int>(i) + 1);
    }

    // pcost_before
    assert(isnan(tree_ops.pcost_before(external_nodes[0])));
    for (std::size_t i = 1; i < external_nodes.size(); ++i) {
        assert(tree_ops.pcost_before(external_nodes[i]) == static_cast<double>(i - 1));
    }

    // pcost_after
    assert(isnan(tree_ops.pcost_after(external_nodes[external_nodes.size() - 1])));
    for (std::size_t i = 0; i < external_nodes.size() - 1; ++i) {
        assert(tree_ops.pcost_after(external_nodes[i]) == static_cast<double>(i));
    }

    // pmincost_before
    p = tree_ops.pmincost_before(root);
    assert(p->external && p->node_index == 1);

    // pmincost_after
    p = tree_ops.pmincost_after(root);
    assert(p->external && p->node_index == 0);

    TreeNode* q;
    double cost;
    // split_before + concatenate
    tree_ops.split_before(external_nodes[0], p, q, cost);
    assert(p == nullptr);
    assert(root == q);
    assert(isnan(cost));
    // Non-trivial cases
    for (std::size_t i = 1; i < external_nodes.size(); ++i) {
        tree_ops.split_before(external_nodes[i], p, q, cost);
        assert(cost == static_cast<double>(i - 1));
        std::vector<int> index_before(original_index_array.begin(), original_index_array.begin() + i);
        std::vector<int> index_after(original_index_array.begin() + i, original_index_array.end());
        std::vector<double> cost_before(original_array.begin(), original_array.begin() + (i - 1));
        std::vector<double> cost_after(original_array.begin() + i, original_array.end());
        assert(vertex_inorder(tree_ops, p, index_before));
        assert(vertex_inorder(tree_ops, q, index_after));
        assert(cost_inorder(tree_ops, p, cost_before));
        assert(cost_inorder(tree_ops, q, cost_after));
        root = tree_ops.concatenate(p, q, cost);
        assert(vertex_inorder(tree_ops, root, original_index_array));
        assert(cost_inorder(tree_ops, root, original_array));
    }

    // split_after + concatenate
    tree_ops.split_after(external_nodes[external_nodes.size() - 1], p, q, cost);
    assert(root == p);
    assert(q == nullptr);
    assert(isnan(cost));
    // Non-trivial cases
    for (std::size_t i = 0; i < external_nodes.size() - 1; ++i) {
        tree_ops.split_after(external_nodes[i], p, q, cost);
        assert(cost == static_cast<double>(i));
        std::vector<int> index_before(original_index_array.begin(), original_index_array.begin() + (i+1));
        std::vector<int> index_after(original_index_array.begin() + (i+1), original_index_array.end());
        std::vector<double> cost_before(original_array.begin(), original_array.begin() + i);
        std::vector<double> cost_after(original_array.begin() + (i + 1), original_array.end());
        assert(vertex_inorder(tree_ops, p, index_before));
        assert(vertex_inorder(tree_ops, q, index_after));
        assert(cost_inorder(tree_ops, p, cost_before));
        assert(cost_inorder(tree_ops, q, cost_after));
        root = tree_ops.concatenate(p, q, cost);
        assert(vertex_inorder(tree_ops, root, original_index_array));
        assert(cost_inorder(tree_ops, root, original_array));
    }

    // Comprehensive tests
    // Add +5 to sub-path (0, 15)
    for (std::size_t i = 0; i < 15; ++i) {
        original_array[i] += 5;
    }
    TreeNode* r;
    double cost2;
    tree_ops.split_before(external_nodes[0], p, q, cost);
    tree_ops.split_after(external_nodes[15], q, r, cost2);
    tree_ops.pupdate(q, 5);
    q = tree_ops.concatenate(q, r, cost2);
    root = tree_ops.concatenate(p, q, cost);
    assert(vertex_inorder(tree_ops, root, original_index_array));
    assert(cost_inorder(tree_ops, root, original_array));
    subpathAllCorrect(tree_ops, root, external_nodes, original_array, original_index_array);

    // Add -6 to sub-path (5, 20)
    for (std::size_t i = 5; i < original_array.size(); ++i) {
        original_array[i] -= 6;
    }
    tree_ops.split_before(external_nodes[5], p, q, cost);
    tree_ops.split_after(external_nodes[original_array.size()], q, r, cost2);
    tree_ops.pupdate(q, -6);
    q = tree_ops.concatenate(q, r, cost2);
    root = tree_ops.concatenate(p, q, cost);
    assert(vertex_inorder(tree_ops, root, original_index_array));
    assert(cost_inorder(tree_ops, root, original_array));
    subpathAllCorrect(tree_ops, root, external_nodes, original_array, original_index_array);

    // Add 1.5 to sub-path (6, 12)
    for (std::size_t i = 6; i < 12; ++i) {
        original_array[i] += 1.5;
    }
    tree_ops.split_before(external_nodes[6], p, q, cost);
    tree_ops.split_after(external_nodes[12], q, r, cost2);
    tree_ops.pupdate(q, 1.5);
    q = tree_ops.concatenate(q, r, cost2);
    root = tree_ops.concatenate(p, q, cost);
    assert(vertex_inorder(tree_ops, root, original_index_array));
    assert(cost_inorder(tree_ops, root, original_array));
    subpathAllCorrect(tree_ops, root, external_nodes, original_array, original_index_array);

    tree_ops.clearall(root);

    // Equal edge value segment case
    double same_cost = 3.14;
    for (std::size_t i = 6; i < 12; ++i) {
        original_array[i] = same_cost;
    }
    root = tree_ops.gen_new_node(true, 0);
    external_nodes[0] = root;
    for (std::size_t i = 0; i < edge_num; ++i) {
        p = tree_ops.gen_new_node(true, static_cast<int>(i) + 1);
        external_nodes[i + 1] = p;
        root = tree_ops.concatenate(root, p, original_array[i]);
        assert(!root->external);
    }

    tree_ops.split_before(external_nodes[6], p, q, cost);
    tree_ops.split_after(external_nodes[12], q, r, cost2);
    TreeNode* tmp = tree_ops.pmincost_before(q);
    assert(tmp->external && tmp->node_index == 7);
    assert(same_cost == tree_ops.pcost_before(tmp));
    tmp = tree_ops.pmincost_after(q);
    assert(tmp->external && tmp->node_index == 11);
    assert(same_cost == tree_ops.pcost_after(tmp));
    q = tree_ops.concatenate(q, r, cost2);
    root = tree_ops.concatenate(p, q, cost);
    assert(vertex_inorder(tree_ops, root, original_index_array));
    assert(cost_inorder(tree_ops, root, original_array));
    tree_ops.clearall(root);

    std::cout << "All unit tests of dynamic_path_ops passed!\n";
}

int main(int argc, const char * argv[]) {
    dynamic_path_unit_tests();
    // Experiment array: edges with costs: {1, 2, ..., 10}.
    std::vector<double> original_array;
    for (std::size_t i = 1; i <= 10; ++i) {
        original_array.push_back(static_cast<double>(i));
    }

    dp_array dynamic_array;
    dynamic_array.init_dynamic_path(original_array);
    dynamic_array.update_constant(2, 5, -3.5);

    std::vector<double> new_array;
    assert(dynamic_array.vectorize(new_array));

    assert(new_array.size() == original_array.size());
    for (std::size_t i = 0; i < original_array.size(); ++i) {
        if (i >= 2 && i < 5) {
            assert(original_array[i] - 3.5 == new_array[i]);
        } else  {
            assert(original_array[i] == new_array[i]);
        }
    }

    assert(dynamic_array.min_cost(0) == -0.5);
    assert(std::isnan(dynamic_array.min_cost(10)));
    assert(dynamic_array.min_cost(2, 5) == -0.5);
    assert(std::isnan(dynamic_array.min_cost(2, 2)));
    assert(dynamic_array.min_cost(4) == 1.5);
    assert(dynamic_array.min_cost(8) == 9);

    // Large scale data test.
    std::size_t maxNum = 100000000;
    std::cout << "Generate a randomly array of " << std::to_string(maxNum) << " elements ... \n";
    original_array.clear();
    auto rng = std::default_random_engine {};
    std::uniform_real_distribution<double> distribution(-1.0, 1.0);
    for (std::size_t i = 0; i < maxNum; ++i) {
        original_array.push_back(distribution(rng));
    }
    std::cout << "Random array generation complete ... \n";

    auto start = std::chrono::steady_clock::now();
    dynamic_array.init_dynamic_path(original_array);
    auto end = std::chrono::steady_clock::now();
    std::cout << "Initialize the corresponding dynamic path in time "
        << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
        << " ms.\n";

    // Find minimums of the (sub-)path.
    start = std::chrono::steady_clock::now();
    double min_cost = dynamic_array.min_cost(0);
    end = std::chrono::steady_clock::now();
    std::cout << "Find the minimum edge cost in the path in time "
        << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
        << " ms.\n";

    start = std::chrono::steady_clock::now();
    double min_cost_ref = original_array[0];
    for (std::size_t i = 0; i < original_array.size(); ++i) {
        if (original_array[i] < min_cost_ref) {
            min_cost_ref = original_array[i];
        }
    }
    end = std::chrono::steady_clock::now();
    std::cout << "Find the minimum edge cost in the original path in time "
        << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
        << " ms.\n";
    assert(fabs(min_cost_ref - min_cost) < 1e-6);

    start = std::chrono::steady_clock::now();
    min_cost = dynamic_array.min_cost(1000, static_cast<int>(maxNum / 2));
    end = std::chrono::steady_clock::now();
    std::cout << "Find the minimum edge cost in the sub-path (1000, " << std::to_string(maxNum / 2) << ") in time "
        << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
        << " ms.\n";

    start = std::chrono::steady_clock::now();
    min_cost_ref = original_array[1000];
    for (std::size_t i = 1000; i < maxNum / 2; ++i) {
        if (original_array[i] < min_cost_ref) {
            min_cost_ref = original_array[i];
        }
    }
    end = std::chrono::steady_clock::now();
    std::cout << "Find the minimum edge cost in the sub-path (1000, " << std::to_string(maxNum / 2) << ") of the original array in time "
        << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
        << " ms.\n";
    assert(fabs(min_cost - min_cost_ref) < 1e-6);

    // Update (sub-)path.
    start = std::chrono::steady_clock::now();
    dynamic_array.update_constant(100, static_cast<int>(maxNum / 2 + 1), 1.1);
    end = std::chrono::steady_clock::now();
    std::cout << "Add a constant to all edge cost in the sub-path (100, " << std::to_string(maxNum / 2 + 1) << ") in time "
        << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
        << " ms.\n";

    start = std::chrono::steady_clock::now();
    for (std::size_t i = 100; i < maxNum / 2 + 1; ++i) {
        original_array[i] += 1.1;
    }
    end = std::chrono::steady_clock::now();
    std::cout << "Add a constant to all edge cost in the sub-path (100, " << std::to_string(maxNum / 2 + 1) << ") of the original array in time "
        << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
        << " ms.\n";

    assert(dynamic_array.vectorize(new_array));
    assert(new_array.size() == original_array.size());
    min_cost = dynamic_array.min_cost(1000, static_cast<int>(maxNum / 2));
    assert(fabs(min_cost - (min_cost_ref + 1.1)) < 1e-6);
    for (std::size_t i = 0; i < new_array.size(); ++i) {
        assert(fabs(new_array[i] - original_array[i]) < 1e-6);
    }

    std::cout << "All tests passed!\n";

    return 0;
}
