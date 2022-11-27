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
    size_t K_;
    const vector<vector<vertex>>* graph_ = nullptr;
    UsingEdgesGraph A_;
    VertexUsed vertex_used_ = VertexUsed(0);

    size_t k_A_ = 0;
    size_t m_A_ = 0;

    void ApplyCToA(const vector<Edge>& C);

    bool TryUpdateA(const vector<Edge>& C_with_repeats);

    bool FindCAndUpdateA(vector<Edge>& C);
    void DFS(vertex v, vector<vertex>& result);
    vector<vertex> CalculateCycleByA();
    void Initialize(const vector<vector<vertex>>& graph);
public:
    explicit OriginalSolver(int k) : K_(k) {};

    pair<size_t, vector<vertex>> Solve(const vector<vector<vertex>>& graph,
                                       const vector<vector<bool>>& adjacency_matrix) override;
};
