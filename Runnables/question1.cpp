#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include "./../Algorithms/Dijkstra/Dijkstra.h"

int main() {
    const int numPairs = 200;
    const int numVertices = 1070376;  // Number of vertices in the Florida graph
    std::vector<std::pair<int, int>> pairs(numPairs);

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);
    std::uniform_int_distribution<int> distribution(0, numVertices - 1);

    for (int i = 0; i < numPairs; ++i) {
        pairs[i] = std::make_pair(distribution(generator), distribution(generator));
    }

    Dijkstra<Graph> dijkstra(graph);
    auto start = std::chrono::high_resolution_clock::now();

    for (const auto& pair : pairs) {
        dijkstra.run(pair.first, pair.second);
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end - start;
    std::cout << "Total runtime: " << diff.count() << " seconds" << std::endl;

    return 0;
}