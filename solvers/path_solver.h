#pragma once

#include <csignal>

#include "solvers/tsp_solver.h"


template <template <typename, typename> typename PathContainer>
class PathSolver : public ApproximateSolver {
    static const std::sig_atomic_t result_backup_version_ = 0;
    static const std::sig_atomic_t result_current_version_ = 1;
    volatile std::sig_atomic_t result_correct_version_ = result_backup_version_;
    vector<vertex> backup_result_;
    vector<vertex> current_result_;
protected:
    vector<PathContainer<vertex, std::allocator<vertex>>> paths_;
    static vector<vertex> CalculateResult(const vector<PathContainer<vertex, std::allocator<vertex>>>& paths);
    virtual vector<vertex> FindRandomResult(const vector<vector<vertex>>& graph) = 0;
public:
    vector<vertex> Solve(const vector<vector<vertex>>& graph) override;
    vector<vertex> GetBestResult() override;
};


template <template <typename, typename> class PathContainer>
vector<vertex> PathSolver<PathContainer>::Solve(const vector<vector<vertex>> &graph) {
    paths_.resize(graph.size());

    backup_result_.resize(graph.size());
    std::iota(backup_result_.begin(), backup_result_.end(), 0);
    current_result_ = backup_result_;
    auto adjacency_matrix = ToAdjacencyMatrix(graph);
    size_t best_cost = CalculateCycleCost(backup_result_, adjacency_matrix);
    while (best_cost != graph.size()) {
        current_result_ = FindRandomResult(graph);
        size_t new_cost = CalculateCycleCost(current_result_, adjacency_matrix);

        if (new_cost >= best_cost) {
            continue;
        }
        result_correct_version_ = result_current_version_;
        backup_result_ = current_result_;
        result_correct_version_ = result_backup_version_;
        best_cost = new_cost;
    }

    return backup_result_;
}


template <template <typename, typename> class PathContainer>
vector<vertex> PathSolver<PathContainer>::GetBestResult() {
    if (result_correct_version_ == result_backup_version_) {
        return backup_result_;
    }
    return current_result_;
}


template <template <typename, typename> class PathContainer>
vector<vertex> PathSolver<PathContainer>::CalculateResult(const vector<PathContainer<vertex, std::allocator<vertex>>>
        &paths) {
    vector<vertex> result;
    result.reserve(paths.size());

    for (const auto& path : paths) {
        for (auto u : path) {
            result.push_back(u);
        }
    }

    return result;
}
