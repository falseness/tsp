#include "random_union.h"

#include <list>

#include "utils/random.h"


using std::list;
using std::optional;


vector<vertex> RandomUnionSolver::Solve(const vector<vector<vertex>> &graph) {
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

void RandomUnionSolver::UnionPaths(vector<list<vertex>*> &get_list_by_boundary_element, size_t v, vertex u) {
    auto it = get_list_by_boundary_element[u]->begin();

    if ((*it) == u) {
        it = get_list_by_boundary_element[u]->end();
        --it;
    }
    vertex another_boundary = *it;
    bool one_vertex_list = get_list_by_boundary_element[v]->size() == 1;

    get_list_by_boundary_element[v]->splice(get_list_by_boundary_element[v]->end(),
                                            std::move(*get_list_by_boundary_element[u]));
    get_list_by_boundary_element[u] = nullptr;
    get_list_by_boundary_element[another_boundary] = get_list_by_boundary_element[v];
    if (!one_vertex_list) {
        get_list_by_boundary_element[v] = nullptr;
    }
}

vector<vertex> RandomUnionSolver::GetBestResult() {
    if (result_correct_version_ == result_backup_version_) {
        return backup_result_;
    }
    return current_result_;
}


vector<vertex> RandomUnionSolver::CalculateResult(const vector<std::list<vertex>> &paths) {
    vector<vertex> result;
    result.reserve(paths.size());

    for (const auto& path : paths) {
        for (auto u : path) {
            result.push_back(u);
        }
    }

    return result;
}

vector<vertex> RandomUnionSolver::FindRandomResult(const vector<vector<vertex>> &graph) {
    auto permutation = GenerateRandomPermutation(graph.size());

    vector<list<vertex>*> get_list_by_boundary_element;
    get_list_by_boundary_element.reserve(graph.size());

    for (vertex i = 0; i < graph.size(); ++i) {
        paths_[i] = list({i});
        get_list_by_boundary_element.push_back(&paths_[i]);
    }

    for (auto v : permutation) {
        if (!get_list_by_boundary_element[v]) {
            continue;
        }
        for (auto u : graph[v]) {
            if (!get_list_by_boundary_element[u]) {
                continue;
            }

            UnionPaths(get_list_by_boundary_element, v, u);

            break;
        }
    }
    return CalculateResult(paths_);
}
