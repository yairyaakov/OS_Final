#ifndef SERVER_H
#define SERVER_H

#include <string>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
#include "../../DesignPatterns/include/MSTFactory.h"
#include "../../DataStructures/Include/MST.h"

class Server {
public:
    explicit Server(std::string  port);
    virtual ~Server() = default;

    // Pure virtual method that must be implemented by derived classes
     void run() ;
     
     virtual void stop() = 0 ;

protected:
    int listener;  // Server listener socket
    bool keepRunning = true;  // To stop the server from outside
    std::mutex graphMutex;  // Mutex to protect access to the graph
    std::string port;
    std::vector<std::thread> clientThreads; // the clients

    // Methods to handle graph operations
    void newGraph(int clientSocket, Graph& g);
    void addEdge(int clientSocket, Graph& g);
    void removeEdge(int clientSocket, Graph& g);
    std::pair<int , int> getVerticesForShortestPath(int clientSocket);
    // Utility to get the listener socket
    int getListenerSocket();

    // Utility to send response to client
    void sendResponse(int clientSocket, const std::string& response);

    // Handle client requests in a separate thread
    void handleClient(int clientSocket);

    // Perform MST computations to be implemented by lf and pipline
    virtual void performMSTComputation(int clientSocket,MST& mst) = 0;

    // Thread function to manage each client connection
    static void clientThreadFunction(Server* server, int clientSocket);

    void processCommand(std::string& command , int clientSocket, Graph& g);
};

#endif //SERVER_H
