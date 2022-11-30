#pragma once

#include <csignal>

#include "solvers/original_solver.h"


class ApproximateOriginalSolver : public OriginalSolver, public ApproximateSolver {
    // Поскольку signal может прийти в любой момент, нужно хранить еще одну версию A.
    UsingEdgesGraph A_backup_;
    static const std::sig_atomic_t A_backup_version_ = 0;
    static const std::sig_atomic_t A_main_version_ = 1;
    // Номер версии A, которая гарантированно корректная.
    volatile std::sig_atomic_t A_correct_version_ = A_backup_version_;
public:
    explicit ApproximateOriginalSolver(size_t K) : OriginalSolver(K) {};
    vector<vertex> Solve(const vector<vector<vertex>> &graph) override;
    vector<vertex> GetBestResult() override;
};