#include "random_permutation.h"

#include "utils/random.h"

vector<vertex> RandomPermutationSolver::Solve(const vector<vector<vertex>> &graph) {
    backup_result_.resize(graph.size());

    std::iota(backup_result_.begin(), backup_result_.end(), 0);
    current_permutation_of_random_permutation_ = backup_result_;
    random_permutation_ = GenerateRandomPermutation(graph.size());

    auto adjacency_matrix = ToAdjacencyMatrix(graph);

    size_t best_cost = CalculateCycleCost(current_permutation_of_random_permutation_, adjacency_matrix);

    do {
        auto result = GetCycleByPermutationOfPermutation();
        size_t new_cost = CalculateCycleCost(result, adjacency_matrix);
        if (new_cost >= best_cost) {
            continue;
        }
        best_cost = new_cost;
        result_correct_version_ = result_current_version_;
        backup_result_ = result;
        result_correct_version_ = result_backup_version_;
    } while (std::next_permutation(current_permutation_of_random_permutation_.begin(),
                                   current_permutation_of_random_permutation_.end()));


    return GetCycleByPermutationOfPermutation();
}

vector<vertex> RandomPermutationSolver::GetBestResult() {
    if (result_correct_version_ == result_backup_version_) {
        return backup_result_;
    }
    return GetCycleByPermutationOfPermutation();
}

vector<vertex> RandomPermutationSolver::GetCycleByPermutationOfPermutation() {
    vector<vertex> result(random_permutation_.size());
    for (size_t i = 0; i < current_permutation_of_random_permutation_.size(); ++i) {
        result[current_permutation_of_random_permutation_[i]] = random_permutation_[i];
    }

    return result;
}
