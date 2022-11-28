#include "original_solver.h"

#include "utils/delete_repeats.h"


vector<vertex> OriginalSolver::Solve(const vector<vector<vertex>> &graph,
                                                   const vector<vector<bool>> &adjacency_matrix) {
    Initialize(graph);

    vector<Edge> C;
    C.reserve(K_);

    while (FindCAndUpdateA(C)) {
        C.clear();

        size_t cost = CalculateCycleCost(CalculateCycle(A_), adjacency_matrix);
        if (cost * 7 <= graph.size() * 8) {
            break;
        }
    }
    auto result = CalculateCycle(A_);
    A_.graph.clear();
    vertex_used_.clear();
    return result;
}

void OriginalSolver::Initialize(const vector<vector<vertex>> &graph)  {
    graph_ = &graph;
    vertex_used_ = VertexUsed(graph.size());
    A_.graph.resize(graph.size());
    k_A_ = graph.size();
    m_A_ = 0;
}

vector<vertex> OriginalSolver::CalculateCycle(const UsingEdgesGraph& A) {
    vector<vertex> result;
    vertex_used_.next_epoch();
    for (vertex i = 0; i < A.graph.size(); ++i) {
        if (!vertex_used_.is_used(i) && A.graph[i].size() == 1) {
            DFS(A, i, result);
        }
    }
    for (vertex i = 0; i < A.graph.size(); ++i) {
        if (!vertex_used_.is_used(i)) {
            DFS(A, i, result);
        }
    }
    return result;
}

void OriginalSolver::DFS(const UsingEdgesGraph& A, vertex v, vector<vertex> &result) {
    vertex_used_.set_used(v);
    result.push_back(v);
    for (auto u : A.graph[v]) {
        if (!vertex_used_.is_used(u)) {
            DFS(A, u, result);
        }
    }
}

bool OriginalSolver::FindCAndUpdateA(vector<Edge> &C) {
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

bool OriginalSolver::TryUpdateA(const vector<Edge>& C_with_repeats) {
    vector<Edge> C = DeleteRepetitions<Edge>(C_with_repeats);

    A_ ^= C;

    auto [k_A, m_A] = CalculateKAAndMA(A_, vertex_used_);
    if (k_A <  k_A_ || k_A == k_A_ && m_A > m_A_) {
        k_A_ = k_A;
        m_A_ = m_A;
        return true;
    }
    A_ ^= C;
    return false;
}
