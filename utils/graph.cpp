#include "graph.h"

#include <cassert>


using std::max;


vector<vector<vertex>> FromAdjacencyMatrix(const vector<vector<bool>>& graph) {
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


vector<vector<vertex>> FromEdgesList(const vector<Edge>& edges_list) {
    int size = 0;
    for (auto u : edges_list) {
        assert (u.vertexOne >= 0 && u.vertexTwo >= 0);
        size = max(size, u.vertexOne);
        size = max(size, u.vertexTwo);
    }
    vector<vector<vertex>> result(size + 1);
    for (auto u : edges_list) {
        result[u.vertexOne].push_back(u.vertexTwo);
        result[u.vertexTwo].push_back(u.vertexOne);
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


Edge::Edge(vertex vertexOne, vertex vertexTwo) : vertexOne(vertexOne), vertexTwo(vertexTwo) {}


void UsingEdgesGraph::operator^=(const Edge& e) {
    assert(vertex_correct(e.vertexOne) && vertex_correct(e.vertexTwo));
    auto it = graph[e.vertexOne].find(e.vertexTwo);
    if (it != graph[e.vertexOne].end()) {
        graph[e.vertexOne].erase(it);
        graph[e.vertexTwo].erase(e.vertexOne);
    }
    else {
        graph[e.vertexOne].insert(e.vertexTwo);
        graph[e.vertexTwo].insert(e.vertexOne);
    }
}


size_t CalculateCycleCost(const std::vector<vertex>& cycle, const std::vector<vector<bool>>& graph) {
    size_t result = 0;
    for (size_t i = 0; i < cycle.size(); ++i) {
        vertex start = cycle[i];
        vertex finish = cycle[(i + 1) % cycle.size()];
        result += 1 + !graph[start][finish];
    }
    return result;
}
