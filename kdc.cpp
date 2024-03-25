#include <iostream>
#include <string>
#include <thread>
#include <cstring>
#include <unistd.h>
#include <map>
#include <mutex>       // For mutex
#include <iostream>    // For std::cerr
#include <string>      // For std::string
#include <thread>      // For std::thread
#include <arpa/inet.h> // For sockaddr_in, inet_addr, htons
#include <cstring>     // For memset, strlen
#include <unistd.h>    // For close

#include "Csocket.h" 
#include "message.h"
#include "displayFunctions.h"
std::string clientName;
DisplayText myDisplayText;
const int keysize=256;

// Function to handle incoming messages from clients
void startServerCallback(const std::string& message, const std::string& clientAddress, int sock) {
    std::cout << "Received message from client at address: " << clientAddress << "\n";
    std::cout << "Message: " << message << std::endl;

    // Process the received message
    auto processedMessage = processMessage(message,clientName,keysize);
    std::vector<std::string> additionalData = processedMessage.first;
    std::string sMessage = processedMessage.second;

    if (!additionalData.empty()) {
        // Extract client IP and port from the additional data
        std::string clientIP = additionalData[0];
        int clientPort = std::stoi(additionalData[1]); // Convert port to integer

        // Create client address structure
        struct sockaddr_in clientAddr;
        memset(&clientAddr, 0, sizeof(clientAddr));
        clientAddr.sin_family = AF_INET;
        clientAddr.sin_addr.s_addr = inet_addr(clientIP.c_str());
        clientAddr.sin_port = htons(clientPort);

        // Send message to the client
        if (sendMessage(sock, clientAddr, sMessage.c_str()) == 1) {
            std::cout << "Message sent successfully.\n";
        } else {
            std::cerr << "Failed to send message.\n";
        }
    } else {
        std::cerr << "Empty output from processMessage. Not sending any message.\n";
    }
}


