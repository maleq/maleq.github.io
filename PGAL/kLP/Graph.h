//
// Created by Sharon Boddu.
//

#ifndef KLP_GRAPH_H
#define KLP_GRAPH_H

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "util.h"

class Graph {
    class Neighborhood {
    public:
        Neighborhood(const Vertex vertex, const VSize degree, Vertex* neighbors)
            : vertex_(vertex), degree_(degree), neighbors_(neighbors) {}

        using iterator = Vertex*;

        [[nodiscard]] iterator begin() const { return neighbors_; }
        [[nodiscard]] iterator end() const { return neighbors_ + degree_; }

    private:
        Vertex vertex_;
        VSize degree_;
        Vertex* neighbors_;
    };

    struct LabelPair {
        Vertex label;
        double confidence;
    };

public:
    VSize numVert;
    VSize numEdg;
    VSize* deg;
    VSize maxDeg;
    VSize* Offsets;
    Vertex* Edges;
    VSize* Reverse;

    Graph()
        : numVert(0),
          numEdg(0),
          deg(nullptr),
          maxDeg(0),
          Offsets(nullptr),
          Edges(nullptr),
          Reverse(nullptr) {}

    ~Graph() {
        graphClear();
    }

    Graph(const Graph&) = delete;
    Graph& operator=(const Graph&) = delete;

    [[nodiscard]] Neighborhood nbr_iter(const Vertex vertex) const {
        return {vertex, deg[vertex], Edges + Offsets[vertex]};
    }

    [[nodiscard]] Neighborhood nbr_iter(const Vertex vertex,
                                        const VSize start) const {
        return {
            vertex,
            deg[vertex] - start,
            Edges + Offsets[vertex] + start
        };
    }

    [[nodiscard]] Neighborhood nbr_iter(const Vertex vertex,
                                        const VSize start,
                                        const VSize count) const {
        const VSize remaining = deg[vertex] - start;
        return {
            vertex,
            std::min(count, remaining),
            Edges + Offsets[vertex] + start
        };
    }

    [[nodiscard]] Vertex nbr(const Vertex vertex, const VSize index) const {
        return Edges[Offsets[vertex] + index];
    }

    [[nodiscard]] VSize degree(const Vertex vertex) const {
        return deg[vertex];
    }

    [[nodiscard]] VSize Size() const {
        return numVert;
    }

    void graphClear() {
        delete[] deg;
        delete[] Offsets;
        delete[] Edges;
        delete[] Reverse;

        numVert = 0;
        numEdg = 0;
        deg = nullptr;
        maxDeg = 0;
        Offsets = nullptr;
        Edges = nullptr;
        Reverse = nullptr;
    }

    void readGraph(const std::string& filename) {
        std::ifstream input(filename);

        VSize vertexCount;
        VSize edgeCount;
        input >> vertexCount >> edgeCount;

        std::vector<VSize> degrees(vertexCount, 0);
        std::vector<std::vector<Vertex>> adjacency(vertexCount);
        VSize degreeSum = 0;
        VSize largestDegree = 0;

        for (VSize record = 0; record < vertexCount; ++record) {
            Vertex vertex;
            VSize vertexDegree;
            input >> vertex >> vertexDegree;

            degrees[vertex] = vertexDegree;
            adjacency[vertex].resize(vertexDegree);

            for (VSize i = 0; i < vertexDegree; ++i) {
                input >> adjacency[vertex][i];
            }

            degreeSum += vertexDegree;
            largestDegree = std::max(largestDegree, vertexDegree);
        }

        auto* newDegrees = new VSize[vertexCount];
        auto* newOffsets = new VSize[vertexCount];
        auto* newEdges = new Vertex[degreeSum];
        auto* newReverse = new VSize[degreeSum];

        VSize offset = 0;
        for (Vertex vertex = 0; vertex < vertexCount; ++vertex) {
            newDegrees[vertex] = degrees[vertex];
            newOffsets[vertex] = offset;
            std::copy(
                adjacency[vertex].begin(),
                adjacency[vertex].end(),
                newEdges + offset);
            offset += degrees[vertex];
        }

        for (Vertex vertex = 0; vertex < vertexCount; ++vertex) {
            for (VSize i = 0; i < degrees[vertex]; ++i) {
                const Vertex neighbor =
                    newEdges[newOffsets[vertex] + i];
                Vertex* neighborBegin =
                    newEdges + newOffsets[neighbor];
                const std::ptrdiff_t reversePosition = BinarySearch(
                    neighborBegin,
                    std::ptrdiff_t{0},
                    static_cast<std::ptrdiff_t>(
                        degrees[neighbor] - 1),
                    vertex);
                newReverse[newOffsets[vertex] + i] =
                    static_cast<VSize>(reversePosition - 1);
            }
        }

        graphClear();
        numVert = vertexCount;
        numEdg = edgeCount;
        deg = newDegrees;
        maxDeg = largestDegree;
        Offsets = newOffsets;
        Edges = newEdges;
        Reverse = newReverse;
    }

