#ifndef LFSERVER_H
#define LFSERVER_H

#include "Server.h"
#include "../../DesignPatterns/include/LeaderFollower.h"
#include"../../DesignPatterns/include/MSTFactory.h"
#include "../../Algorithms/include/MSTUtils.h"
#include <condition_variable>

struct LFTask {
    std::mutex taskMutex;
    std::condition_variable cv;
    std::string result;
    bool isCompleted = false;

    void performComputation(MST& mst, int u, int v) {
        std::lock_guard<std::mutex> lock(taskMutex);
        result += MSTUtils::mstTotalWeight(mst) + "\n";
        result += MSTUtils::findAvgDistance(mst) + "\n";
        result += MSTUtils::findMstLongestDistance(mst) + "\n";
        result += MSTUtils::findShortestPathWithMstEdge(mst, mst.getGraph().getAdjList(), u, v) + "\n";
        isCompleted = true;
        cv.notify_one();
    }
};

class LFServer : public Server {
public:
    explicit LFServer(std::string port):lf(4), Server(std::move(port)) {};

    void stop() {
        keepRunning = false;
        close(listener);

        this->lf.finish();
        
        // Wait for all client threads to finish
        for (auto& thread : clientThreads) {
            if (thread.joinable()) {
                thread.join(); 
            }
        }
    }

private:

    LeaderFollower lf;

    void performMSTComputation(int clientSocket, MST& mst) override {

        std::pair<int, int> verticesForShortestPath = getVerticesForShortestPath(clientSocket);
        int u = verticesForShortestPath.first;
        int v = verticesForShortestPath.second;

        LFTask task;
        // Add a single task with multiple MST computations
        this->lf.addTask([&mst, u, v, &task]() {
            task.performComputation(mst, u, v);
        });
        
        std::unique_lock<std::mutex> lock(task.taskMutex);
        task.cv.wait(lock, [&task] {return task.isCompleted;});
        // Send the result to the client
        sendResponse(clientSocket, task.result);
    }

};



#endif //LFSERVER_H