// Function to handle menu-driven user interactions
void menuDrivenCallback(int sock) {
    int choice;
    while (true) {


        myDisplayText.body = "\nMenu:\nEnter Your Choice: \n0. Check Message Pool \n1. Add Client\n2. Establish Session Key With Client \n3. Send Message To Client \n4. Add New KDC\n5. Establish SSK With KDC\n";

        refreshScreen(myDisplayText);
        if (!(std::cin >> choice)) {
            refreshScreen(myDisplayText);   
            std::cerr << "Error: Invalid input\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        std::cin.ignore();

        switch (choice) {
            // Check Message Pool
           case 0: {
            // Display all message orders according to timestamp, with the latest first
            std::cout << "All messages in the pool:\n";
            int index = 1;
            for (auto it = Message_pool.rbegin(); it != Message_pool.rend(); ++it) {
                const std::vector<std::string>& entry = it->second;
                std::cout << index << ". " << it->first << " " << entry[0] << " " << entry[1] << std::endl;
                index++;
            }

            // Allow the user to select a number corresponding to a message
            int selectedMessage;
            std::cout << "Select a message number to view details (0 to cancel): ";
            std::cin >> selectedMessage;

            if (selectedMessage == 0) {
                break; // Exit the switch statement if the user cancels
            } else {
                // Display the entire message when the user selects a number
                auto it = Message_pool.rbegin(); // Start from the latest message
                std::advance(it, selectedMessage - 1); // Move to the selected message
                const std::vector<std::string>& selectedEntry = it->second;
                std::cout << "Selected message details:\n";
                std::cout << "Timestamp: " << it->first << std::endl;
                std::cout << "Name: " << selectedEntry[0] << std::endl;
                std::cout << "Code: " << selectedEntry[1] << std::endl;
                std::cout << "Message: " << selectedEntry[2] << std::endl;
            }

            // Wait for 'q' to be pressed to quit
            char input;
            std::cout << "Press 'q' to quit: ";
            std::cin >> input;
            
            if (input == 'q') {
                break; // Exit the switch statement if 'q' is pressed
            }
            break;
        }


            // Add Client
            case 1: {
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
                    break; // Exit the switch statement if input is invalid
                }
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                // Ensure thread safety when accessing clientsMap
                {
                    addClientToMap(clientName, clientIP, clientPort);
                }
                break;
            }

            // Establish Session Key With Client
            case 2: {
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
                        // TODO: update this to 
                        clientAddr.sin_port = htons(9134);

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
           
             // Send Message To Client 
            case 3:{
                break;
            }
            //  Add New KDC
            case 4:{
                break;
            }
            // Establish SSK With KDC
            case 5:{break;}
            // case 10: {
            //     // Accept receiver's IP address and port
            //     std::string receiverIP;
            //     int receiverPort;

            //     std::cout << "Enter receiver's IP address: ";
            //     std::getline(std::cin, receiverIP);
                
            //     std::cout << "Enter receiver's port: ";
            //     if (!(std::cin >> receiverPort)) {
            //         std::cerr << "Error: Invalid input\n";
            //         std::cin.clear();
            //         std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            //         break; // Exit the switch statement if input is invalid
            //     }
            //     std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            //     // Generate message body
            //     std::vector<std::vector<std::string>> body;
            //     body.push_back({GenerateGlobalParametersAndHalfKey(512)});
            //     int keySize = TestKeySize;
            //     std::string plainText = TestPlainText;
            //     SecByteBlock key = GenerateAESKey(keySize);
            //     body.push_back({SecByteBlockToString(key), plainText, Encrypt(plainText, key)});

            //     // Concatenate header and body into a single string
            //     std::vector<std::string> head = {serverIpAddress, std::to_string(serverPort), receiverIP, std::to_string(receiverPort), "0"}; // Assuming message code is 0 for this case
            //     std::string concatenatedString = concatenateHeadAndBody(head, body);

            //     // Send the concatenated message
            //     // Create sockaddr_in for the receiver
            //     struct sockaddr_in receiverAddr;
            //     memset(&receiverAddr, 0, sizeof(receiverAddr));
            //     receiverAddr.sin_family = AF_INET;
            //     receiverAddr.sin_addr.s_addr = inet_addr(receiverIP.c_str());
            //     receiverAddr.sin_port = htons(receiverPort);

            //     // Send message to the receiver
            //     if (sendMessage(sock, receiverAddr, concatenatedString.c_str()) == 1) {
            //         std::cout << "Message sent successfully.\n";
            //     } else {
            //         std::cerr << "Failed to send message.\n";
            //     }

            //     break;
            // }
            // Exit           
            case 6:
                return;

            default:
                std::cout << "Invalid choice. Please try again.\n";
        }
    }
}

void menuDrivenCallback_rr(int sock) {
    cout<<"hhehe";
}

// Main function
int main() {
    startup();
    const char* serverIpAddress = "127.0.0.1";
    int serverPort;
    system("clear");

    std::cout << "Welcome! This is a kdc-client program.\n";
    std::cout << "Please enter your name: ";
    std::getline(std::cin, clientName);
    std::cout << "Enter the port number: ";
    std::cin >> serverPort;

    myDisplayText.title = "Otway-Rees(KDC application)";
    myDisplayText.notification = "0";
    myDisplayText.header = "KDC Name: " + clientName + "\nip-address: " + serverIpAddress + "\nport No: " + std::to_string(serverPort);
    myDisplayText.body = "Body: This is the body. Line 1.\nLine 2.\nLine 3.\nLine 4.\nLine 5.\nLine 6.\nLine 7.\nLine 8.\nLine 9.";
    myDisplayText.footer = "Footer: This is the footer";
    myDisplayText.error = "";

    // Initialize client and start the server in a separate thread
    if (initializeClient(serverIpAddress, serverPort, menuDrivenCallback, startServerCallback) == -1) {
        return -1;
    }

    return 0;
}