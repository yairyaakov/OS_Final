#ifndef OS_FINALPROJECT_MST_H
#define OS_FINALPROJECT_MST_H

#include <utility>
#include <iostream>
#include <vector>
#include <set>
#include <tuple>
#include "Graph.h"

// Using Edge as a tuple (u, v, weight)
using Edge = std::tuple<int, int, int>;
using adjList = std::vector<std::vector<std::pair<int, int>>>; // Adjacency list with weights

class MST {
public:

    MST(){};

    // Constructor to initialize the MST with the original graph and adjacency list
    MST(Graph originalGraph, adjList mst) : graph(std::move(originalGraph)), mst(std::move(mst)) {};

    // Getter for the graph
    Graph &getGraph() { return graph; }

    // Getter for the MST adjacency list
    adjList &getMSTAdjList() { return mst; }


    std::string getMstEdges() {
        std::set<Edge> printedEdges;  // To avoid printing duplicate edges
        std::string edges="Mst:\n";
        for (size_t u = 0; u < mst.size(); ++u) {
            for (const auto &neighbor : mst[u]) {
                int v = neighbor.first;  // The neighboring node
                int w = neighbor.second; // The weight of the edge

                // Ensure the edge (u, v) is only printed once, using a set to track
                if (u < v) {
                    edges+= "Edge from "+std::to_string(u)+" to "+std::to_string(v)+" with weight "+std::to_string(w)+"\n" ;
                }
            }
        }
        return edges;
    }

private:
    adjList mst;  // Adjacency list representing the MST
    Graph graph;  // The original graph

    friend class MSTUtils;
};

#endif //OS_FINALPROJECT_MST_H
