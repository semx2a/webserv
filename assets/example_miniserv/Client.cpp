#include <iostream>
#include <vector>
#include <thread>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

const int NUM_PORTS = 5;
const int NUM_REQUESTS = 10;

void handleRequest(int threadId, int port) {
    for (int i = 0; i < NUM_REQUESTS; ++i) {
        int sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0) {
            std::cerr << "Thread " << threadId << ": Error opening socket." << std::endl;
            continue;
        }

		struct sockaddr_in server_addr;
		server_addr.sin_family = AF_INET;
		server_addr.sin_port = htons (port);
		server_addr.sin_addr.s_addr = INADDR_ANY;

		if (connect (sockfd, (struct sockaddr*)&server_addr, sizeof (server_addr)) < 0) {
			std::cerr << "Error connecting to the server on port " << port << std::endl;
			close (sockfd);
			return;
		}

		std::string message = "Hello, server on port " + std::to_string (port);
		send (sockfd, message.c_str (), message.size (), 0);

		char buffer [1024] = {0};
		recv (sockfd, buffer, sizeof (buffer), 0);
		std::cout << "Response from server on port " << port << ": " << buffer << std::endl;


        close(sockfd);
    }
}

int main() {
    std::vector<std::thread> threads;

    for (int i = 0; i < NUM_PORTS; ++i) {
        int port = 8080 + i;
        threads.emplace_back(handleRequest, i, port);
    }

    // Lancer tous les threads
    for (auto& t : threads) {
        t.join();
    }

    return 0;
}

