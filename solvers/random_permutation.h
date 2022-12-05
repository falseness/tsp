#pragma once

#include "solvers/tsp_solver.h"

#include <csignal>

class RandomPermutationSolver : public ApproximateSolver {
    static const std::sig_atomic_t result_backup_version_ = 0;
    static const std::sig_atomic_t result_current_version_ = 1;
    volatile std::sig_atomic_t result_correct_version_ = result_backup_version_;
    vector<size_t> random_permutation_;
    vector<vertex> backup_result_;
    vector<vertex> current_permutation_of_random_permutation_;


    vector<vertex> GetCycleByPermutationOfPermutation();
public:
    vector<vertex> Solve(const vector<vector<vertex>>& graph) override;
    vector<vertex> GetBestResult() override;
};