#include <cstdlib>
#include <string>


#include "utils/random.h"
#include "utils/graph.h"
#include "utils/input_output.h"


int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "incorrect input" << std::endl;
        std::abort();
    }
    size_t graph_size = std::stoll(std::string(argv[1]));

    if (graph_size == 0 || graph_size > static_cast<size_t>(1e5)) {
        std::cerr << "incorrect graph size" << std::endl;
        std::abort();
    }

    size_t seed = std::stoll(std::string(argv[2]));


    srand(seed);

    OutputGraph(FromAdjacencyMatrix(GenerateRandomGraph(graph_size, 0.5)));
}
