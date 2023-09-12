#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <chrono>
#include "../Algorithms/Dijkstra/Dijkstra.h"
#include "./../Runnables/Commands/NetworkIO.h"
#include "./../DataStructures/Graph/Graph.h"
#include "./../Shell/Shell.h"

using namespace Shell;

template<typename GRAPH, bool DEBUG = false>
class DijkstraTester {
public:
    void runTests() {
        auto startTime = std::chrono::high_resolution_clock::now();
        
        GRAPH graph;
        Dijkstra<GRAPH, false> dijkstra(graph);
        
        std::vector<Input> inputs = readInputFile("./../Networks/Florida/OSM/dimacs/USA-road-d.FLA.gr");
        
        for (const auto& input : inputs) {
            executeTest(dijkstra, input);
        }
        
        auto endTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(endTime - startTime);
        std::cout << "Total runtime: " << duration.count() << " seconds" << std::endl;
    }

private:
    struct Input {
        Vertex source;
        Vertex target;
        std::vector<Vertex> expectedPath;
        int expectedLength;
    };

    std::vector<Input> readInputFile(const std::string& filename) {
        std::vector<Input> inputs;
        std::ifstream inputFile(filename);
        if (!inputFile.is_open()) {
            std::cerr << "Failed to open '" << filename << "' file!" << std::endl;
            exit(1);
        }
        
        std::string line;
        while (std::getline(inputFile, line)) {
            std::istringstream iss(line);
            int source, target, expectedLength;
            std::vector<int> expectedPath;
            
            if (!(iss >> source >> target >> expectedLength)) {
                std::cerr << "Error reading input data from file!" << std::endl;
                exit(1);
            }
            
            int pathValue;
            while (iss >> pathValue) {
                expectedPath.push_back(pathValue);
            }
            
            inputs.push_back({source, target, expectedPath, expectedLength});
        }
        
        inputFile.close();
        return inputs;
    }

    void executeTest(Dijkstra<GRAPH, false>& dijkstra, const Input& input) {
        dijkstra.run(input.source, input.target);
        
        std::cout << "Source: " << input.source << ", Target: " << input.target << std::endl;
        
        auto path = dijkstra.getPath(input.target);
        int pathLength = dijkstra.getDistance(input.target);
        
        std::cout << "Shortest Path:";
        for (int vertex : path) {
            std::cout << " " << vertex;
        }
        std::cout << std::endl;
        
        std::cout << "Shortest path length: " << pathLength << std::endl;
        
        if (path == input.expectedPath && pathLength == input.expectedLength) {
            std::cout << "Test PASSED" << std::endl;
        } else {
            std::cout << "Test FAILED" << std::endl;
        }
        
        std::cout << std::endl;
    }
};

int main() {
        Shell::Shell shell;  // Fully qualified name

    // Create an instance of the LoadDimacsGraph class
    LoadDimacsGraph loadDimacsGraph(shell);

    // Set the parameters for the LoadDimacsGraph command (using a method that likely exists)
    loadDimacsGraph.publicAddParameter("Input file", "./../Networks/Florida/OSM/dimacs/USA-road-d.FLA.gr");
    loadDimacsGraph.publicAddParameter("Output file", "./output.binary");
    loadDimacsGraph.publicAddParameter("Graph type", "dynamic");  // or "static" based on your needs
    loadDimacsGraph.publicAddParameter("Coordinate factor", "0.000001");

    // Execute the command to load the DIMACS graph and convert it to custom binary format
    loadDimacsGraph.execute();

    std::cout << "DIMACS graph loaded and converted to custom binary format." << std::endl;

    DijkstraTester<TransferGraph> tester;  // Replace YourGraphType with the actual graph type you're using
    tester.runTests();
    return 0;
    
}

