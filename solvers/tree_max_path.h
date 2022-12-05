#pragma once

#include <csignal>

#include "solvers/path_solver.h"


class TreeMaxPath : public PathSolver<std::vector>  {
    VertexUsed dfs_used_ = VertexUsed(0);
    VertexUsed vertex_in_path_ = VertexUsed(0);
    void CreateDFSTree(vertex v, size_t depth, size_t& max_depth, vertex& max_depth_leaf,
                       vector<vector<vertex>>& graph, vector<vector<vertex>>& tree_graph);

    vector<vertex> FindRandomResult(const vector<vector<vertex>>& graph) override;
public:
    vector<vertex> Solve(const vector<vector<vertex>>& graph) override;
};
