#include <iostream>
#include <string>
#include <thread>
#include <arpa/inet.h>
#include <vector>
#include <cstring>
#include <unistd.h>
#include <map>
#include <mutex> // For mutex
#include <iostream>   // For std::cerr
#include <string>     // For std::string
#include <thread>     // For std::thread
#include <arpa/inet.h> // For sockaddr_in, inet_addr, htons
#include <cstring>     // For memset, strlen
#include <unistd.h>    // For close
#include <sstream>

using namespace std;


std::mutex clientsMapMutex;


std::map<std::string, std::pair<std::string, int>> clientsMap;


std::string clientName;

std::map<std::string, std::string> extractValues(const std::vector<std::string>& keys, const std::string& message) {
    std::map<std::string, std::string> result;

    // Create a string stream from the message
    std::istringstream iss(message);

    // Iterate through keys and extract values from the message
    for (const auto& key : keys) {
        std::string value;
        if (std::getline(iss, value, ',')) {
            result[key] = value;
        }
    }

    return result;
}

// Define a structure to represent a packet
struct Packet {
    int client_state_id;
    std::string message;
};

// Function to format a packet into a string for sending
std::string formatPacket(const Packet& packet) {
    return std::to_string(packet.client_state_id) + "," + packet.message;
}

// Function to parse a string into a packet
Packet parsePacket(const std::string& packetString) {
    Packet packet;
    size_t pos = packetString.find(",");
    packet.client_state_id = std::stoi(packetString.substr(0, pos));
    packet.message = packetString.substr(pos + 1);
    return packet;
}

// Function to process the message based on client state
std::string processMessage(int clientState, const std::string& message) {
    if (clientState == 1) {
        // State 1: Format message as name, date, time
        return "name,dat,time"; // Replace with actual implementation
    } else if (clientState == 2) {
        // State 2: Format message as key
        return "key"; // Replace with actual implementation
    } else {
        // Default case
        return "Unknown state"; // Replace with appropriate handling
    }
}

void addClientToMap(const std::string& clientName, const std::string& clientIP, int clientPort) {
    // Ensure thread safety when accessing clientsMap
    std::lock_guard<std::mutex> lock(clientsMapMutex);

    // Add the client to the map
    clientsMap[clientName] = std::make_pair(clientIP, clientPort);
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

// Function to handle incoming messages from clients
void startServerCallback(const std::string& message, const std::string& clientAddress, int sock) {
    std::cout << "Received message from client at address: " << clientAddress << "\n";
    std::cout << "Message: " << message << std::endl;

    char options;
    std::cout << "Do you want to respond to this message? (Y / any other key): ";
    std::cin >> options;
    std::cin.ignore();

    if (options == 'Y' || options == 'y') {
        std::string sMessage;
        std::cout << "Enter your response: ";
        std::getline(std::cin, sMessage);

        // Convert client address to sockaddr_in
        struct sockaddr_in clientAddr;
        memset(&clientAddr, 0, sizeof(clientAddr));
        clientAddr.sin_family = AF_INET;
        clientAddr.sin_addr.s_addr = inet_addr(clientAddress.c_str());
        clientAddr.sin_port = htons(clientsMap[clientName].second);

        // Ensure thread safety when accessing clientsMap
        std::lock_guard<std::mutex> lock(clientsMapMutex);
        
        // Add the client to the map if not already present
        addClientToMap(clientName, clientAddress, clientsMap[clientName].second);

        if (sendMessage(sock, clientAddr, sMessage.c_str()) == 1) {
            std::cout << "Message sent successfully.\n";
        } else {
            std::cerr << "Failed to send message.\n";
        }
    }
}


// Function to handle menu-driven user interactions
void menuDrivenCallback(int sock) {
    int choice;
    while (true) {
        std::cout << "\nMenu:\n";
        std::cout << "1. Connect to client\n";
        std::cout << "2. Send message\n";
        std::cout << "3. Exit\n";
        std::cout << "Enter your choice: ";
        if (!(std::cin >> choice)) {
            std::cerr << "Error: Invalid input\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        std::cin.ignore();

        switch (choice) {
           case 1: {
                std::string clientName;
                std::string clientIP;
                int clientPort;
                
                std::cout << "Enter client name: ";
                std::getline(std::cin, clientName);
                
                std::cout << "Enter client IP address: ";
                std::getline(std::cin, clientIP);
                
                std::cout << "Enter client port: ";
                if (!(std::cin >> clientPort)) {
                    std::cerr << "Error: Invalid input\n";
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    break; // Exit the switch statement if input is invalid
                }
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                // Ensure thread safety when accessing clientsMap
                {
                    // std::lock_guard<std::mutex> lock(clientsMapMutex);
                    addClientToMap(clientName, clientIP, clientPort);
                }

                // Send the welcome message (uncomment if needed)
                // sendMessage(sock, clientIP, "hello");
                break;
            }


            case 2: {
                std::cout << "Available clients:\n";
                int i = 1;

                // Ensure thread safety when accessing clientsMap
                std::lock_guard<std::mutex> lock(clientsMapMutex);

                for (const auto& client : clientsMap) {
                    std::cout << i << ". " << client.first << std::endl;
                    i++;
                }

                int clientIndex;
                std::cout << "Select a client to send message: ";
                if (!(std::cin >> clientIndex)) {
                    std::cerr << "Error: Invalid input\n";
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    continue;
                }
                std::cin.ignore();

                if (clientIndex <= clientsMap.size()) {
                    std::string clientName = "";
                    int j = 1;

                    for (const auto& client : clientsMap) {
                        if (j == clientIndex) {
                            clientName = client.first;
                            break;
                        }
                        j++;
                    }

                    if (clientName != "") {
                        std::string sMessage;
                        std::cout << "Enter message to send: ";
                        std::getline(std::cin, sMessage);

                        struct sockaddr_in clientAddr;
                        memset(&clientAddr, 0, sizeof(clientAddr));
                        clientAddr.sin_family = AF_INET;
                        clientAddr.sin_addr.s_addr = inet_addr(clientsMap[clientName].first.c_str());
                        clientAddr.sin_port = htons(clientsMap[clientName].second);

                        // Send message to selected client
                        if (sendMessage(sock, clientAddr, sMessage.c_str()) == 1) {
                            std::cout << "Message sent successfully.\n";
                        } else {
                            std::cout << "Failed to send message.\n";
                        }
                    } else {
                        std::cout << "Invalid client selection.\n";
                    }
                } else {
                    std::cout << "Invalid client selection.\n";
                }
                break;
            }
            case 3:
                return;
            default:
                std::cout << "Invalid choice. Please try again.\n";
        }
    }
}

// Main function
int main() {
    const char* serverIpAddress = "127.0.0.1";
    int serverPort;

    std::cout << "Welcome! This is a client program.\n";
    std::cout << "Please enter your name: ";
    std::getline(std::cin, clientName);
    std::cout << "Enter the port number: ";
    std::cin >> serverPort;

    // Initialize client and start the server in a separate thread
    if (initializeClient(serverIpAddress, serverPort, menuDrivenCallback, startServerCallback) == -1) {
        return -1;
    }

    return 0;
}
