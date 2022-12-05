#include "random_union.h"

#include <list>

#include "utils/random.h"


using std::list;
using std::optional;


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
