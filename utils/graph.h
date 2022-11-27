#pragma once

#include <tuple>
#include <vector>

using std::pair;
using std::vector;

typedef int vertex;

typedef pair<vertex, vertex> edge;


vector<vector<vertex>> from_adjacency_matrix(const vector<vector<bool>>& graph);
vector<vector<vertex>> FromEdgesList(const vector<edge>& edges_list);
vector<vector<bool>> ToAdjacencyMatrix(const vector<vector<vertex>>& graph);
