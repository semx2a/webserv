#include <iostream>
#include <vector>
#include <thread>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

const int NUM_PORTS = 5;
const int NUM_REQUESTS = 10;

<<<<<<< HEAD
void handleRequest (int port) {

    int sockfd = socket (AF_INET, SOCK_STREAM, 0);
=======
void handleRequest(int port) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
>>>>>>> refs/remotes/origin/noemi
    if (sockfd < 0) {
        std::cerr << "Error opening socket." << std::endl;
        return;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
<<<<<<< HEAD
    server_addr.sin_port = htons (port);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (connect (sockfd, (struct sockaddr*)&server_addr, sizeof (server_addr)) < 0) {
        std::cerr << "Error connecting to the server on port " << port << std::endl;
        close (sockfd);
=======
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Error connecting to the server on port " << port << std::endl;
        close(sockfd);
>>>>>>> refs/remotes/origin/noemi
        return;
    }

    // Send some test data
<<<<<<< HEAD
    std::string message = "Hello, server on port " + std::to_string (port);
    send (sockfd, message.c_str (), message.size (), 0);

    // Receive response from the server
    char buffer [1024] = {0};
    recv (sockfd, buffer, sizeof (buffer), 0);
    std::cout << "Response from server on port " << port << ": " << buffer << std::endl;

    close (sockfd);
}

int main () {

    std::vector <std::thread> threads;
=======
    std::string message = "Hello, server on port " + std::to_string(port);
    send(sockfd, message.c_str(), message.size(), 0);

    // Receive response from the server
    char buffer[1024] = {0};
    recv(sockfd, buffer, sizeof(buffer), 0);
    std::cout << "Response from server on port " << port << ": " << buffer << std::endl;

    close(sockfd);
}

int main() {
    std::vector<std::thread> threads;
>>>>>>> refs/remotes/origin/noemi

    // Launch multiple threads to send requests on different ports
    for (int i = 0; i < NUM_PORTS; ++i) {
        int port = 8080 + i; // Replace this with your desired port range
        for (int j = 0; j < NUM_REQUESTS; ++j) {
<<<<<<< HEAD
            threads.emplace_back (handleRequest, port);
=======
            threads.emplace_back(handleRequest, port);
>>>>>>> refs/remotes/origin/noemi
        }
    }

    // Wait for all threads to finish
    for (auto& t : threads) {
<<<<<<< HEAD
        t.join ();
=======
        t.join();
>>>>>>> refs/remotes/origin/noemi
    }

    return 0;
}

