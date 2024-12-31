#include "../include/PrimAlgorithm.h"
#include <vector>
#include <queue>
#include <limits>

MST PrimAlgorithm::findMST() {
    const int V = graph.getNumberOfVertices();
    adjList mst(V);
    std::vector<bool> inMST(V, false);
    std::vector<int> parent(V, -1);
    std::vector<int> key(V, std::numeric_limits<int>::max());

    // Min-heap priority queue
    using Edge = std::pair<int, int>; // (weight, vertex)
    std::priority_queue<Edge, std::vector<Edge>, std::greater<>> pq;

    // Start from vertex 0
    key[0] = 0;
    pq.emplace(0, 0); // (weight, vertex)

    while (!pq.empty()) {
        int u = pq.top().second;
        pq.pop();

        if (inMST[u]) continue;

        inMST[u] = true;

        // Explore neighbors of u
        for (const auto& neighbor : graph.getAdjList()[u]) {
            int v = neighbor.first;
            int weight = neighbor.second;

            if (!inMST[v] && weight < key[v]) {
                key[v] = weight;
                parent[v] = u;
                pq.emplace(weight, v);
            }
        }
    }

    // Build the MST adjacency list
    for (int v = 1; v < V; ++v) {
        if (parent[v] != -1) {
            mst[v].emplace_back(parent[v], key[v]);
            mst[parent[v]].emplace_back(v, key[v]);
        }
    }

    return  {graph,mst};
}