    void printGraph() const {
        std::cout << numVert << ' ' << numEdg << '\n';
        for (Vertex vertex = 0; vertex < numVert; ++vertex) {
            std::cout << vertex << ' ' << deg[vertex] << '\n';
            for (const Vertex neighbor : nbr_iter(vertex)) {
                std::cout << neighbor << '\n';
            }
        }
    }

    [[nodiscard]] Vertex* kLPUniform(const VSize k=3) const {
        LabelPair labels[numVert][k];
        LabelPair newLabels[numVert][k];
        auto* J = new float[2 * numEdg];
        double C[numVert];
        Vertex Z[numVert];
        std::fill_n(C, numVert, -1.0);

        // Initialize the labels with unique values
        for (Vertex vertex = 0; vertex < numVert; ++vertex) {
            for (VSize i = 0; i < k; ++i) {
                labels[vertex][i] = {numVert, 0.0};
                newLabels[vertex][i] = {numVert, 0.0};
            }
            labels[vertex][0] = {vertex, 1.0};
        }

        // Compute the Jaccard indices.
        for (Vertex v = 0; v < numVert; ++v) {
            const VSize dv = deg[v];
            const VSize v_offset = Offsets[v];
            Vertex* v_begin = Edges + v_offset;

            for (VSize v_idx = 0; v_idx < dv; ++v_idx) {
                const Vertex u = v_begin[v_idx];
                if (u > v) {
                    const VSize du = deg[u];
                    const VSize u_offset = Offsets[u];
                    Vertex* u_begin = Edges + u_offset;
                    VSize i_count = 0;

                    const bool v_is_small = dv < du;
                    const VSize small_degree = v_is_small ? dv : du;
                    const VSize large_degree = v_is_small ? du : dv;

                    if (large_degree > 16 * small_degree) {
                        Vertex* small_begin =
                            v_is_small ? v_begin : u_begin;
                        Vertex* large_begin =
                            v_is_small ? u_begin : v_begin;
                        VSize large_pos = 0;

                        for (VSize i = 0;
                             i < small_degree && large_pos < large_degree;
                             ++i) {
                            const std::ptrdiff_t found = BinarySearch(
                                large_begin,
                                static_cast<std::ptrdiff_t>(large_pos),
                                static_cast<std::ptrdiff_t>(
                                    large_degree - 1),
                                small_begin[i]);
                            if (found != 0) {
                                ++i_count;
                                large_pos = static_cast<VSize>(found);
                            }
                        }
                    } else {
                        VSize iv = 0;
                        VSize iu = 0;
                        while (iv < dv && iu < du) {
                            if (v_begin[iv] < u_begin[iu]) {
                                ++iv;
                            } else if (v_begin[iv] > u_begin[iu]) {
                                ++iu;
                            } else {
                                ++i_count;
                                ++iv;
                                ++iu;
                            }
                        }
                    }

                    const VSize u_count =
                        dv + du - i_count;
                    const float jaccard =
                        static_cast<float>(i_count) /
                        static_cast<float>(u_count);
                    const VSize u_idx =
                        Reverse[v_offset + v_idx];

                    J[v_offset + v_idx] = jaccard;
                    J[u_offset + u_idx] = jaccard;
                }
            }
        }

        bool changed = true;
        while (changed) {
            changed = false;

            for (Vertex vertex = 0; vertex < numVert; ++vertex) {
                VSize size = 0;

                for (VSize i = 0; i < k; ++i) {
                    const LabelPair label = labels[vertex][i];
                    if (label.label != numVert &&
                        label.confidence > 0.0) {
                        C[label.label] = label.confidence;
                        Z[size++] = label.label;
                    }
                }

                for (VSize neighborIndex = 0;
                     neighborIndex < deg[vertex];
                     ++neighborIndex) {
                    const Vertex neighbor = nbr(vertex, neighborIndex);
                    const double weight =
                        J[Offsets[vertex] + neighborIndex];

                    for (VSize i = 0; i < k; ++i) {
                        const LabelPair label = labels[neighbor][i];
                        const double contribution =
                            weight * label.confidence;
                        if (label.label != numVert &&
                            contribution > 0.0) {
                            if (C[label.label] < 0.0) {
                                C[label.label] = contribution;
                                Z[size++] = label.label;
                            } else {
                                C[label.label] += contribution;
                            }
                        }
                    }
                }

                const VSize selectedCount = std::min(k, size);
                std::partial_sort(
                    Z,
                    Z + selectedCount,
                    Z + size,
                    [&C](const Vertex first, const Vertex second) {
                        if (C[first] == C[second]) {
                            return first < second;
                        }
                        return C[first] > C[second];
                    });

                if (selectedCount == 0) {
                    newLabels[vertex][0] = labels[vertex][0];
                    for (VSize i = 1; i < k; ++i) {
                        newLabels[vertex][i] = {numVert, 0.0};
                    }
                } else {
                    for (VSize i = 0; i < selectedCount; ++i) {
                        const Vertex label = Z[i];
                        newLabels[vertex][i] = {label, C[label]};
                    }
                    for (VSize i = selectedCount; i < k; ++i) {
                        newLabels[vertex][i] = {numVert, 0.0};
                    }
                    if (newLabels[vertex][0].label !=
                        labels[vertex][0].label) {
                        changed = true;
                    }
                }

                for (VSize i = 0; i < size; ++i) {
                    C[Z[i]] = -1.0;
                }
            }

            for (Vertex vertex = 0; vertex < numVert; ++vertex)
                for (VSize i = 0; i < k; ++i)
                    std::swap(labels[vertex][i], newLabels[vertex][i]);
        }

        auto* communities = new Vertex[numVert];
        for (Vertex vertex = 0; vertex < numVert; ++vertex) {
            communities[vertex] = labels[vertex][0].label;
        }
        delete[] J;

        return communities;
    }

