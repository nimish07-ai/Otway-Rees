#include <iostream>
#include <string>
#include <limits>
#include "shared_maps_methods.h"

void add_client(){
    myDisplayText.body = "in choice 1:";
    refreshScreen(myDisplayText);   
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
        return; // Exit the switch statement if input is invalid
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    // Ensure thread safety when accessing clientsMap
    {
        addClientToMap(clientName, clientIP, clientPort);
    }
}

void establish_session_key_with_client()
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