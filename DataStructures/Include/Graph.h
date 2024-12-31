#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <utility> // For std::pair

#define REMOVE_EDGE 1
#define ADD_EDGE 0

using adjList = std::vector<std::vector<std::pair<int, int>>>; // Adjacency list with weights

class Graph {
    adjList graph;
    int V, E;

public:
    // Constructor from number of vertices and edges (assumes 0-based indexing)
    Graph( int V,  int E);
    
    // Constructor from an adjacency list
    explicit Graph(adjList &graph);

    Graph() :V(0) ,E(0) {}

    // Add an edge with a given weight
    void addEdge(int u, int v, int w);

    // Remove an edge from the graph
    void removeEdge(int u, int v);

    int getNumberOfVertices() const { return V; }

    const adjList& getAdjList()  {return graph;}

    bool isValidInput(int u , int v , int w , int action) const;
    
    bool isEmpty() const {return V == 0 && E == 0;}


private:
    // Check if an edge exists
    [[nodiscard]] bool edgeExists(int u, int v) const;

    // Remove an edge from a specific vertex's adjacency list
    void removeEdgeFromList(int u, int v);
};

#endif // GRAPH_H
