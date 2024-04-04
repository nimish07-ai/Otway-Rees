#include "global/global.h"

//function to handel request from internet
void startServerCallback(const std::string& message, const std::string& clientAddress, int sock) {
    std::cout << "Received message from client at address: " << clientAddress << "\n";
    std::cout << "Message: " << message << std::endl;
    auto processedMessage = processMessage(message);
    sendMessage_preprocessor(processedMessage, sock);
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
                Message_Pool_check();
                break;
            }

            // Add Client
            case 1: {
                add_client();
                break;
            }

            // Establish Session Key With Client
            case 2: {
                establish_session_key_with_client(sock);
                break;
            }
            
            // Send Message To Client 
            case 3:{
                Send_Message_To_Client(sock);
                break;
            }
            
            //  Add New KDC
            case 4:{
                add_kdc();
                break;
            }
            // Establish SSK With KDC
            case 5:{
                Establish_SSK_With_KDC();
                break;}
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