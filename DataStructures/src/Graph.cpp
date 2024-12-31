#include "../Include/Graph.h"
#include <algorithm> // For std::remove_if
#include <iostream>
#include <stdexcept> // For std::invalid_argument

// Constructor from number of vertices and edges ( 0-based indexing)
Graph::Graph( int V,  int E) : V(V), E(E) {
    if (V <= 0 || E < 0) {
        throw std::invalid_argument("Number of vertices must be positive and edges cannot be negative.");
    }
    graph.resize(V);

    int u, v, w;
    for (int i = 0; i < E; i++) {
        std::cin >> u >> v >> w;
        if (u < 0 || v < 0 || u >= V || v >= V || w < 0) {
            throw std::invalid_argument("Invalid edge input.");
        }
        graph[u].emplace_back(v, w);
        graph[v].emplace_back(u, w);
    }
}

// Constructor from adjacency list
Graph::Graph(adjList &graph) : graph(graph), V(graph.size()), E(0) {
    for (const auto& adj : graph) {
        E += adj.size();
    }
    E /= 2; // Since undirected graph, each edge is counted twice
}

// Add an edge with a given weight
void Graph::addEdge(int u, int v, int w) {
    if (!edgeExists(u, v)) {
        graph[u].emplace_back(v, w);
        graph[v].emplace_back(u, w);
        E++;
    }
}

// Remove an edge from the graph
void Graph::removeEdge(int u, int v) {
    removeEdgeFromList(u, v);
    removeEdgeFromList(v, u);
    E--;
}

// Check if an edge exists
bool Graph::edgeExists(int u, int v) const {
    for (const auto& edge : graph[u]) {
        if (edge.first == v) {
            return true;
        }
    }
    return false;
}

// Remove an edge from a specific vertex's adjacency list
void Graph::removeEdgeFromList(int u, int v) {
    auto& edges = graph[u];
    auto it = std::remove_if(edges.begin(), edges.end(),
        [v](const std::pair<int, int>& edge) {
            return edge.first == v;
        });
    edges.erase(it, edges.end());
}


bool Graph::isValidInput(int u, int v, int w , int action) const {

    if (action == REMOVE_EDGE) {
        if (u >= 0 && u < V && v >= 0 && v < V) {
            return edgeExists(u, v);
        }
        return false;
    }
    // Check for negative weight and vertices range if adding a new edge
    return u >= 0 && u < V && v >= 0 && v < V && w > 0;
}
