#include "solver.h"

#include "gmock/gmock-matchers.h"

#include "utils/graph.h"
#include "utils/random.h"
#include "solvers/original_solver.h"


using testing::AllOf;
using testing::Ge;
using testing::Le;


void OriginalSolverTests::SetUp() {}

void OriginalSolverTests::TearDown() {}

TEST_F(OriginalSolverTests, UnitTest1) {
    auto graph = FromEdgesList({{0, 1}, {1, 2}, {2, 0}});
    auto adjacency_matrix = ToAdjacencyMatrix(graph);
    auto result = OriginalSolver(5).Solve(graph, adjacency_matrix);

    EXPECT_EQ(result.size(), 3);
    EXPECT_EQ(set(result.begin(), result.end()), set<vertex>({0, 1, 2}));
}


bool is_cycle(const vector<vertex> cycle) {
    vector<uint8_t> used(cycle.size(), 0);
    for (auto u : cycle) {
        if (u < 0 || u >= cycle.size()) {
            return false;
        }
        used[u] = true;
    }
    for (auto value : used) {
        if (!value) {
            return false;
        }
    }
    return true;
}


void TestGraph(TSPSolver& solver, const vector<vector<vertex>>& graph, const vector<vector<bool>>& adjacency_matrix) {
    auto result = solver.Solve(graph, adjacency_matrix);

    EXPECT_EQ(graph.size(), result.size());
    EXPECT_TRUE(is_cycle(result));
}


TEST_F(OriginalSolverTests, SmallGraph) {
    auto solver = OriginalSolver(5);
    for (size_t i = 0; i < 50; ++i) {
        auto adjacency_matrix = GenerateRandomGraph(GenerateRandInt(1, 9), 0.5);
        auto graph = FromAdjacencyMatrix(adjacency_matrix);

        TestGraph(solver, graph, adjacency_matrix);
    }
}



TEST_F(OriginalSolverTests, HamiltonianCycleGraphs) {
    srand(0);
    auto solver = OriginalSolver(1);
    for (size_t i = 0; i < 200; ++i) {
        auto adjacency_matrix = GenerateGraphWithHamiltonianCycle(GenerateRandInt(10, 1000), 0.9);
        auto graph = FromAdjacencyMatrix(adjacency_matrix);

        TestGraph(solver, graph, adjacency_matrix);
    }
}