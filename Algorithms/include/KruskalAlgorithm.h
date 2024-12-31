#ifndef KRUSKALALGORITHM_H
#define KRUSKALALGORITHM_H

#include "MSTAlgorithm.h"
#include <vector>
#include <algorithm>

class KruskalAlgorithm : public MSTAlgorithm {
public:
    explicit KruskalAlgorithm(const Graph &graph) : MSTAlgorithm(graph) {}
    ~KruskalAlgorithm() override = default;
    MST findMST() override;

private:
    struct Edge {
        int u, v, weight;
        bool operator<(const Edge& other) const {
            return weight < other.weight;
        }
    };
    std::vector<Edge> edges;

    void initializeEdges();
    int findParent(int node, std::vector<int>& parent);
    void unionSets(int u, int v, std::vector<int>& parent, std::vector<int>& rank);
};

#endif // KRUSKALALGORITHM_H