    [[nodiscard]] Vertex* kLPDegree(const VSize k=3) const {
        LabelPair labels[numVert][k];
        LabelPair newLabels[numVert][k];
        auto* J = new float[2 * numEdg];
        double C[numVert];
        Vertex Z[numVert];
        std::fill_n(C, numVert, -1.0);

        for (Vertex vertex = 0; vertex < numVert; ++vertex) {
            for (VSize i = 0; i < k; ++i) {
                labels[vertex][i] = {numVert, 0.0};
                newLabels[vertex][i] = {numVert, 0.0};
            }
            labels[vertex][0] = {
                vertex,
                static_cast<double>(deg[vertex])
            };
        }

        // Compute the Jaccard indices.
        for (Vertex v = 0; v < numVert; ++v) {
            const VSize dv = deg[v];
            const VSize v_offset = Offsets[v];
            Vertex* v_begin = Edges + v_offset;

            for (VSize v_idx = 0; v_idx < dv; ++v_idx) {
                const Vertex u = v_begin[v_idx];
                if (u > v) {
                    const VSize du = deg[u];
                    const VSize u_offset = Offsets[u];
                    Vertex* u_begin = Edges + u_offset;
                    VSize i_count = 0;

                    const bool v_is_small = dv < du;
                    const VSize small_degree = v_is_small ? dv : du;
                    const VSize large_degree = v_is_small ? du : dv;

                    if (large_degree > 16 * small_degree) {
                        Vertex* small_begin =
                            v_is_small ? v_begin : u_begin;
                        Vertex* large_begin =
                            v_is_small ? u_begin : v_begin;
                        VSize large_pos = 0;

                        for (VSize i = 0;
                             i < small_degree && large_pos < large_degree;
                             ++i) {
                            const std::ptrdiff_t found = BinarySearch(
                                large_begin,
                                static_cast<std::ptrdiff_t>(large_pos),
                                static_cast<std::ptrdiff_t>(
                                    large_degree - 1),
                                small_begin[i]);
                            if (found != 0) {
                                ++i_count;
                                large_pos = static_cast<VSize>(found);
                            }
                        }
                    } else {
                        VSize iv = 0;
                        VSize iu = 0;
                        while (iv < dv && iu < du) {
                            if (v_begin[iv] < u_begin[iu]) {
                                ++iv;
                            } else if (v_begin[iv] > u_begin[iu]) {
                                ++iu;
                            } else {
                                ++i_count;
                                ++iv;
                                ++iu;
                            }
                        }
                    }

                    const VSize u_count =
                        dv + du - i_count;
                    const float jaccard =
                        static_cast<float>(i_count) /
                        static_cast<float>(u_count);
                    const VSize u_idx =
                        Reverse[v_offset + v_idx];

                    J[v_offset + v_idx] = jaccard;
                    J[u_offset + u_idx] = jaccard;
                }
            }
        }

        bool changed = true;
        while (changed) {
            changed = false;

            for (Vertex vertex = 0; vertex < numVert; ++vertex) {
                VSize size = 0;

                for (VSize i = 0; i < k; ++i) {
                    const LabelPair label = labels[vertex][i];
                    if (label.label != numVert &&
                        label.confidence > 0.0) {
                        C[label.label] = label.confidence;
                        Z[size++] = label.label;
                    }
                }

                for (VSize neighborIndex = 0;
                     neighborIndex < deg[vertex];
                     ++neighborIndex) {
                    const Vertex neighbor = nbr(vertex, neighborIndex);
                    const double weight =
                        J[Offsets[vertex] + neighborIndex];

                    for (VSize i = 0; i < k; ++i) {
                        const LabelPair label = labels[neighbor][i];
                        const double contribution =
                            weight * label.confidence;
                        if (label.label != numVert &&
                            contribution > 0.0) {
                            if (C[label.label] < 0.0) {
                                C[label.label] = contribution;
                                Z[size++] = label.label;
                            } else {
                                C[label.label] += contribution;
                            }
                        }
                    }
                }

                const VSize selectedCount = std::min(k, size);
                std::partial_sort(
                    Z,
                    Z + selectedCount,
                    Z + size,
                    [&C](const Vertex first, const Vertex second) {
                        if (C[first] == C[second]) {
                            return first < second;
                        }
                        return C[first] > C[second];
                    });

                if (selectedCount == 0) {
                    newLabels[vertex][0] = labels[vertex][0];
                    for (VSize i = 1; i < k; ++i) {
                        newLabels[vertex][i] = {numVert, 0.0};
                    }
                } else {
                    for (VSize i = 0; i < selectedCount; ++i) {
                        const Vertex label = Z[i];
                        newLabels[vertex][i] = {label, C[label]};
                    }
                    for (VSize i = selectedCount; i < k; ++i) {
                        newLabels[vertex][i] = {numVert, 0.0};
                    }
                    if (newLabels[vertex][0].label !=
                        labels[vertex][0].label) {
                        changed = true;
                    }
                }

                for (VSize i = 0; i < size; ++i) {
                    C[Z[i]] = -1.0;
                }
            }

            for (Vertex vertex = 0; vertex < numVert; ++vertex)
                for (VSize i = 0; i < k; ++i)
                    std::swap(labels[vertex][i], newLabels[vertex][i]);
        }

        auto* communities = new Vertex[numVert];
        for (Vertex vertex = 0; vertex < numVert; ++vertex) {
            communities[vertex] = labels[vertex][0].label;
        }
        delete[] J;

        return communities;
    }

