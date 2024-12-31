#include "../include/KruskalAlgorithm.h"

MST KruskalAlgorithm::findMST() {
    initializeEdges();
    std::sort(edges.begin(), edges.end());

    std::vector<int> parent(graph.getNumberOfVertices());
    std::vector<int> rank(graph.getNumberOfVertices(), 0);
    for (int i = 0; i < parent.size(); ++i) {
        parent[i] = i;
    }

    adjList mst(graph.getNumberOfVertices());
    for (const auto& edge : edges) {
        int u = edge.u;
        int v = edge.v;
        if (findParent(u, parent) != findParent(v, parent)) {
            unionSets(u, v, parent, rank);
            mst[u].emplace_back(v, edge.weight);
            mst[v].emplace_back(u, edge.weight);
        }
    }
    return {graph,mst};
}

void KruskalAlgorithm::initializeEdges() {
    // Convert graph adjacency list to edge list
    const auto& adj = graph.getAdjList();
    edges.clear();
    for (int u = 0; u < adj.size(); ++u) {
        for (const auto& edge : adj[u]) {
            int v = edge.first;
            int w = edge.second;
            if (u < v) { // Avoid adding both (u,v) and (v,u) in undirected graph
                edges.push_back({u, v, w});
            }
        }
    }
}

int KruskalAlgorithm::findParent(int node, std::vector<int>& parent) {
    if (parent[node] != node) {
        parent[node] = findParent(parent[node], parent);
    }
    return parent[node];
}

void KruskalAlgorithm::unionSets(int u, int v, std::vector<int>& parent, std::vector<int>& rank) {
    int rootU = findParent(u, parent);
    int rootV = findParent(v, parent);
    if (rootU != rootV) {
        if (rank[rootU] < rank[rootV]) {
            parent[rootU] = rootV;
        } else if (rank[rootU] > rank[rootV]) {
            parent[rootV] = rootU;
        } else {
            parent[rootV] = rootU;
            rank[rootU]++;
        }
    }
}
