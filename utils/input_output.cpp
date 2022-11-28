#include "input_output.h"

#include <iostream>
#include <unistd.h>

vector<vector<vertex>> InputGraph() {
    size_t n, m;
    std::cin >> n >> m;
    vector<vector<vertex>> result(n);

    for (size_t i = 0; i < m; ++i) {
        vertex vOne, vTwo;
        std::cin >> vOne >> vTwo;

        result[vOne].push_back(vTwo);
        result[vTwo].push_back(vOne);
    }
    return result;
}

void OutputGraph(const vector<vector<vertex>>& graph) {
    size_t two_times_edges_count = 0;
    for (const auto& adjacency_list : graph) {
        two_times_edges_count += adjacency_list.size();
    }
    assert(two_times_edges_count % 2 == 0);
    std::cout << graph.size() << ' ' << two_times_edges_count / 2 << '\n';
    for (size_t i = 0; i < graph.size(); ++i) {
        for (auto u : graph[i]) {
            assert(i != u);
            if (i < u) {
                std::cout << i << ' ' << u << '\n';
            }
        }
    }
}

void OutputResult(int cost) {
    std::cout << cost << ", ";
}


