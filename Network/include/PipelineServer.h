#ifndef PIPELINESERVER_H
#define PIPELINESERVER_H

#include "LFserver.h"
#include "Server.h"
#include "../../DesignPatterns/include/MstComputationPipeline.h"
#include "../../Algorithms/include/MSTUtils.h"

class PipelineServer : public  Server {
public:
    explicit PipelineServer(std::string port) :Server(std::move(port)) {} ;

    void stop() {
        keepRunning = false;
        close(listener);

        this->pipeline.stopPipeline();
    
        for (auto& thread : clientThreads) {
            if (thread.joinable()) {
                thread.join(); // Wait for all client threads to finish
            }
        }
    }


private:

    //The pipeline object
    MSTComputationPipeline  pipeline;

    void performMSTComputation(int clientSocket,MST& mst) override {
        // get two vertices for shortest path
        std::pair<int,int> verticesForShortestPath = getVerticesForShortestPath(clientSocket);
        
        const std:: string result = this->pipeline.performMstComputation(mst,verticesForShortestPath);

        // Send the result to the client
        sendResponse(clientSocket, result);
    }

};



#endif //PIPELINESERVER_H
