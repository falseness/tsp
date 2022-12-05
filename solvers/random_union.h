#pragma once

#include <csignal>
#include <list>

#include "solvers/path_solver.h"


class RandomUnionSolver : public PathSolver<std::list> {
    static void UnionPaths(vector<std::list<vertex>*> &get_list_by_boundary_element, size_t v, vertex u);
    vector<vertex> FindRandomResult(const vector<vector<vertex>>& graph) override;
};