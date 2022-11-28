#include "approximate_original_solver.h"

#include "utils/delete_repeats.h"

vector<vertex> ApproximateOriginalSolver::Solve(const vector<vector<vertex>> &graph,
                                                const vector<vector<bool>> &adjacency_matrix) {
    // разрешаем использовать только 1 раз.
    assert(graph_ == nullptr);

    Initialize(graph);
    A_backup_.graph.resize(graph.size());
    vector<Edge> C;
    C.reserve(K_);

    while (FindCAndUpdateA(C)) {

        A_version_ = 1;
        A_backup_ ^= DeleteRepetitions(C);
        A_version_ = 0;
        C.clear();
    }

    return CalculateCycle(A_);
}

vector<vertex> ApproximateOriginalSolver::GetBestResult() {
    if (A_version_ == 0) {
        return CalculateCycle(A_);
    }
    return CalculateCycle(A_backup_);
}
