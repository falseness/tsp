#include "tree_max_path.h"

#include "utils/random.h"


vector<vertex> TreeMaxPath::FindRandomResult(const vector<vector<vertex>> &original_graph) {
    paths_.clear();
    vertex_in_path_.next_epoch();
    auto graph = original_graph;
    auto permutation = GenerateRandomPermutation(graph.size());

    for (auto u : permutation) {
        if (vertex_in_path_.is_used(static_cast<vertex>(u))) {
            continue;
        }
        size_t max_depth = 0;
        vertex max_depth_leaf;
        vector<vector<vertex>> tree_graph(graph.size());
        dfs_used_.next_epoch();
        CreateDFSTree(static_cast<vertex>(u), 1, max_depth, max_depth_leaf, graph, tree_graph);
        auto new_path = MaxPathInTreeCalculator(max_depth_leaf, tree_graph).Calculate();
        for (auto vertex_from_path : new_path) {
            vertex_in_path_.set_used(vertex_from_path);
        }
        paths_.push_back(std::move(new_path));

    }

    return CalculateResult(paths_);
}

void TreeMaxPath::CreateDFSTree(vertex v, size_t depth, size_t& max_depth, vertex& max_depth_leaf,
                                vector<vector<vertex>> &graph, vector<vector<vertex>>& tree_graph) {
    if (depth > max_depth) {
        max_depth = depth;
        max_depth_leaf = v;
    }
    dfs_used_.set_used(v);

    for (size_t i = 0; i < graph[v].size(); ++i) {
        auto u = graph[v][i];
        if (vertex_in_path_.is_used(u)) {
            std::swap(graph[v][i], graph[v][graph[v].size() - 1]);
            graph[v].pop_back();
            --i;
            continue;
        }
        if (dfs_used_.is_used(u)) {
            continue;
        }
        tree_graph[v].push_back(u);
        tree_graph[u].push_back(v);
        CreateDFSTree(u, depth + 1, max_depth, max_depth_leaf, graph, tree_graph);
    }
}

vector<vertex> TreeMaxPath::Solve(const vector<vector<vertex>> &graph) {
    dfs_used_ = VertexUsed(graph.size());
    vertex_in_path_ = VertexUsed(graph.size());
    return PathSolver::Solve(graph);
}
