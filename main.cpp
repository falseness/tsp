#include <iostream>

#include "solvers/original_solver.h"
#include "utils/random.h"



int main() {

    srand(0);
    OriginalSolver solver(5);
    /*auto result = solver.solve({{1}, {2}, {3}, {0}});
    for (auto u : result) {
        std::cout << u << ' ';
    }*/
    for (int cycle = 0; cycle < 1000; ++cycle) {
        auto adjacency_matrix = GenerateGraphWithHamiltonianCycle(100, 0.999);
        auto graph = from_adjacency_matrix(adjacency_matrix);
        std::cout << "graph size " << graph.size() << std::endl;
        auto result = solver.solve(graph, adjacency_matrix);
        size_t cost = calculate_cost(result.second, adjacency_matrix);
        if (cost * 7 > graph.size() * 8) {
            std::cout << "NEOK\n";
            for (size_t i = 0; i < graph.size(); ++i) {
                std::cout << i << ": ";
                for (auto u : graph[i]) {
                    std::cout << u << ' ';
                }
                std::cout << '\n';
            }
            std::cout << "output:";
            std::cout << result.first << " real cost:" << cost << '\n';
            for (auto u : result.second) {
                std::cout << u << ' ';
            }
            break;
        }
        std::cout << "test " << cycle << " length " << graph.size() << " ok" << std::endl;
    }

    return 0;
}
