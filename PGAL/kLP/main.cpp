#include <chrono>
#include <fstream>
#include <iostream>
#include <string>

#include "Graph.h"

int main(int, char* argv[]) {
    const std::string graphFile = argv[1];
    const VSize k = std::stoull(argv[2]);
    const std::string initialConfidence = argv[3];
    const std::string outputFile = argv[4];

    Graph graph;
    graph.readGraph(graphFile);

    const auto start = std::chrono::steady_clock::now();
    Vertex* communities;

    if (initialConfidence == "uniform") {
        communities = graph.kLPUniform(k);
    } else if (initialConfidence == "degree") {
        communities = graph.kLPDegree(k);
    } else if (initialConfidence == "avg-jaccard") {
        communities = graph.kLPAverageJaccard(k);
    } else if (initialConfidence == "clustering") {
        communities = graph.kLPClusteringCoefficient(k);
    } else {
        return 1;
    }

    const std::chrono::duration<double> elapsed =
        std::chrono::steady_clock::now() - start;
    std::cout << "Elapsed time: " << elapsed.count() << " seconds\n";

    std::ofstream output(outputFile);
    for (Vertex vertex = 0; vertex < graph.Size(); ++vertex) {
        output << vertex << ' ' << communities[vertex] << '\n';
    }

    delete[] communities;
    return 0;
}
