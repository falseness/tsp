#pragma once

#include <cstdlib>
#include <vector>

#include <concepts>


using std::vector;

template <std::integral T>
inline int GenerateRandInt(T minimum, T maximum) {
    return minimum + rand() % (maximum - minimum + 1);
}


template <std::floating_point T>
inline T GenerateRandomDouble(T minimum, T maximum) {
    T rand_result = (long double)rand() / RAND_MAX;
    return minimum + rand_result * (maximum - minimum);
}


vector<vector<bool>> GenerateRandomGraph(size_t size, long double edge_chance);


vector<size_t> GenerateRandomPermutation(size_t size);


vector<vector<bool>> GenerateGraphWithHamiltonianCycle(size_t size, long double edge_chance);
