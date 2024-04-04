#include <iostream>
#include <string>
#include <limits>
#include "shared_maps_methods.h"


void add_client() {
    myDisplayText.body = "in choice 1:";
    refreshScreen(myDisplayText);

    std::string clientName;
    std::string clientIP;
    int clientPort;

    std::cout << "Enter client name: ";
    std::getline(std::cin, clientName);

    // Check if client with the same name exists
    {
        std::lock_guard<std::mutex> lock(clientsMapMutex);
        if (clientsMap.find(clientName) != clientsMap.end()) {
            cout<<"\033[1;31mclient with the name " + clientName + " already exists\033[0m";
            std::this_thread::sleep_for(std::chrono::seconds(5));
            return; // Exit the function if client already exists
        }
    }

    std::cout << "Enter client IP address: ";
    std::getline(std::cin, clientIP);

    std::cout << "Enter client port: ";
    if (!(std::cin >> clientPort)) {
        std::cerr << "Error: Invalid input\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return; // Exit the function if input is invalid
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    // Add the client to the map
    addClientToMap(clientName, clientIP, clientPort);

    cout<<"in choice 1: client " + clientName + " added successfully";
    std::this_thread::sleep_for(std::chrono::seconds(5));
}


void establish_session_key_with_client(int sock)
{
        myDisplayText.body = "in choice 2:";
        refreshScreen(myDisplayText);
        std::cout << "Available clients:\n";
        int i = 1;

        // Ensure thread safety when accessing clientsMap
        std::lock_guard<std::mutex> lock(clientsMapMutex);

        for (const auto& client : clientsMap) {
            std::cout << i << ". " << client.first << std::endl;
            i++;
        }

        int clientIndex;
        std::cout << "Select a client to establish ssk: ";
        if (!(std::cin >> clientIndex)) {
            std::cerr << "Error: Invalid input\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
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
                // TODO: update this to 
                clientAddr.sin_port = htons(9134);

                // Send message to selected client
                if (sendMessage(sock, clientAddr, sMessage.c_str()) == 1) {
                    std::cout << "Request sent successfully.\n";
                } else {
                    std::cout << "Failed to send Request.\n";
                }
            } else {
                std::cout << "Invalid client selection.\n";
            }
        } else {
            std::cout << "Invalid client selection.\n";
        }
}


void Message_Pool_check() {
    std::lock_guard<std::mutex> lock(Message_poolMutex);
    
    // Reverse iterate over the Message_pool map
    for (auto it = Message_pool.rbegin(); it != Message_pool.rend(); ++it) {
        const std::vector<std::string>& entry = it->second;
        std::cout << "Timestamp: " << it->first << std::endl;
        std::cout << "Name: " << entry[0] << std::endl;
        std::cout << "Message Code: " << entry[1] << std::endl;
        std::cout << "Message: " << entry[2] << std::endl;
        std::cout << std::endl;
    }
}


void Send_Message_To_Client(int sock) {
    std::cout << "Available clients:\n";
    int i = 1;

    // Ensure thread safety when accessing clientsMap
    std::lock_guard<std::mutex> lock(clientsMapMutex);

    // Print name, IP address, and port of the clients in the map
    for (const auto& client : clientsMap) {
        std::cout << i << ". Name: " << client.first << ", IP: " << client.second.first << ", Port: " << client.second.second << std::endl;
        i++;
    }

    int clientIndex;
    std::cout << "Select a client to send message: ";
    if (!(std::cin >> clientIndex)) {
        std::cerr << "Error: Invalid input\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        // If the user enters an invalid input, let them leave
        return;
    }
    std::cin.ignore();

    if (clientIndex <= clientsMap.size()) {
        std::string clientName = "";
        int j = 1;

        // Find the selected client by index
        for (const auto& client : clientsMap) {
            if (j == clientIndex) {
                clientName = client.first;
                break;
            }
            j++;
        }

        if (!clientName.empty()) {
            int choice;
            std::cout << "Enter choice (1 for plain text, 2 for encrypted text): ";
            if (!(std::cin >> choice)) {
                std::cerr << "Error: Invalid input\n";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                // If the user enters an invalid input, let them leave
                return;
            }
            std::cin.ignore();

            if (choice == 1) {
                // User selected plain text
                std::cout << "Sending plain text message...\n";
                
                // Implement logic to send plain text message
            } else if (choice == 2) {
                // User selected encrypted text
                if (ssk_map.find(clientName) != ssk_map.end()) {
                    // Client exists in the ssk_map
                    std::cout << "Sending encrypted message...\n";
                    // sendMessage_preprocessor(send_400(serverIpAddress,senderport,receiverip,receiverport, message),sock );
                    // Implement logic to send encrypted message
                } else {
                    // Client does not exist in the ssk_map
                    std::cout << "Client is not encrypted. Cannot send encrypted message.\n";
                }
            } else {
                std::cout << "Invalid choice. Please enter 1 for plain text or 2 for encrypted text.\n";
            }
        } else {
            std::cout << "Invalid client selection.\n";
        }
    } else {
        std::cout << "Invalid client selection.\n";
    }
}

void add_kdc() {
    // Display message
    std::cout << "In choice 4: Adding KDC entry\n";

    std::string clientName;
    std::string clientIP;
    int clientPort;

    // Get input from user
    std::cout << "Enter KDC name: ";
    std::getline(std::cin, clientName);

    // Check if KDC with the same name exists
    {
        std::lock_guard<std::mutex> lock(kdcMapMutex);
        if (kdcMap.find(clientName) != kdcMap.end()) {
            std::cout << "\033[1;31mKDC with the name " + clientName + " already exists\033[0m\n";
            std::this_thread::sleep_for(std::chrono::seconds(5));
            return; // Exit the function if KDC already exists
        }
    }

    std::cout << "Enter KDC IP address: ";
    std::getline(std::cin, clientIP);

    std::cout << "Enter KDC port: ";
    if (!(std::cin >> clientPort)) {
        std::cerr << "Error: Invalid input\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return; // Exit the function if input is invalid
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    // Add the KDC entry to the map
    addKDCToMap(clientName, clientIP, clientPort);

    std::cout << "In choice 1: KDC " + clientName + " added successfully\n";
    std::this_thread::sleep_for(std::chrono::seconds(5));
}

void Establish_SSK_With_KDC()
{
    
}