#pragma once

#include <iostream>

#include <vector>
#include <set>
#include <cassert>
#include <numeric>

#include "tsp_solver.h"

using std::set;

class VertexUsed {
    size_t used_value_ = 0;
    vector<size_t> vertices_;
public:
    explicit VertexUsed(size_t vertices_count) : vertices_(vertices_count, 0) {}
    [[nodiscard]] inline bool is_used(vertex v) const {
        assert(0 <= v && v <= vertices_.size());
        return vertices_[v] == used_value_;
    }
    void set_used(vertex v) {
        vertices_[v] = used_value_;
    }
    void next_epoch() {
        ++used_value_;
    }
};


struct UsingEdgesGraph {
    vector<set<vertex>> graph;
private:
    [[nodiscard]] inline bool vertex_correct(const vertex v) const {
        return v >= 0 && v <= graph.size();
    }
public:
    void operator^=(const edge e) {
        assert(vertex_correct(e.first) && vertex_correct(e.second));
        auto it = graph[e.first].find(e.second);
        if (it != graph[e.first].end()) {
            graph[e.first].erase(it);
            graph[e.second].erase(e.first);
        }
        else {
            graph[e.first].insert(e.second);
            graph[e.second].insert(e.first);
        }
    }
};


class DFSByPass {
    const UsingEdgesGraph& A_;
    VertexUsed& vertex_used_;
    size_t vertices_in_component;
    bool was_cycle;
    bool correctness = true;

    void dfs(vertex v, vertex parent) {
        if (!correctness) {
            return;
        }
        vertex_used_.set_used(v);
        ++vertices_in_component;

        if (A_.graph[v].size() > 2) {
            correctness = false;
            return;
        }
        for (auto u : A_.graph[v]) {
            if (u == parent) {
                continue;
            }
            if (vertex_used_.is_used(u)) {
                was_cycle = true;
                continue;
            }
            dfs(u, v);
        }
    }
public:
    DFSByPass(const UsingEdgesGraph& A, VertexUsed& vertex_used) : A_(A),
                                                                   vertex_used_(vertex_used) {}

    pair<size_t, size_t> calculate_k_A_and_m_A() {
        vertex_used_.next_epoch();

        size_t k_A = 0;
        size_t m_A = 0;

        for (vertex v = 0; v < A_.graph.size(); ++v) {
            if (vertex_used_.is_used(v)) {
                continue;
            }
            vertices_in_component = 0;
            was_cycle = false;
            dfs(v, static_cast<vertex>(A_.graph.size()));
            if (!correctness) {
                return {A_.graph.size(), 0};
            }
            ++k_A;
            if (was_cycle) {
                m_A += vertices_in_component;
            }
        }
        return {k_A, m_A};
    }
};


size_t calculate_cost(const vector<vertex>& cycle, const vector<vector<bool>>& graph) {
    size_t result = 0;
    for (size_t i = 0; i < cycle.size(); ++i) {
        vertex start = cycle[i];
        vertex finish = cycle[(i + 1) % cycle.size()];
        result += 1 + !graph[start][finish];
    }
    return result;
}


class OriginalSolver : public TSPSolver {
    int K_;
    const vector<vector<vertex>>* graph_;
    UsingEdgesGraph A_;
    VertexUsed vertex_used_ = VertexUsed(0);

    size_t k_A_ = 0;
    size_t m_A_ = 0;

    void apply_C_to_A(const vector<edge>& C) {
        for (auto e : C) {
            A_ ^= e;
        }
    }

    bool update_A(const vector<edge>& C_with_repeats) {
        vector<edge> C = C_with_repeats;
        sort(C.begin(), C.end());
        C.erase(std::unique(C.begin(), C.end()), C.end());

        apply_C_to_A(C);

        auto [k_A, m_A] = DFSByPass(A_, vertex_used_).calculate_k_A_and_m_A();
        if (k_A <  k_A_ || k_A == k_A_ && m_A > m_A_) {
            k_A_ = k_A;
            m_A_ = m_A;
            return true;
        }
        apply_C_to_A(C);
        return false;
    }

    bool find_c(vector<edge>& C) {
        if (C.size() == K_) {
            return update_A(C);
        }
        for (vertex i = 0; i < graph_->size(); ++i) {
            for (auto u : (*graph_)[i]) {
                if (i >= u) {
                    continue;
                }
                C.emplace_back(i, u);
                if (find_c(C)) {
                    return true;
                }
                C.pop_back();
            }
        }
        return false;
    }
    void dfs(vertex v, vector<vertex>& result) {
        vertex_used_.set_used(v);
        result.push_back(v);
        for (auto u : A_.graph[v]) {
            if (!vertex_used_.is_used(u)) {
                dfs(u, result);
            }
        }
    }
    vector<vertex> calculate_result_by_A() {
        vector<vertex> result;
        vertex_used_.next_epoch();
        for (vertex i = 0; i < A_.graph.size(); ++i) {
            if (!vertex_used_.is_used(i) && A_.graph[i].size() == 1) {
                dfs(i, result);
            }
        }
        for (vertex i = 0; i < A_.graph.size(); ++i) {
            if (!vertex_used_.is_used(i)) {
                dfs(i, result);
            }
        }
        //A_.graph.clear();
        return result;
    }
public:
    explicit OriginalSolver(int k) : K_(k) {};

    pair<size_t, vector<vertex>> solve(const vector<vector<vertex>>& graph, const vector<vector<bool>>& adjacency_matrix) override {
        graph_ = &graph;
        vertex_used_ = VertexUsed(graph.size());
        A_.graph.clear(); // change later
        A_.graph.resize(graph.size());
        k_A_ = graph.size();
        m_A_ = 0;

        vector<edge> C;
        C.reserve(K_);

        size_t cost;
        size_t i = 0;
        while (find_c(C)) {
            C.clear();

            cost = calculate_cost(calculate_result_by_A(), adjacency_matrix);
            //std::cout << ++i << ' ' << cost << std::endl;
            if (cost * 7 <= graph.size() * 8) {
                break;
            }
        }
        auto result = calculate_result_by_A();

        return std::make_pair(calculate_cost(result, adjacency_matrix), result);
    }
};