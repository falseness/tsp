#include <csignal>

#include <iostream>

#include <memory>

#include "solvers/approximate_original_solver.h"
#include "utils/random.h"
#include "utils/graph.h"
#include "utils/input_output.h"


std::unique_ptr<ApproximateSolver> solver;
std::vector<vector<bool>> adjacency_matrix;


void signal_handler(int) {
    if (!solver || adjacency_matrix.empty()) {
        std::abort();
    }
    auto result = solver->GetBestResult();
    if (result.size() != adjacency_matrix.size()) {
        assert(result.size() < adjacency_matrix.size());
        // solver не успел ничего посчитать
        OutputResult(adjacency_matrix.size() * 2);
    }
    OutputResult(CalculateCycleCost(result, adjacency_matrix));
    exit(0);
}


int main() {
    std::signal(SIGTERM, signal_handler);
    solver = std::make_unique<ApproximateOriginalSolver>(10);

    auto graph = InputGraph();

    adjacency_matrix = ToAdjacencyMatrix(graph);

    solver->Solve(graph, adjacency_matrix);

    raise(SIGTERM);

    return 0;
}
