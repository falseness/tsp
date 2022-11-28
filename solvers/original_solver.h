#pragma once

#include <iostream>

#include <vector>
#include <set>
#include <cassert>
#include <numeric>

#include "tsp_solver.h"
#include "graph_characteristics_calculator.h"
#include "utils/graph.h"


class OriginalSolver : public TSPSolver {
protected:
    size_t K_;
    const vector<vector<vertex>>* graph_ = nullptr;
    UsingEdgesGraph A_;
    VertexUsed vertex_used_ = VertexUsed(0);

    size_t k_A_ = 0;
    size_t m_A_ = 0;

    bool TryUpdateA(const vector<Edge>& C_with_repeats);

    bool FindCAndUpdateA(vector<Edge>& C);
    void DFS(const UsingEdgesGraph& A, vertex v, vector<vertex>& result);
    vector<vertex> CalculateCycle(const UsingEdgesGraph& A);
    void Initialize(const vector<vector<vertex>>& graph);
public:
    explicit OriginalSolver(size_t k) : K_(k) {};

    vector<vertex> Solve(const vector<vector<vertex>>& graph,
                                       const vector<vector<bool>>& adjacency_matrix) override;
};
