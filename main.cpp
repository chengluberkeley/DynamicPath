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

int main(int argc, const char * argv[]) {
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
    std::size_t maxNum = 500000000;
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
