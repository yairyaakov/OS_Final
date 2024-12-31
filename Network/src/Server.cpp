#include <utility>

#include "../include/Server.h"


// Constructor
Server::Server(std::string port)
    : listener(-1), port(port) {
}

void Server::run() {
    // Set up the listener socket once
    listener = getListenerSocket();
    if (listener < 0) {
        std::cerr << "Failed to set up the server listener." << std::endl;
        return;
    }
    std::cout<<"Server is listening on port: "<<port<<std::endl;
    std::cout<<"Waiting for connections..."<<std::endl;

    int clientSocket = -1;
    while (keepRunning) {
        struct sockaddr_storage remoteaddr;
        socklen_t addrlen = sizeof(remoteaddr);
        clientSocket = accept(listener, (struct sockaddr*)&remoteaddr, &addrlen);
        if (clientSocket == -1) {
            if (keepRunning) {  // Check if server is still running
                perror("accept");
            }
            continue;
        }

        
        {
            std::lock_guard<std::mutex> lock(graphMutex);
            std::cout << "New connection established" << std::endl;
            // Start a new thread to handle the client
            clientThreads.emplace_back(clientThreadFunction, this, clientSocket);
        }
    }
}



// Set up a listener socket
int Server::getListenerSocket() {
    int listener; // Listening socket descriptor
    int yes = 1; // For setsockopt() SO_REUSEADDR, below
    int rv;

    struct addrinfo hints, *ai, *p;

    // Get us a socket and bind it
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    if ((rv = getaddrinfo(nullptr, port.c_str(), &hints, &ai)) != 0) {
        std::cerr << "selectserver: " << gai_strerror(rv) << std::endl;
        exit(1);
    }

    for (p = ai; p != nullptr; p = p->ai_next) {
        listener = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (listener < 0) {
            continue;
        }

        // Lose the pesky "address already in use" error message
        setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

        if (bind(listener, p->ai_addr, p->ai_addrlen) < 0) {
            close(listener);
            continue;
        }

        break;
    }

    freeaddrinfo(ai); // All done with this

    // If we got here, it means we didn't get bound
    if (p == nullptr) {
        return -1;
    }

    // Listen
    if (listen(listener, 10) == -1) {
        return -1;
    }

    return listener;
}



// Client utils
void Server::sendResponse(int clientSocket, const std::string &response) {
    if (send(clientSocket, response.c_str(), response.size(), 0) == -1) {
        perror("send");
    }
}

void Server::handleClient(int clientSocket) {
    Graph g;
    char buf[256];
    while (true) {
        memset(buf, 0, sizeof(buf));
        int nbytes = recv(clientSocket, buf, sizeof(buf), 0);
        if (nbytes <= 0) {
            if (nbytes == 0) {
                std::lock_guard<std::mutex> lock(graphMutex);
                std::cout << "Client disconnected." << std::endl;
            }
            else {
                perror("recv");
            }
            close(clientSocket);
            break;
        }

        buf[nbytes] = '\0';
        std::string command(buf);
        std::string response;

        processCommand(command,clientSocket, g);
    }
}

void Server::processCommand(std::string &command, int clientSocket, Graph& g) {
    command.erase(std::ranges::remove(command, '\n').begin(), command.end());
    {
        std::lock_guard<std::mutex> lock(graphMutex);
        std::cout<<command<<" received from client"<<std::endl;
    }
    
    if (command == "NewGraph")
        newGraph(clientSocket, g);

    else if (command == "NewEdge" || command == "RemoveEdge" || command == "prim" || command == "kruskal") {
    
        // Check if the graph is empty
        if(g.isEmpty()) {
            std::string response = "Cannot perform '" + command + "' on an empty graph.\n";
            std::cout << response;
            
            if (send(clientSocket, response.c_str(), response.size(), 0) == -1) {
                perror("send");
            }
        }
        else if (command == "NewEdge")
            addEdge(clientSocket, g);

        else if (command == "RemoveEdge")
            removeEdge(clientSocket, g);

        else if (command == "prim" || command == "kruskal") {
            std::unique_ptr<MSTAlgorithm> algo = MSTFactory::createAlgorithm(command, g);
            MST mst = algo->findMST();
            std::string mstEdges = mst.getMstEdges();
            send(clientSocket, mstEdges.c_str(), mstEdges.size(), 0);
            performMSTComputation(clientSocket,mst);
        }
    }

    
    else {
        std::string response = "Invalid command: " + command+"\n";
        {
            std::lock_guard<std::mutex> lock(graphMutex);
            std::cout <<response;
        }
        
        if (send(clientSocket, response.c_str(), response.size(), 0) == -1) {
            perror("send");
        }
    }
}

void Server::clientThreadFunction(Server *server, int clientSocket) {
    server->handleClient(clientSocket);
}


// Shared graph utils
void Server::newGraph(int clientSocket, Graph& g) {
    try{
        int n, m;
        {
            std::lock_guard<std::mutex> lock(graphMutex);

            // redirect the inputs into the client socket
            dup2(clientSocket, STDIN_FILENO);

            std::cin >> n >> m;
            g = Graph(n, m);        
        }
        std::string response = "Client created a new graph with " + std::to_string(n) + " nodes and " + std::to_string(m) +
                               " edges.\n";
        sendResponse(clientSocket, response);
    } catch(const std::invalid_argument& e){
        std::string errorMessage = std::string(e.what()) + "\n";
        sendResponse(clientSocket, errorMessage);
    }
    
}

void Server::addEdge(int clientSocket, Graph& g) {
    int u, v, w;
    {
            std::lock_guard<std::mutex> lock(graphMutex);

            // redirect the inputs into the client socket
            dup2(clientSocket, STDIN_FILENO);

            std::cin >> u >> v >> w;
    }

    if(!g.isValidInput(u , v , w , ADD_EDGE)) {
        sendResponse(clientSocket, "Please insert a valid input i.e no negative weight and vertices in range 0 and " + std::to_string(g.getNumberOfVertices()) + "\n");
        return;
    }

    g.addEdge(u, v, w);
    sendResponse(clientSocket, "Edge added.\n");
}

void Server::removeEdge(int clientSocket, Graph& g) {
    int u, v;
    {
            std::lock_guard<std::mutex> lock(graphMutex);

            // redirect the inputs into the client socket
            dup2(clientSocket, STDIN_FILENO);

            std::cin >> u >> v;
    }

    if(!g.isValidInput(u , v , 0, REMOVE_EDGE)) {
        sendResponse(clientSocket, "Please insert a valid input i.e no negative vertices\n");
        return;
    }
    g.removeEdge(u, v);
    sendResponse(clientSocket, "Edge removed.\n");
}

std::pair<int, int> Server::getVerticesForShortestPath(int clientSocket) {
    int u, v;
    std::string sendToClient = "Enter two vertices to find shortest path using at least one mst edge\n";
    sendResponse(clientSocket,sendToClient);

    {
            std::lock_guard<std::mutex> lock(graphMutex);

            // redirect the inputs into the client socket
            dup2(clientSocket, STDIN_FILENO);

             std::cin >> u >> v;
    }
   
    return std::make_pair(u, v);
}
