#include "../include/MSTUtils.h"

const int MSTUtils::NINF = std::numeric_limits<int>::min();
const int MSTUtils::INF = std::numeric_limits<int>::max();

std::string MSTUtils::mstTotalWeight(MST &mst) {
    // Convert the MST adjacency list into an edge list
    std::set<Edge> mstEdges = adjListToEdgeSet(mst.getMSTAdjList());
    int totalWeight = 0;

    for (const auto &edge: mstEdges) {
        totalWeight += std::get<2>(edge); // Sum the weights
    }

    return "The total weight of the MST is: " + std::to_string(totalWeight);
}

/*
std::string MSTUtils::findMstSLongestDistanceBetweenTwo(MST &mst, int src, int dest) {
    const adjList &mstAdjList = mst.getMSTAdjList();  // Get the MST adjacency list
    int V = static_cast<int>(mstAdjList.size());

    // Vector to keep track of visited nodes
    std::vector<bool> visited(V, false);

    // This will hold the maximum distance found during DFS
    int maxDist = NINF;

    // Perform DFS starting from the source node
    bool foundPath = dfs(mstAdjList, src, dest, 0, visited, maxDist);

    if (!foundPath) {
        return "No path exists between " + std::to_string(src) + " and " + std::to_string(dest);
    }

    return "The longest distance from " + std::to_string(src) + " to " + std::to_string(dest) + " is: " +
           std::to_string(maxDist);
}
*/
std::string MSTUtils::findMstLongestDistance(MST &mst) {
    const adjList &mstAdjList = mst.getMSTAdjList();
    int V = static_cast<int>(mstAdjList.size());

    int maxDist = NINF;
    int srcNode = -1, destNode = -1;

    for (int i = 0; i < V; ++i) {
        std::vector<bool> visited(V, false);
        int farthestNode = i;  // Start from i
        int localMaxDist = NINF;

        // First DFS to find the farthest node from node i
        dfs(mstAdjList, i, 0, visited, localMaxDist, farthestNode);

        // Reset visited array for the second DFS
        visited.assign(V, false);
        int newFarthestNode = farthestNode;  // Start from the found farthestNode
        int longestDist = NINF;

        // Second DFS to find the longest distance from farthestNode
        dfs(mstAdjList, newFarthestNode, 0, visited, longestDist, newFarthestNode);

        // Update global max distance and nodes if this path is longer
        if (longestDist > maxDist) {
            maxDist = longestDist;
            srcNode = farthestNode;
            destNode = newFarthestNode;
        }
    }

    return "The longest distance in the MST is between nodes " + std::to_string(srcNode) +
           " and " + std::to_string(destNode) + " with a distance of: " + std::to_string(maxDist);
}


std::string MSTUtils::findAvgDistance(MST &mst) {
    const adjList &graph = mst.getGraph().getAdjList();
    int V = static_cast<int>(graph.size());

    // Get the distance matrix using Floyd-Warshall
    std::vector<std::vector<int>> dist = floydWarshall(graph, V);

    // Compute the average distance
    double totalDistance = 0.0;
    int reachablePairs = 0;

    // Iterate over each pair of vertices (i, j) such that i < j to avoid double-counting
    for (int i = 0; i < V; ++i) {
        for (int j = i + 1; j < V; ++j) {
            if (dist[i][j] != INF) { // Only count reachable pairs
                totalDistance += dist[i][j];
                reachablePairs++;
            }
        }
    }

    // Return the average distance
    if (reachablePairs == 0) {
        return "No reachable pairs in the graph";
    }

    double avgDistance = totalDistance / reachablePairs;
    return "Average distance between vertices: " + std::to_string(avgDistance);
}

