#ifndef OS_FINALPROJECT_MSTUTILS_H
#define OS_FINALPROJECT_MSTUTILS_H

#include "../../DataStructures/Include/MST.h"
#include <set>
#include <tuple>
#include <stack>
#include <queue>
#include <iostream>
#include <limits>

using Edge = std::tuple<int, int, int>; // <u ,v ,w(uv) >
using adjList = std::vector<std::vector<std::pair<int, int>>>; // Adjacency list with weights

class MSTUtils {
public:
    static std::set<Edge> adjListToEdgeSet(const adjList &adj_list);


    static std::string mstTotalWeight(MST &mst);

    // finds the avg distance from every u to v in V(G)
    static std::string findAvgDistance(MST &mst);



    static std::string findMstLongestDistance(MST &mst);

    // find the shortest path from u to v, with at least one edge from mst
    static std::string findShortestPathWithMstEdge(MST &mst,const adjList &graph, int src, int dest);

private:
    // Helper function to perform DFS and find the longest path
    static bool dfs(const adjList &mstAdjList, int currentNode, int currentDist, std::vector<bool> &visited, int &maxDist, int &farthestNode);

    // Floyd-Warshall algorithm to compute all-pairs shortest paths
    static std::vector<std::vector<int>> floydWarshall(const adjList &graph, int V);

    static const int NINF;
    static const int INF;
};

#endif //OS_FINALPROJECT_MSTUTILS_H