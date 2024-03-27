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


std::string getCurrentTimestamp() {
    // Get the current time point
    auto now = std::chrono::system_clock::now();

    // Convert the time point to a time_t object
    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);

    // Convert the time_t object to a string in the desired format
    std::stringstream ss;
    ss << std::put_time(std::localtime(&currentTime), "%Y-%m-%d %H:%M:%S");

    return ss.str();
}

std::mutex inputMutex;
std::condition_variable inputCV;
std::string userInput;
bool isStartServerTurn = false; 


std::mutex clientsMapMutex;
std::mutex sskMapMutex;
std::mutex Message_poolMutex; 
std::map<std::string, std::pair<std::string, int>> clientsMap;
std::map<std::string, std::string> ssk_map;
std::map<std::string, std::vector<std::string>> Message_pool;

std::map<std::string, std::pair<std::string, int>> kdcMap;

std::map<std::string,  std::vector<std::string>> kdc_longeterm_ssk_map;

std::mutex kdcMapMutex;
std::mutex kdc_longeterm_ssk_mutex;


void addClientToMap(const std::string& clientName, const std::string& clientIP, int clientPort) {
    // Ensure thread safety when accessing clientsMap
    std::lock_guard<std::mutex> lock(clientsMapMutex);

    // Add the client to the map
    clientsMap[clientName] = std::make_pair(clientIP, clientPort);
}

void addsskToMap(const std::string& clientName, const std::string& key) {
    // Ensure thread safety when accessing clientsMap
    std::lock_guard<std::mutex> lock(sskMapMutex);

    // Add the client to the map
    ssk_map[clientName] = key;
}

void addKDCToMap(const std::string& clientName, const std::string& clientIP, int clientPort) {
    // Ensure thread safety when accessing clientsMap
    std::lock_guard<std::mutex> lock(kdcMapMutex);

    // Add the client to the map
    kdcMap[clientName] = std::make_pair(clientIP, clientPort);
}

void addkdc_sskToMap(const std::string& clientName, const std::string& prime, const std::string& generator,const std::string& privateKey,const std::string& ssk) {
    // Ensure thread safety when accessing clientsMap
    std::lock_guard<std::mutex> lock(kdc_longeterm_ssk_mutex);
    
    std::vector<std::string> entry;
    entry.push_back(prime);
    entry.push_back(generator);
    entry.push_back(privateKey);
    entry.push_back(ssk);    
    // Add the client to the map
    kdc_longeterm_ssk_map[clientName] = entry;
}

void addMessagepool(const std::string& message, const std::string& name, const int msgcode) {
    std::lock_guard<std::mutex> lock(Message_poolMutex);

    // Construct the message entry as a vector
    std::vector<std::string> entry;
    entry.push_back(name);
    entry.push_back(std::to_string(msgcode));
    entry.push_back(message);

    // Add the message entry to the pool with the current timestamp as key
    Message_pool[getCurrentTimestamp()] = entry;
}


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