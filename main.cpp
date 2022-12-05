#include <csignal>

#include <iostream>

#include <memory>
#include <unistd.h>

#include "solvers/approximate_original_solver.h"
#include "solvers/random_permutation.h"
#include "solvers/random_union.h"
#include "solvers/tree_max_path.h"
#include "utils/random.h"
#include "utils/graph.h"
#include "utils/input_output.h"


ApproximateSolver* solver = nullptr;
std::vector<vector<bool>> adjacency_matrix;


void signal_handler(int) {
    if (!solver || adjacency_matrix.empty()) {
        std::cout << "input is too long" << std::endl;
        //std::abort();
        _exit(0);
    }
    auto result = solver->GetBestResult();
    if (result.size() != adjacency_matrix.size()) {
        if (result.size() > adjacency_matrix.size()) {
            std::cout << "something went wrong"<< std::endl;
        }
        //assert(result.size() < adjacency_matrix.size());
        // solver не успел ничего посчитать
        OutputResult(adjacency_matrix.size() * 2);
    }
    else {
        OutputResult(CalculateCycleCost(result, adjacency_matrix));
    }
    _exit(0);
}


int main() {
    std::signal(SIGTERM, signal_handler);
    TreeMaxPath this_solver;
    solver = &this_solver;

    auto graph = InputGraph();
    adjacency_matrix = ToAdjacencyMatrix(graph);

    this_solver.Solve(graph);

    raise(SIGTERM);
    return 0;
}
