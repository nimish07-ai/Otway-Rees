#include "Csocket.h" // Include your own header file
#include <iostream> 
#include <string> 
#include <map> 
#include <mutex> 
#include <netinet/in.h> 
#include <arpa/inet.h> 
#include <sys/socket.h> 
#include <cstring> 
#include <condition_variable>
#include <vector> 
#include <unistd.h> 
#include <thread> 
#include <sstream> 
#include <iomanip>
#include <random>
#include <chrono>
#include <ctime>
#include <iomanip>


using namespace std;
int createSocket(const char* ipAddress, int port) {
    int sock = socket(AF_INET, SOCK_DGRAM, 0); // Change SOCK_STREAM to SOCK_DGRAM
    if (sock == -1) {
        std::cerr << "Error: Could not create socket\n";
        return -1;
    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(ipAddress);
    serverAddr.sin_port = htons(port);

    if (bind(sock, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "Error: Bind failed\n";
        close(sock);
        return -1;
    }

    return sock;
}

void startServer(void (*callback)(const std::string&, const std::string&, int), int sock) {
    std::cout << "Server listening\n";

    struct sockaddr_in clientAddr;
    socklen_t clientAddrSize = sizeof(clientAddr);
    while (true) {
        char buffer[1024] = {0};
        int bytesRead = recvfrom(sock, buffer, sizeof(buffer), 0, (struct sockaddr *)&clientAddr, &clientAddrSize);
        if (bytesRead == -1) {
            std::cerr << "Error: Failed to receive message\n";
            continue;
        }
        buffer[bytesRead] = '\0'; // Null-terminate the received message
        std::string clientAddress = inet_ntoa(clientAddr.sin_addr);
        callback(buffer, clientAddress, sock); // Pass the received message and client address to the callback function
    }
}

int sendMessage(int sock, const sockaddr_in& clientAddr, const char* message) {
    cout << "Sending message to client at address: " << inet_ntoa(clientAddr.sin_addr) << "\n";
    if (sendto(sock, message, strlen(message), 0, (const struct sockaddr *)&clientAddr, sizeof(clientAddr)) == -1) {
        std::cerr << "Error: Sending message failed\n";
        return 0;
    }
    return 1;
}

int initializeClient(const char* serverIpAddress, int serverPort, void (*menuDrivenCallback)(int), void (*startServerCallback)(const std::string&, const std::string&, int)) {
    int clientSocket = createSocket(serverIpAddress, serverPort);
    if (clientSocket == -1) {
        std::cerr << "Error: Failed to create client socket\n";
        return -1;
    }

    // Start the server in a separate thread
    std::thread(startServer, startServerCallback, clientSocket).detach();

    // Call the menu-driven callback function and pass the client socket
    menuDrivenCallback(clientSocket);

    // Close the client socket
    close(clientSocket);

    return 0;
}