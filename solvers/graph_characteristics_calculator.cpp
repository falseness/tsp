#include "graph_characteristics_calculator.h"


class GraphCharacteristicsCalculator {
    const UsingEdgesGraph& A_;
    VertexUsed& vertex_used_;
    size_t vertices_in_component = 0;
    bool was_cycle = false;
    bool correctness = true;

    void DFS(vertex v, vertex parent);
public:
    GraphCharacteristicsCalculator(const UsingEdgesGraph& A, VertexUsed& vertex_used) : A_(A),
                                                                                        vertex_used_(vertex_used) {}

    pair<size_t, size_t> CalculateKAAndMA();
};


pair<size_t, size_t> GraphCharacteristicsCalculator::CalculateKAAndMA()  {
    vertex_used_.next_epoch();

    size_t k_A = 0;
    size_t m_A = 0;

    for (vertex v = 0; v < A_.graph.size(); ++v) {
        if (vertex_used_.is_used(v)) {
            continue;
        }
        vertices_in_component = 0;
        was_cycle = false;
        DFS(v, static_cast<vertex>(A_.graph.size()));
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


void GraphCharacteristicsCalculator::DFS(vertex v, vertex parent)  {
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
        DFS(u, v);
    }
}


pair<size_t, size_t> CalculateKAAndMA(const UsingEdgesGraph& A, VertexUsed& vertex_used)  {
   return GraphCharacteristicsCalculator(A, vertex_used).CalculateKAAndMA();
}