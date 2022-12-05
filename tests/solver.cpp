#include "solver.h"

#include "gmock/gmock-matchers.h"

#include "utils/graph.h"
#include "utils/random.h"
#include "solvers/original_solver.h"
#include "solvers/random_permutation.h"
#include "solvers/random_union.h"
#include "solvers/tree_max_path.h"


using testing::AllOf;
using testing::Ge;
using testing::Le;
using std::unique_ptr;
using std::make_unique;


void OriginalSolverTests::SetUp() {}

void OriginalSolverTests::TearDown() {}

void UnitTest1(TSPSolver& solver) {
    auto graph = FromEdgesList({{0, 1}, {1, 2}, {2, 0}});

    auto result = solver.Solve(graph);

    EXPECT_EQ(result.size(), 3);
    EXPECT_EQ(set(result.begin(), result.end()), set<vertex>({0, 1, 2}));
}

TEST_F(OriginalSolverTests, UnitTest1) {
    vector<unique_ptr<TSPSolver>> solvers;
    solvers.push_back(make_unique<OriginalSolver>(5));
    solvers.push_back(make_unique<RandomPermutationSolver>());
    solvers.push_back(make_unique<RandomUnionSolver>());
    solvers.push_back(make_unique<TreeMaxPath>());

    for (auto& solver : solvers) {
        UnitTest1(*solver);
    }
}


bool IsCycle(const vector<vertex>& cycle) {
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


void TestGraph(TSPSolver& solver, const vector<vector<vertex>>& graph) {
    auto result = solver.Solve(graph);

    EXPECT_EQ(graph.size(), result.size());
    EXPECT_TRUE(IsCycle(result));
}


void SmallGraphTest(TSPSolver& solver) {
    for (size_t i = 0; i < 50; ++i) {
        auto adjacency_matrix = GenerateRandomGraph(GenerateRandInt(1, 9), 0.5);
        auto graph = FromAdjacencyMatrix(adjacency_matrix);

        TestGraph(solver, graph);
    }
}


TEST_F(OriginalSolverTests, SmallGraph) {
    auto solver = OriginalSolver(5);
    SmallGraphTest(solver);
    auto random_permutation_solver = RandomPermutationSolver();
    SmallGraphTest(random_permutation_solver);
}



TEST_F(OriginalSolverTests, HamiltonianCycleGraphs) {
    srand(0);
    auto solver = OriginalSolver(1);
    for (size_t i = 0; i < 200; ++i) {
        auto adjacency_matrix = GenerateGraphWithHamiltonianCycle(GenerateRandInt(10, 1000), 0.9);
        auto graph = FromAdjacencyMatrix(adjacency_matrix);

        TestGraph(solver, graph);
    }
}