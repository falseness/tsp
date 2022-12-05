#pragma once

#include "solvers/tsp_solver.h"

#include <csignal>
#include <list>


class RandomUnionSolver : public ApproximateSolver {
    static const std::sig_atomic_t result_backup_version_ = 0;
    static const std::sig_atomic_t result_current_version_ = 1;
    volatile std::sig_atomic_t result_correct_version_ = result_backup_version_;
    vector<vertex> backup_result_;
    vector<vertex> current_result_;
    vector<std::list<vertex>> paths_;

    static vector<vertex> CalculateResult(const vector<std::list<vertex>>& paths);
    static void UnionPaths(vector<std::list<vertex>*> &get_list_by_boundary_element, size_t v, vertex u);
    vector<vertex> FindRandomResult(const vector<vector<vertex>>& graph);
public:
    vector<vertex> Solve(const vector<vector<vertex>>& graph) override;
    vector<vertex> GetBestResult() override;
};