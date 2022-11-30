#include "original_solver.h"


vector<vertex> OriginalSolver::Solve(const vector<vector<vertex>> &graph) {
    Initialize(graph);

    vector<Edge> C;
    C.reserve(K_);

    auto adjacency_matrix = ToAdjacencyMatrix(graph);

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

bool OriginalSolver::FindCAndUpdateA(vector<Edge>& C) {
    for (size_t K = 1; K <= K_; ++K) {
        if (FindCAndUpdateA(C, K)) {
            return true;
        }
    }
    return false;
}


bool OriginalSolver::FindCAndUpdateA(vector<Edge> &C, size_t K) {
    // инициализирует рекурсивно C, чтобы ребра шли по возрастанию (чтобы гарантировать не повторяемость)
    // причем внутри ребра vertex_one < vertex_two

    if (C.size() == K) {
        return TryUpdateA(C);
    }

    vertex previous_v_one = 0;
    vertex previous_v_two = 0;
    if (!C.empty()) {
        previous_v_one = C.back().vertex_one;
        previous_v_two = C.back().vertex_two;
    }
    // случай, когда первая вершина следующего ребра больше первой вершины предыдущего ребра
    for (vertex i = previous_v_one + 1; i < graph_->size(); ++i) {
        for (auto u : (*graph_)[i]) {
            if (i >= u) {
                continue;
            }
            C.emplace_back(i, u);
            if (FindCAndUpdateA(C, K)) {
                return true;
            }
            C.pop_back();
        }
    }
    // первая вершина равна вершине следующего ребра
    for (auto u : (*graph_)[previous_v_one]) {
        if (u <= previous_v_two) {
            continue;
        }
        C.emplace_back(previous_v_one, previous_v_two);
        if (FindCAndUpdateA(C, K)) {
            return true;
        }
        C.pop_back();
    }
    return false;
}

bool OriginalSolver::TryUpdateA(const vector<Edge>& C) {
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
