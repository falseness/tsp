#include "random.h"

#include <cassert>
#include <numeric>


vector<vector<bool>> GenerateRandomGraph(size_t size, long double edge_chance) {
    assert (0 <= edge_chance && edge_chance <= 1.0);
    vector<vector<bool>> result(size, vector<bool>(size, false));

    for (size_t i = 0; i < size; ++i) {
        for (size_t j = i + 1; j < size; ++j) {
            result[i][j] = result[j][i] = GenerateRandomDouble(0.0, 1.0) <= edge_chance;
        }
    }
    return result;
}


vector<size_t> GenerateRandomPermutation(size_t size) {
    vector<size_t> result(size);
    std::iota(result.begin(), result.end(), 0);
    for (size_t i = 0; i + 1 < result.size(); ++i) {
        std::swap(result[i], result[GenerateRandInt(i + 1, result.size() - 1)]);
    }
    return result;
}


vector<vector<bool>> GenerateGraphWithHamiltonianCycle(size_t size, long double edge_chance) {
    assert(size > 0);

    auto graph = GenerateRandomGraph(size, edge_chance);

    auto permutation = GenerateRandomPermutation(size);
    for (size_t i = 0; i + 1 < size; ++i) {
        graph[permutation[i]][permutation[i + 1]] = graph[permutation[i + 1]][permutation[i]] = true;
    }
    graph[permutation[size - 1]][permutation[0]] = graph[permutation[0]][permutation[size - 1]] = true;
    return graph;
}