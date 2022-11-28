#pragma once

#include <vector>
#include <fstream>

#include "utils/graph.h"


class TSPSolver {
public:
    virtual vector<vertex> Solve(const vector<vector<vertex>>& graph,
                                               const vector<vector<bool>>& adjacency_matrix) = 0;
    virtual ~TSPSolver() = default;
};

class ApproximateSolver : public TSPSolver {
public:
    virtual vector<vertex> GetBestResult() = 0;
};
