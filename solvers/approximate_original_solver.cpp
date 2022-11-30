#include "approximate_original_solver.h"

#include "utils/delete_repeats.h"

vector<vertex> ApproximateOriginalSolver::Solve(const vector<vector<vertex>> &graph) {
    // разрешаем использовать только 1 раз.
    assert(graph_ == nullptr);

    Initialize(graph);
    A_backup_.graph.resize(graph.size());
    vector<Edge> C;
    C.reserve(K_);

    while (FindCAndUpdateA(C)) {
        A_correct_version_ = A_main_version_;
        A_backup_ ^= DeleteRepetitions(C);
        A_correct_version_ = A_backup_version_;
        C.clear();
    }

    return CalculateCycle(A_);
}

vector<vertex> ApproximateOriginalSolver::GetBestResult() {
    if (A_correct_version_ == A_backup_version_) {
        return CalculateCycle(A_backup_);
    }
    return CalculateCycle(A_);
}
