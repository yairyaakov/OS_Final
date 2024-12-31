#include <unordered_set>
#include <vector>
#include <algorithm>
#include <iostream>
#include <set>

#include "../include/doctest.h"
#include "../../Algorithms/include/KruskalAlgorithm.h"
#include "../../Algorithms/include/PrimAlgorithm.h"
#include "../../Algorithms/include/MSTUtils.h"

using adjList = std::vector<std::vector<std::pair<int, int>>>; // <v, w>
using Edge = std::tuple<int, int, int>; // Edge <u, v, w>
using DistanceVertexPair = std::pair<int, int>; // {distance, vertex}

// Define the expected MST edges (with weights)
const std::set<Edge> expectedMSTEdges1 = {
        {0, 1, 2}, {1, 2, 1}, {1, 3, 4}, {2, 4, 6}
};
const std::set<Edge> expectedMSTEdges2 = {
        {0, 1, 1}, {1, 2, 2}, {2, 3, 3}
};

// Graph adjacency lists
adjList graph1 = {
        {{1, 2}, {2, 3}},      // Edges from vertex 0
        {{0, 2}, {2, 1}, {3, 4}}, // Edges from vertex 1
        {{0, 3}, {1, 1}, {3, 5}, {4, 6}}, // Edges from vertex 2
        {{1, 4}, {2, 5}, {4, 7}}, // Edges from vertex 3
        {{2, 6}, {3, 7}}       // Edges from vertex 4
};

 adjList graph2 = {
        {{1, 1}, {2, 4}},  // Edges from vertex 0
        {{0, 1}, {2, 2}, {3, 6}},  // Edges from vertex 1
        {{0, 4}, {1, 2}, {3, 3}},  // Edges from vertex 2
        {{1, 6}, {2, 3}}   // Edges from vertex 3
};
 adjList graph3={
         {{1,1},{2,1}},
         {{0,1},{3,2},{2,3}},
         {{0,1},{1,3},{3,2}},
         {{1,2},{2,2}}
 };
 Graph g1(graph1);
 Graph g2(graph2);
 Graph g3(graph3);

TEST_CASE("Test findMST from adjList vector") {

    SUBCASE("Test Kruskal Algorithm") {
        // For graph 1
        KruskalAlgorithm kruskalRes1(g1);
        MST mst1 = kruskalRes1.findMST();
        adjList result1 = mst1.getMSTAdjList();
        std::set<Edge> resultEdges = MSTUtils::adjListToEdgeSet(result1);
        CHECK(resultEdges == expectedMSTEdges1);

        // For graph 2
        KruskalAlgorithm kruskalRes2(g2);
        MST mst2 = kruskalRes2.findMST();
        adjList result2 = mst2.getMSTAdjList();
        std::set<Edge> resultEdges2 = MSTUtils::adjListToEdgeSet(result2);
        CHECK(resultEdges2 == expectedMSTEdges2);
    }

    SUBCASE("Test Prim Algorithm") {
        // For graph 1
        PrimAlgorithm primRes1(g1);
        MST mst1 = primRes1.findMST();
        adjList result1 = mst1.getMSTAdjList();
        std::set<Edge> resultEdges1 = MSTUtils::adjListToEdgeSet(result1);
        CHECK(resultEdges1 == expectedMSTEdges1);

        // For graph 2
        PrimAlgorithm primRes2(g2);
        MST mst2 = primRes2.findMST();
        adjList result2 = mst2.getMSTAdjList();
        std::set<Edge> resultEdges2 = MSTUtils::adjListToEdgeSet(result2);
        CHECK(resultEdges2 == expectedMSTEdges2);
    }
}

TEST_CASE("Test MST Library"){

    SUBCASE("Test total mst weight"){
        // for graph 1
        std::string g1ExpectedTotalWeight = "The total weight of the MST is: 13";
        PrimAlgorithm primRes1(g1);
        MST mst1 = primRes1.findMST();
        std::string res1 = MSTUtils::mstTotalWeight(mst1);
        CHECK(res1 == g1ExpectedTotalWeight);
        // for graph2
        std::string g2ExpectedTotalWeight = "The total weight of the MST is: 6";
        KruskalAlgorithm kruskalRes2(g2);
        MST mst2 = kruskalRes2.findMST();
        std::string res2 = MSTUtils::mstTotalWeight(mst2);

    }

    SUBCASE("Test Longest distance") {

        KruskalAlgorithm kruskalRes1(g1);
        MST mst1 = kruskalRes1.findMST();
        std::string g1ExpectedLongestDistance = "The longest distance in the MST is between nodes 4 and 3 with a distance of: 11";
        std::string res1 = MSTUtils::findMstLongestDistance(mst1);
        CHECK(res1 == g1ExpectedLongestDistance);


        PrimAlgorithm primRes2(g2);
        MST mst2 = primRes2.findMST();

        std::string g2ExpectedLongestDistance="The longest distance in the MST is between nodes 3 and 0 with a distance of: 6";
        std::string res2=MSTUtils::findMstLongestDistance(mst2);
        CHECK(g2ExpectedLongestDistance==res2);


    }

    SUBCASE("Test avg distance") {
        PrimAlgorithm primRes1(g1);
        MST mst1 = primRes1.findMST();

        std::string expectedAvgDistanceOfg1 = "Average distance between vertices: 5.000000";
        std::string resMst1 = MSTUtils::findAvgDistance(mst1);
        CHECK(expectedAvgDistanceOfg1 == resMst1);
    }
}