    [[nodiscard]] Vertex* kLPAverageJaccard(const VSize k=3) const {
        LabelPair labels[numVert][k];
        LabelPair newLabels[numVert][k];
        auto* J = new float[2 * numEdg];
        double initialConfidence[numVert];
        double C[numVert];
        Vertex Z[numVert];
        std::fill_n(C, numVert, -1.0);

        for (Vertex vertex = 0; vertex < numVert; ++vertex) {
            initialConfidence[vertex] = 0.0;

            for (VSize i = 0; i < k; ++i) {
                labels[vertex][i] = {numVert, 0.0};
                newLabels[vertex][i] = {numVert, 0.0};
            }
        }

        // Compute the Jaccard indices.
        for (Vertex v = 0; v < numVert; ++v) {
            const VSize dv = deg[v];
            const VSize v_offset = Offsets[v];
            Vertex* v_begin = Edges + v_offset;

            for (VSize v_idx = 0; v_idx < dv; ++v_idx) {
                const Vertex u = v_begin[v_idx];
                if (u > v) {
                    const VSize du = deg[u];
                    const VSize u_offset = Offsets[u];
                    Vertex* u_begin = Edges + u_offset;
                    VSize i_count = 0;

                    const bool v_is_small = dv < du;
                    const VSize small_degree = v_is_small ? dv : du;
                    const VSize large_degree = v_is_small ? du : dv;

                    if (large_degree > 16 * small_degree) {
                        Vertex* small_begin =
                            v_is_small ? v_begin : u_begin;
                        Vertex* large_begin =
                            v_is_small ? u_begin : v_begin;
                        VSize large_pos = 0;

                        for (VSize i = 0;
                             i < small_degree && large_pos < large_degree;
                             ++i) {
                            const std::ptrdiff_t found = BinarySearch(
                                large_begin,
                                static_cast<std::ptrdiff_t>(large_pos),
                                static_cast<std::ptrdiff_t>(
                                    large_degree - 1),
                                small_begin[i]);
                            if (found != 0) {
                                ++i_count;
                                large_pos = static_cast<VSize>(found);
                            }
                        }
                    } else {
                        VSize iv = 0;
                        VSize iu = 0;
                        while (iv < dv && iu < du) {
                            if (v_begin[iv] < u_begin[iu]) {
                                ++iv;
                            } else if (v_begin[iv] > u_begin[iu]) {
                                ++iu;
                            } else {
                                ++i_count;
                                ++iv;
                                ++iu;
                            }
                        }
                    }

                    const VSize u_count =
                        dv + du - i_count;
                    const float jaccard =
                        static_cast<float>(i_count) /
                        static_cast<float>(u_count);
                    const VSize u_idx =
                        Reverse[v_offset + v_idx];

                    J[v_offset + v_idx] = jaccard;
                    J[u_offset + u_idx] = jaccard;
                    initialConfidence[v] += jaccard;
                    initialConfidence[u] += jaccard;
                }
            }
        }

        for (Vertex vertex = 0; vertex < numVert; ++vertex) {
            if (deg[vertex] > 0) {
                initialConfidence[vertex] /=
                    static_cast<double>(deg[vertex]);
            }
            labels[vertex][0] = {
                vertex,
                initialConfidence[vertex]
            };
        }

        bool changed = true;
        while (changed) {
            changed = false;

            for (Vertex vertex = 0; vertex < numVert; ++vertex) {
                VSize size = 0;

                for (VSize i = 0; i < k; ++i) {
                    const LabelPair label = labels[vertex][i];
                    if (label.label != numVert &&
                        label.confidence > 0.0) {
                        C[label.label] = label.confidence;
                        Z[size++] = label.label;
                    }
                }

                for (VSize neighborIndex = 0;
                     neighborIndex < deg[vertex];
                     ++neighborIndex) {
                    const Vertex neighbor = nbr(vertex, neighborIndex);
                    const double weight =
                        J[Offsets[vertex] + neighborIndex];

                    for (VSize i = 0; i < k; ++i) {
                        const LabelPair label = labels[neighbor][i];
                        const double contribution =
                            weight * label.confidence;
                        if (label.label != numVert &&
                            contribution > 0.0) {
                            if (C[label.label] < 0.0) {
                                C[label.label] = contribution;
                                Z[size++] = label.label;
                            } else {
                                C[label.label] += contribution;
                            }
                        }
                    }
                }

                const VSize selectedCount = std::min(k, size);
                std::partial_sort(
                    Z,
                    Z + selectedCount,
                    Z + size,
                    [&C](const Vertex first, const Vertex second) {
                        if (C[first] == C[second]) {
                            return first < second;
                        }
                        return C[first] > C[second];
                    });

                if (selectedCount == 0) {
                    newLabels[vertex][0] = labels[vertex][0];
                    for (VSize i = 1; i < k; ++i) {
                        newLabels[vertex][i] = {numVert, 0.0};
                    }
                } else {
                    for (VSize i = 0; i < selectedCount; ++i) {
                        const Vertex label = Z[i];
                        newLabels[vertex][i] = {label, C[label]};
                    }
                    for (VSize i = selectedCount; i < k; ++i) {
                        newLabels[vertex][i] = {numVert, 0.0};
                    }
                    if (newLabels[vertex][0].label !=
                        labels[vertex][0].label) {
                        changed = true;
                    }
                }

                for (VSize i = 0; i < size; ++i) {
                    C[Z[i]] = -1.0;
                }
            }

            for (Vertex vertex = 0; vertex < numVert; ++vertex)
                for (VSize i = 0; i < k; ++i)
                    std::swap(labels[vertex][i], newLabels[vertex][i]);
        }

        auto* communities = new Vertex[numVert];
        for (Vertex vertex = 0; vertex < numVert; ++vertex) {
            communities[vertex] = labels[vertex][0].label;
        }
        delete[] J;

        return communities;
    }

