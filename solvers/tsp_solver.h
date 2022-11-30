#pragma once

#include <vector>
#include <fstream>

#include "utils/graph.h"


class TSPSolver {
public:
    virtual vector<vertex> Solve(const vector<vector<vertex>>& graph) = 0;
    virtual ~TSPSolver() = default;
};

class ApproximateSolver : public TSPSolver {
public:
    virtual vector<vertex> GetBestResult() = 0;
};
