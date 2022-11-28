#pragma once

#include <csignal>

#include "solvers/original_solver.h"


class ApproximateOriginalSolver : public OriginalSolver, public ApproximateSolver {
    // Поскольку signal может прийти в любой момент, нужно хранить еще одну версию A.
    UsingEdgesGraph A_backup_;
    // Указатель на версию A, которая гарантированно корректная.
    std::sig_atomic_t A_version_ = 0;
public:
    explicit ApproximateOriginalSolver(size_t K) : OriginalSolver(K) {};
    vector<vertex> Solve(const vector<vector<vertex>> &graph,
                                                       const vector<vector<bool>> &adjacency_matrix) override;
    vector<vertex> GetBestResult() override;
};