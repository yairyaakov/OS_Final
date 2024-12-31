#include <csignal>
#include "Network/include/LFserver.h"

Server* server = nullptr;

void signalHandler(int signal) {
	if (signal == SIGINT) {
		if (server) {
			server->stop();  // Stop the server gracefully
		}
	}
}

void setupSignalHandler() {
	std::signal(SIGINT, signalHandler);
}

int main() {
	setupSignalHandler();	
	
        server =  new LFServer("4050");
        server->run();
        delete server;
}



