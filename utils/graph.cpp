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
        assert (u.vertex_one >= 0 && u.vertex_two >= 0);
        size = max(size, u.vertex_one);
        size = max(size, u.vertex_two);
    }
    vector<vector<vertex>> result(size + 1);
    for (auto u : edges_list) {
        result[u.vertex_one].push_back(u.vertex_two);
        result[u.vertex_two].push_back(u.vertex_one);
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


Edge::Edge(vertex vertexOne, vertex vertexTwo) : vertex_one(vertexOne), vertex_two(vertexTwo) {}


void UsingEdgesGraph::operator^=(const Edge& e) {
    assert(vertex_correct(e.vertex_one) && vertex_correct(e.vertex_two));
    auto it = graph[e.vertex_one].find(e.vertex_two);
    if (it != graph[e.vertex_one].end()) {
        graph[e.vertex_one].erase(it);
        graph[e.vertex_two].erase(e.vertex_one);
    }
    else {
        graph[e.vertex_one].insert(e.vertex_two);
        graph[e.vertex_two].insert(e.vertex_one);
    }
}


void UsingEdgesGraph::operator^=(const vector<Edge>& edges) {
    for (auto e : edges) {
        (*this) ^= e;
    }
}


[[nodiscard]] size_t CalculateCycleCost(const std::vector<vertex>& cycle, const std::vector<vector<bool>>& graph) {
    size_t result = 0;
    for (size_t i = 0; i < cycle.size(); ++i) {
        vertex start = cycle[i];
        vertex finish = cycle[(i + 1) % cycle.size()];
        result += 1 + !graph[start][finish];
    }
    return result;
}


void MaxPathInTreeCalculator::DFS(vertex v, vertex parent, size_t depth) {
    if (depth > max_depth_) {
        max_depth_ = depth;
        max_depth_vertex_ =  v;
    }
    parent_[v] = parent;
    for (auto u : graph_[v]) {
        if (u != parent) {
            DFS(u, v, depth + 1);
        }
    }
}

vector<vertex> MaxPathInTreeCalculator::Calculate() {
    // разрешаем использование только 1 раз
    assert(!max_depth_);
    DFS(root_, root_parent_, 1);
    // использование в этой функции push_back может привести к ошибкам при использовании сигналов (почему-то).
    vector<vertex> max_path(max_depth_);

    vertex current = max_depth_vertex_;

    for (size_t i = 0; current != root_parent_; ++i) {
        max_path[i] = current;
        current = parent_[current];
    }
    return max_path;
}
