#include "graph.h"

#include <cassert>

using std::max;

vector<vector<vertex>> from_adjacency_matrix(const vector<vector<bool>>& graph) {
    vector<vector<vertex>> result(graph.size());
    for (vertex i = 0; i < graph.size(); ++i) {
        for (vertex j = 0; j < graph.size(); ++j) {
            if (graph[i][j]) {
                result[i].push_back(j);
            }
        }
    }
    return result;
}

vector<vector<vertex>> FromEdgesList(const vector<edge>& edges_list) {
    int size = 0;
    for (auto u : edges_list) {
        assert (u.first >= 0 && u.second >= 0);
        size = max(size, u.first);
        size = max(size, u.second);
    }
    vector<vector<vertex>> result(size + 1);
    for (auto u : edges_list) {
        result[u.first].push_back(u.second);
        result[u.second].push_back(u.first);
    }
    return result;
}

vector<vector<bool>> ToAdjacencyMatrix(const vector<vector<vertex>>& graph) {
    vector<vector<bool>> result(graph.size(), vector<bool>(graph.size(), false));
    for (size_t v = 0; v < graph.size(); ++v) {
        for (auto u : graph[v]) {
            assert(u >= 0);

            result[u][v] = result[v][u] = true;
        }
    }
    return result;
}