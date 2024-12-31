#ifndef MSTCOMPUTATIONPIPELINE_H
#define MSTCOMPUTATIONPIPELINE_H

#include "ActiveObject.h"
#include "../../DataStructures/Include/MST.h"
#include "../../Algorithms/include/MSTUtils.h"

class MSTComputationPipeline {
public:

    // Constructor: set up the pipeline stages
    MSTComputationPipeline() {
        // Stage 1: Compute total weight
        totalWeightStage = new ActiveObject(1, [](PTask& task) {
            task.result += MSTUtils::mstTotalWeight(task.mst) + "\n";
        });

        // Stage 2: Compute average distance
        avgDistanceStage = new ActiveObject(2, [](PTask& task) {
            task.result += MSTUtils::findAvgDistance(task.mst) + "\n";
        });

        // Stage 3: Compute the longest distance
        longestDistanceStage = new ActiveObject(3, [](PTask& task) {
            task.result += MSTUtils::findMstLongestDistance(task.mst) + "\n";
        });

        // Stage 4: Compute the shortest path between two vertices u and v
        shortestPathStage = new ActiveObject(4, [](PTask& task) {
            task.result += MSTUtils::findShortestPathWithMstEdge(task.mst, task.mst.getGraph().getAdjList(), task.u, task.v) + "\n";
        });

        // Link the stages to form a pipeline
        totalWeightStage->setNext(avgDistanceStage);
        avgDistanceStage->setNext(longestDistanceStage);
        longestDistanceStage->setNext(shortestPathStage);
    }

    // Perform MST computation using the pipeline
    std::string performMstComputation(MST& mst, std::pair<int, int> verticesForShortestPath) {

        PTask task(mst, verticesForShortestPath.first, verticesForShortestPath.second); 

        // Post the task to the first stage and get the future to wait for completion
        totalWeightStage->postTask([&task]() -> PTask& {return task;});
        
        // Wait for the task to complete and return the final result
        std::unique_lock<std::mutex> lock(task.taskMutex);
        task.cv.wait(lock, [&task] {return task.isCompleted;});

        return task.result;
    }

    // Stop the pipeline
    void stopPipeline() {
        delete totalWeightStage;
        delete avgDistanceStage;
        delete longestDistanceStage;
        delete shortestPathStage;
    }

private:
    ActiveObject* totalWeightStage;
    ActiveObject* avgDistanceStage;
    ActiveObject* longestDistanceStage;
    ActiveObject* shortestPathStage;
};

#endif //MSTCOMUPTATIONPIPELINE_H