std::string MSTUtils::findShortestPathWithMstEdge(MST &mst, const adjList &graph, int src, int dest) {
    const adjList &mstAdjList = mst.getMSTAdjList();  // Get the MST adjacency list
    int V = static_cast<int>(graph.size());

    // Validate src and dest arguments
    if (src < 0 || src >= V || dest < 0 || dest >= V) {
        return "Invalid arguments: src and dest must be between 0 and " + std::to_string(V - 1);
    }

    // Priority queue for Dijkstra's algorithm (min-heap)
    // Elements are pairs of (distance, vertex, usedMstEdge) where usedMstEdge is a boolean
    using State = std::tuple<int, int, bool>;  // (distance, vertex, hasUsedMstEdge)
    std::priority_queue<State, std::vector<State>, std::greater<State>> pq;

    // Distance array: dist[node][mstUsed] -> shortest distance to node, mstUsed = 1 means path has used an MST edge
    std::vector<std::vector<int>> dist(V, std::vector<int>(2, INF));

    // Mark MST edges
    std::set<std::pair<int, int>> mstEdges;
    for (int u = 0; u < V; ++u) {
        for (const auto &neighbor : mstAdjList[u]) {
            int v = neighbor.first;
            mstEdges.insert({u, v});
            mstEdges.insert({v, u});  // Because it's an undirected graph
        }
    }

    // Start Dijkstra from the source
    dist[src][0] = 0;  // Distance to src without using an MST edge is 0
    pq.push({0, src, false});  // Initially, we haven't used an MST edge

    while (!pq.empty()) {
        auto [currentDist, node, hasUsedMstEdge] = pq.top();
        pq.pop();

        // If we've reached the destination and used an MST edge, return the result
        if (node == dest && hasUsedMstEdge) {
            return "The shortest path from " + std::to_string(src) + " to " + std::to_string(dest) +
                   " using at least one MST edge is: " + std::to_string(currentDist);
        }

        // Explore neighbors of the current node
        for (const auto &neighbor : graph[node]) {
            int adjNode = neighbor.first;
            int weight = neighbor.second;
            bool isMstEdge = (mstEdges.count({node, adjNode}) > 0);  // Check if the edge belongs to MST

            // Case 1: If we haven't used an MST edge yet, and this edge is part of the MST, use it
            if (!hasUsedMstEdge && isMstEdge && currentDist + weight < dist[adjNode][1]) {
                dist[adjNode][1] = currentDist + weight;  // Mark this path as using an MST edge
                pq.push({dist[adjNode][1], adjNode, true});
            }

            // Case 2: If we have already used an MST edge, continue exploring
            if (hasUsedMstEdge && currentDist + weight < dist[adjNode][1]) {
                dist[adjNode][1] = currentDist + weight;
                pq.push({dist[adjNode][1], adjNode, true});
            }

            // Case 3: If we haven't used an MST edge yet, explore non-MST edges too
            if (!hasUsedMstEdge && currentDist + weight < dist[adjNode][0]) {
                dist[adjNode][0] = currentDist + weight;  // Mark this path as not using an MST edge yet
                pq.push({dist[adjNode][0], adjNode, false});
            }
        }
    }

    // If no valid path is found
    return "No valid path exists from " + std::to_string(src) + " to " + std::to_string(dest) +
           " using at least one MST edge.";
}


// utils
// Floyd-Warshall algorithm to compute the shortest paths between all pairs of vertices
std::vector<std::vector<int>> MSTUtils::floydWarshall(const adjList &graph, int V) {
    // Initialize the distance matrix with INF
    std::vector<std::vector<int>> dist(V, std::vector<int>(V, INF));

    // Set the distances based on the adjacency list
    for (int u = 0; u < V; u++) {
        dist[u][u] = 0; // Distance to self is 0
        for (const auto &edge: graph[u]) {
            int v = edge.first;
            int w = edge.second;
            dist[u][v] = w; // Directed graph, so only set distance u -> v
        }
    }

    // Apply Floyd-Warshall to compute the shortest paths
    for (int k = 0; k < V; ++k) {
        for (int i = 0; i < V; ++i) {
            for (int j = 0; j < V; ++j) {
                if (dist[i][k] != INF && dist[k][j] != INF) {
                    dist[i][j] = std::min(dist[i][j], dist[i][k] + dist[k][j]);
                }
            }
        }
    }

    return dist;
}

// DFS helper function
bool MSTUtils::dfs(const adjList &mstAdjList, int currentNode, int currentDist, std::vector<bool> &visited, int &maxDist, int &farthestNode) {
    visited[currentNode] = true;

    // Check if this is the farthest node
    if (currentDist > maxDist) {
        maxDist = currentDist;
        farthestNode = currentNode; // Update farthest node
    }

    // Traverse the adjacent vertices
    for (const auto &edge : mstAdjList[currentNode]) {
        int neighbor = edge.first;
        int weight = edge.second;

        if (!visited[neighbor]) {
            // Continue DFS, adding the weight of the edge to the current distance
            dfs(mstAdjList, neighbor, currentDist + weight, visited, maxDist, farthestNode);
        }
    }
}

std::set<Edge> MSTUtils::adjListToEdgeSet(const adjList &adj_list) {
    std::set<Edge> result;
    for (size_t u = 0; u < adj_list.size(); ++u) {
        for (const auto &edge: adj_list[u]) {
            int v = edge.first;
            int w = edge.second; // Weight w between u and v

            if (u < v) {  // To avoid adding both (u, v) and (v, u)
                result.insert(std::make_tuple(u, v, w));  // Insert edge as tuple <u, v, w>
            }
        }
    }
    return result;
}




