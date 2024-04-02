#include "global/global.h"

void startServerCallback(const std::string& message, const std::string& clientAddress, int sock) {
    std::cout << "Received message from client at address: " << clientAddress << "\n";
    std::cout << "Message: " << message << std::endl;

    // Process the received message
    auto processedMessage = processMessage(message);
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
     
            break;
        }


        // Add Client
        case 1: {
    
            break;
        }

        // Establish Session Key With Client
        case 2: {
            
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
            case 6:
                return;

            default:
                std::cout << "Invalid choice. Please try again.\n";
        }
    }
}

// Main function
int main() {
    startup();
    const char* serverIpAddress = "127.0.0.1";
    int serverPort;
    system("clear");

    std::cout << "Welcome! This is a client program.\n";
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