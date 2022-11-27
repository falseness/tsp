#pragma once

#include <vector>

#include "utils/graph.h"


class TSPSolver {
public:
    virtual pair<size_t, vector<vertex>> Solve(const vector<vector<vertex>>& graph,
                                               const vector<vector<bool>>& adjacency_matrix) = 0;
};