    [[nodiscard]] Vertex* kLPClusteringCoefficient(const VSize k=3) const {
        LabelPair labels[numVert][k];
        LabelPair newLabels[numVert][k];
        auto* J = new float[2 * numEdg];
        double initialConfidence[numVert];
        VSize commonNeighborSum[numVert];
        double C[numVert];
        Vertex Z[numVert];
        std::fill_n(C, numVert, -1.0);

        for (Vertex vertex = 0; vertex < numVert; ++vertex) {
            initialConfidence[vertex] = 0.0;
            commonNeighborSum[vertex] = 0;

            for (VSize i = 0; i < k; ++i) {
                labels[vertex][i] = {numVert, 0.0};
                newLabels[vertex][i] = {numVert, 0.0};
            }
        }

        // Compute the Jaccard indices.
        for (Vertex v = 0; v < numVert; ++v) {
            const VSize dv = deg[v];
            const VSize v_offset = Offsets[v];
            Vertex* v_begin = Edges + v_offset;

            for (VSize v_idx = 0; v_idx < dv; ++v_idx) {
                const Vertex u = v_begin[v_idx];
                if (u > v) {
                    const VSize du = deg[u];
                    const VSize u_offset = Offsets[u];
                    Vertex* u_begin = Edges + u_offset;
                    VSize i_count = 0;

                    const bool v_is_small = dv < du;
                    const VSize small_degree = v_is_small ? dv : du;
                    const VSize large_degree = v_is_small ? du : dv;

                    if (large_degree > 16 * small_degree) {
                        Vertex* small_begin =
                            v_is_small ? v_begin : u_begin;
                        Vertex* large_begin =
                            v_is_small ? u_begin : v_begin;
                        VSize large_pos = 0;

                        for (VSize i = 0;
                             i < small_degree && large_pos < large_degree;
                             ++i) {
                            const std::ptrdiff_t found = BinarySearch(
                                large_begin,
                                static_cast<std::ptrdiff_t>(large_pos),
                                static_cast<std::ptrdiff_t>(
                                    large_degree - 1),
                                small_begin[i]);
                            if (found != 0) {
                                ++i_count;
                                large_pos = static_cast<VSize>(found);
                            }
                        }
                    } else {
                        VSize iv = 0;
                        VSize iu = 0;
                        while (iv < dv && iu < du) {
                            if (v_begin[iv] < u_begin[iu]) {
                                ++iv;
                            } else if (v_begin[iv] > u_begin[iu]) {
                                ++iu;
                            } else {
                                ++i_count;
                                ++iv;
                                ++iu;
                            }
                        }
                    }

                    const VSize u_count =
                        dv + du - i_count;
                    const float jaccard =
                        static_cast<float>(i_count) /
                        static_cast<float>(u_count);
                    const VSize u_idx =
                        Reverse[v_offset + v_idx];

                    J[v_offset + v_idx] = jaccard;
                    J[u_offset + u_idx] = jaccard;
                    commonNeighborSum[v] += i_count;
                    commonNeighborSum[u] += i_count;
                }
            }
        }

        for (Vertex vertex = 0; vertex < numVert; ++vertex) {
            if (deg[vertex] > 1) {
                initialConfidence[vertex] =
                    (2.0 * static_cast<double>(
                        commonNeighborSum[vertex])) /
                    (static_cast<double>(deg[vertex]) *
                     static_cast<double>(deg[vertex] - 1));
            }
            labels[vertex][0] = {
                vertex,
                initialConfidence[vertex]
            };
        }

        bool changed = true;
        while (changed) {
            changed = false;

            for (Vertex vertex = 0; vertex < numVert; ++vertex) {
                VSize size = 0;

                for (VSize i = 0; i < k; ++i) {
                    const LabelPair label = labels[vertex][i];
                    if (label.label != numVert &&
                        label.confidence > 0.0) {
                        C[label.label] = label.confidence;
                        Z[size++] = label.label;
                    }
                }

                for (VSize neighborIndex = 0;
                     neighborIndex < deg[vertex];
                     ++neighborIndex) {
                    const Vertex neighbor = nbr(vertex, neighborIndex);
                    const double weight =
                        J[Offsets[vertex] + neighborIndex];

                    for (VSize i = 0; i < k; ++i) {
                        const LabelPair label = labels[neighbor][i];
                        const double contribution =
                            weight * label.confidence;
                        if (label.label != numVert &&
                            contribution > 0.0) {
                            if (C[label.label] < 0.0) {
                                C[label.label] = contribution;
                                Z[size++] = label.label;
                            } else {
                                C[label.label] += contribution;
                            }
                        }
                    }
                }

                const VSize selectedCount = std::min(k, size);
                std::partial_sort(
                    Z,
                    Z + selectedCount,
                    Z + size,
                    [&C](const Vertex first, const Vertex second) {
                        if (C[first] == C[second]) {
                            return first < second;
                        }
                        return C[first] > C[second];
                    });

                if (selectedCount == 0) {
                    newLabels[vertex][0] = labels[vertex][0];
                    for (VSize i = 1; i < k; ++i) {
                        newLabels[vertex][i] = {numVert, 0.0};
                    }
                } else {
                    for (VSize i = 0; i < selectedCount; ++i) {
                        const Vertex label = Z[i];
                        newLabels[vertex][i] = {label, C[label]};
                    }
                    for (VSize i = selectedCount; i < k; ++i) {
                        newLabels[vertex][i] = {numVert, 0.0};
                    }
                    if (newLabels[vertex][0].label !=
                        labels[vertex][0].label) {
                        changed = true;
                    }
                }

                for (VSize i = 0; i < size; ++i) {
                    C[Z[i]] = -1.0;
                }
            }

            for (Vertex vertex = 0; vertex < numVert; ++vertex)
                for (VSize i = 0; i < k; ++i)
                    std::swap(labels[vertex][i], newLabels[vertex][i]);
        }

        auto* communities = new Vertex[numVert];
        for (Vertex vertex = 0; vertex < numVert; ++vertex) {
            communities[vertex] = labels[vertex][0].label;
        }
        delete[] J;

        return communities;
    }

};

#endif // KLP_GRAPH_H
