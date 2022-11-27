#pragma once

#include <iostream>

#include <vector>
#include <set>
#include <cassert>
#include <numeric>

#include "tsp_solver.h"
#include "graph_characteristics_calculator.h"
#include "utils/graph.h"


class OriginalSolver : public TSPSolver {
    size_t K_;
    const vector<vector<vertex>>* graph_;
    UsingEdgesGraph A_;
    VertexUsed vertex_used_ = VertexUsed(0);

    size_t k_A_ = 0;
    size_t m_A_ = 0;

    void ApplyCToA(const vector<Edge>& C) {
        for (auto e : C) {
            A_ ^= e;
        }
    }

    bool TryUpdateA(const vector<Edge>& C_with_repeats) {
        vector<Edge> C = C_with_repeats;
        sort(C.begin(), C.end());
        C.erase(std::unique(C.begin(), C.end()), C.end());

        ApplyCToA(C);

        auto [k_A, m_A] = CalculateKAAndMA(A_, vertex_used_);
        if (k_A <  k_A_ || k_A == k_A_ && m_A > m_A_) {
            k_A_ = k_A;
            m_A_ = m_A;
            return true;
        }
        ApplyCToA(C);
        return false;
    }

    bool FindCAndUpdateA(vector<Edge>& C) {
        if (C.size() == K_) {
            return TryUpdateA(C);
        }
        for (vertex i = 0; i < graph_->size(); ++i) {
            for (auto u : (*graph_)[i]) {
                if (i >= u) {
                    continue;
                }
                C.emplace_back(i, u);
                if (FindCAndUpdateA(C)) {
                    return true;
                }
                C.pop_back();
            }
        }
        return false;
    }
    void DFS(vertex v, vector<vertex>& result) {
        vertex_used_.set_used(v);
        result.push_back(v);
        for (auto u : A_.graph[v]) {
            if (!vertex_used_.is_used(u)) {
                DFS(u, result);
            }
        }
    }
    vector<vertex> CalculateCycleByA() {
        vector<vertex> result;
        vertex_used_.next_epoch();
        for (vertex i = 0; i < A_.graph.size(); ++i) {
            if (!vertex_used_.is_used(i) && A_.graph[i].size() == 1) {
                DFS(i, result);
            }
        }
        for (vertex i = 0; i < A_.graph.size(); ++i) {
            if (!vertex_used_.is_used(i)) {
                DFS(i, result);
            }
        }
        return result;
    }
    void Initialize(const vector<vector<vertex>>& graph) {
        graph_ = &graph;
        vertex_used_ = VertexUsed(graph.size());
        A_.graph.resize(graph.size());
        k_A_ = graph.size();
        m_A_ = 0;
    }
public:
    explicit OriginalSolver(int k) : K_(k) {};

    pair<size_t, vector<vertex>> Solve(const vector<vector<vertex>>& graph,
                                       const vector<vector<bool>>& adjacency_matrix) override {
        Initialize(graph);

        vector<Edge> C;
        C.reserve(K_);

        while (FindCAndUpdateA(C)) {
            C.clear();

            size_t cost = CalculateCycleCost(CalculateCycleByA(), adjacency_matrix);
            //std::cout << ++i << ' ' << cost << std::endl;
            if (cost * 7 <= graph.size() * 8) {
                break;
            }
        }
        auto cycle = CalculateCycleByA();
        auto result = std::make_pair(CalculateCycleCost(cycle, adjacency_matrix), cycle);
        A_.graph.clear();
        vertex_used_.clear();
        return result;
    }
};