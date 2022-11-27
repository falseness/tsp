#pragma once

#include <tuple>
#include <vector>
#include <iostream>
#include <set>
#include <cassert>
#include <numeric>

using std::pair;
using std::vector;
using std::set;

typedef int vertex;


struct Edge {
    vertex vertexOne;
    vertex vertexTwo;
    Edge(vertex vertexOne, vertex vertexTwo);
};


inline bool operator<(const Edge& first, const Edge& second) {
    return first.vertexOne < second.vertexOne ||
            (first.vertexOne == second.vertexOne && first.vertexTwo < second.vertexTwo);
}

inline bool operator==(const Edge& first, const Edge& second) {
    return (first.vertexOne == second.vertexOne && first.vertexTwo == second.vertexTwo) ||
            (first.vertexOne == second.vertexTwo && first.vertexTwo == second.vertexOne);
}


struct UsingEdgesGraph {
    vector<set<vertex>> graph;
private:
    [[nodiscard]] inline bool vertex_correct(const vertex v) const {
        return v >= 0 && v <= graph.size();
    }
public:
    void operator^=(const Edge& e);
};


class VertexUsed {
    size_t used_value_ = 0;
    vector<size_t> vertices_;
public:
    explicit VertexUsed(size_t vertices_count) : vertices_(vertices_count, 0) {}
    [[nodiscard]] inline bool is_used(vertex v) const {
        assert(0 <= v && v <= vertices_.size());
        return vertices_[v] == used_value_;
    }
    inline void set_used(vertex v) {
        vertices_[v] = used_value_;
    }
    inline void next_epoch() {
        ++used_value_;
    }

    inline void clear() {
        vertices_.clear();
    }
};


vector<vector<vertex>> FromAdjacencyMatrix(const vector<vector<bool>>& graph);
vector<vector<vertex>> FromEdgesList(const vector<Edge>& Edges_list);
vector<vector<bool>> ToAdjacencyMatrix(const vector<vector<vertex>>& graph);

size_t CalculateCycleCost(const std::vector<vertex>& cycle, const std::vector<vector<bool>>& graph);
