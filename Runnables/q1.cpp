#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include "./../Runnables/Commands/NetworkIO.h"
#include "./../DataStructures/Graph/Graph.h"
#include "./../Algorithms/Dijkstra/Dijkstra.h"

int main() {
    const int numPairs = 200;
    const int numVertices = 1070376;
    std::vector<std::pair<int, int>> pairs(numPairs);

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);
    std::uniform_int_distribution<int> distribution(0, numVertices - 1);

    for (int i = 0; i < numPairs; ++i) {
        pairs[i] = std::make_pair(distribution(generator), distribution(generator));
    }

    // Load the DIMACS graph and convert it to the transfer graph format
    LoadDimacsGraph command(shell);
    command.setParameter("Input file", "./../Networks/Florida/OSM/dimacs/USA-road-d.FLA.gr");
    command.setParameter("Output file", "Networks/Florida/OSM/BINARY/USA-road-d.FLA.bin");
    command.setParameter("Graph type", "dynamic");
    command.setParameter("Coordinate factor", "0.000001");
    command.execute();

    // Load the transfer graph data from the output file
    TransferGraph transferGraph;
    transferGraph.readBinary("Networks/Florida/OSM/BINARY/USA-road-d.FLA.bin");

    // Create a Dijkstra object and run the algorithm on the transfer graph
    Dijkstra<TransferGraph> dijkstra(transferGraph);
    auto start = std::chrono::high_resolution_clock::now();

    for (const auto& pair : pairs) {
        dijkstra.run(pair.first, pair.second);
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end - start;
    std::cout << "Total runtime in seconds for 200 random Dijkstra: " << diff.count() << std::endl;

    return 0;
}