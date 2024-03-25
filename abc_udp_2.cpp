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
#include <unistd.h>
DisplayText myDisplayText;
const std::string TestPlainText = "This is a test message."; // Example test plain text
const int TestKeySize = 256; // Example test key size (in bits)


using namespace std;
std::string clientName;
const char* serverIpAddress ;
int serverPort;

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
            case 10: {
                // Accept receiver's IP address and port
                std::string receiverIP;
                int receiverPort;

                std::cout << "Enter receiver's IP address: ";
                std::getline(std::cin, receiverIP);
                
                std::cout << "Enter receiver's port: ";
                if (!(std::cin >> receiverPort)) {
                    std::cerr << "Error: Invalid input\n";
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    break; // Exit the switch statement if input is invalid
                }
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                // Generate message body
                std::vector<std::vector<std::string>> body;
                body.push_back({GenerateGlobalParametersAndHalfKey(512)});
                int keySize = TestKeySize;
                std::string plainText = TestPlainText;
                SecByteBlock key = GenerateAESKey(keySize);
                body.push_back({SecByteBlockToString(key), plainText, Encrypt(plainText, key)});

                // Concatenate header and body into a single string
                std::vector<std::string> head = {serverIpAddress, std::to_string(serverPort), receiverIP, std::to_string(receiverPort), "0"}; // Assuming message code is 0 for this case
                std::string concatenatedString = concatenateHeadAndBody(head, body);

                // Send the concatenated message
                // Create sockaddr_in for the receiver
                struct sockaddr_in receiverAddr;
                memset(&receiverAddr, 0, sizeof(receiverAddr));
                receiverAddr.sin_family = AF_INET;
                receiverAddr.sin_addr.s_addr = inet_addr(receiverIP.c_str());
                receiverAddr.sin_port = htons(receiverPort);

                // Send message to the receiver
                if (sendMessage(sock, receiverAddr, concatenatedString.c_str()) == 1) {
                    std::cout << "Message sent successfully.\n";
                } else {
                    std::cerr << "Failed to send message.\n";
                }

                break;
            }
            // Exit           
            case 6:
                return;

            default:
                std::cout << "Invalid choice. Please try again.\n";
        }
    }
}
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
std::map<std::string, std::string> kdc_longeterm_ssk_map;

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

void addkdc_sskToMap(const std::string& clientName, const std::string& key) {
    // Ensure thread safety when accessing clientsMap
    std::lock_guard<std::mutex> lock(kdc_longeterm_ssk_mutex);

    // Add the client to the map
    kdc_longeterm_ssk_map[clientName] = key;
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
    // std::thread(menuDrivenCallback, clientSocket).detach();

    // Call the menu-driven callback function and pass the client socket
    menuDrivenCallback(clientSocket);

    // Close the client socket
    close(clientSocket);

    return 0;
}



#include "diffiehelman.h"
#include <iostream>
#include <vector>
#include <string>
#include <cryptopp/dh.h>
#include <cryptopp/osrng.h>
#include <cryptopp/hex.h> // Include the necessary Crypto++ header for HexEncoder and StringSink

using namespace CryptoPP;
using namespace std;

string IntegerToString(const Integer& p) {
    return IntToString<Integer>(p, 10);
}

std::vector<std::string> GenerateGlobalParametersAndHalfKey(int keySize) {
    AutoSeededRandomPool rnd;

    // Generate DH parameters
    DH dh;
    dh.AccessGroupParameters().GenerateRandomWithKeySize(rnd, keySize);

    // Generate a private key
    Integer privateKey;
    privateKey.Randomize(rnd, Integer::One(), dh.GetGroupParameters().GetSubgroupOrder() - Integer::One());

    // Compute the half key
    Integer halfKey = dh.GetGroupParameters().ExponentiateBase(privateKey);

    // Store global parameters and half key in a vector of strings
    std::vector<std::string> result;
    result.push_back("Prime: " + IntegerToString(dh.GetGroupParameters().GetModulus()));
    result.push_back("Generator: " + IntegerToString(dh.GetGroupParameters().GetGenerator()));
    result.push_back("Half Key: " + IntegerToString(halfKey));

    return result;
}

std::string GenerateFullKeyFromParameters(const std::vector<std::string>& paramsAndHalfKey) {
    // Retrieve parameters and half key from the vector
    const std::string& primeStr = paramsAndHalfKey[0].substr(paramsAndHalfKey[0].find(": ") + 2);
    const std::string& generatorStr = paramsAndHalfKey[1].substr(paramsAndHalfKey[1].find(": ") + 2);
    const std::string& halfKeyStr = paramsAndHalfKey[2].substr(paramsAndHalfKey[2].find(": ") + 2);

    // Convert parameters and half key back to their respective types
    Integer prime(primeStr.c_str());
    Integer generator(generatorStr.c_str());
    Integer halfKey(halfKeyStr.c_str());

    // Create DH parameters
    DH dh;
    dh.AccessGroupParameters().Initialize(prime, generator);

    // Generate a private key
    AutoSeededRandomPool rnd;
    Integer privateKey;
    privateKey.Randomize(rnd, Integer::One(), dh.GetGroupParameters().GetSubgroupOrder() - Integer::One());

    // Compute the full key
    Integer fullKey = dh.GetGroupParameters().ExponentiateBase(privateKey) + halfKey;

    // Convert the full key to a string
    std::string fullKeyStr = IntegerToString(fullKey);

    return fullKeyStr;
}

Integer StringToInteger(const std::string& str) {
    return Integer(str.c_str());
}

SecByteBlock StringToSecByteBlock(const string& str)
{
    return SecByteBlock(reinterpret_cast<const CryptoPP::byte*>(str.data()), str.size());
}

string SecByteBlockToString(const SecByteBlock& block)
{
    string str;
    HexEncoder encoder(new StringSink(str));

    encoder.Put(block, block.size());
    encoder.MessageEnd();

    return str;
}

// int main() {
//     // Generate global parameters and half key
//     std::vector<std::string> paramsAndHalfKey = GenerateGlobalParametersAndHalfKey(512);

//     // Print the vector obtained
//     std::cout << "Generated global parameters and half key:\n";
//     for (const auto& param : paramsAndHalfKey) {
//         std::cout << param << std::endl;
//     }

//     // Generate full key from global parameters and half key
//     std::string fullKeyStr = GenerateFullKeyFromParameters(paramsAndHalfKey);
#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>
#include <mutex>
#include <string>
#include <unistd.h> // for POSIX API
#include <sys/ioctl.h> // for ioctl system call
#include <csignal> // for signal handling
#include <sstream>
using namespace std;

atomic<bool> running{true};
mutex notificationMutex;


struct DisplayText {
    string title;
    string notification;
    string error;
    string header;
    string body;
    string footer;
};
// DisplayText myDisplayText;

// Function to get the terminal window width
int getWindowWidth() {
    struct winsize size;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
    return size.ws_col;
}

// Get the height of the terminal window
int getWindowHeight() {
    struct winsize size;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
    return size.ws_row;
}

// Count the number of lines in a string
int countLines(const std::string& text) {
    std::istringstream iss(text);
    int count = 0;
    std::string line;
    while (std::getline(iss, line)) {
        count++;
    }
    return count;
}


// Function to refresh the screen
void refreshScreen(const DisplayText& displayText) {
    // Clear the screen
    system("clear");

    // Calculate the indentation needed for centering the title
    int windowWidth = getWindowWidth();
    int indentation = (windowWidth - displayText.title.length()) / 2;

    // Calculate the indentation for the notification text
    int notificationIndentation = windowWidth - displayText.notification.length() - 4; // Adjust for space

    // Print the title centered on the screen
    cout << string(1, '\n'); // Move cursor to the middle of the screen
    cout << string(indentation, ' ') << displayText.title << endl;

    // Print the notification aligned to the extreme right
    cout << string(notificationIndentation, ' ') << "\u2709\ufe0f " << displayText.notification << endl;

    // Print the header
    cout << "\n" << displayText.header << endl;

    // Print the body
    cout << "\n " << displayText.body << endl;

    // Print the footer
    // cout << "\n " << displayText.footer << endl;
}



void welcomeFunction() {
    // Clear the screen
    system("clear");

    string asciiArt = R"(
       .aMMMb dMMMMMMP dMP dMP dMP .aMMMb  dMP dMP 
      dMP"dMP   dMP   dMP dMP dMP dMP"dMP dMP.dMP  
     dMP dMP   dMP   dMP dMP dMP dMMMMMP  VMMMMP   
    dMP.aMP   dMP   dMP.dMP.dMP dMP dMP dA .dMP    
    VMMMP"   dMP    VMMMPVMMP" dMP dMP  VMMMP"     
                                                   
        dMMMMb  dMMMMMP dMMMMMP .dMMMb             
       dMP.dMP dMP     dMP     dMP" VP             
      dMMMMK" dMMMP   dMMMP    VMMMb               
     dMP"AMF dMP     dMP     dP .dMP               
    dMP dMP dMMMMMP dMMMMMP  VMMMP"                
    )";    

    // Print the ASCII art line by line with a short delay
    for (char c : asciiArt) {
        // Print the character
        cout << c << flush;

        // Sleep for a short duration to simulate animation
        this_thread::sleep_for(chrono::milliseconds(10));
    }

    // Wait for 3-4 seconds
    this_thread::sleep_for(chrono::seconds(3));
}
void handleResize(int signal) {
    refreshScreen({"Title", "Notification", "Header", "Body", "Footer"});
}


void startup()
{
    welcomeFunction();
    signal(SIGWINCH, handleResize);

}




// int main() {
//     // Call the startup function to display the welcome message
//     startup();

//     // Initialize text content using the structure
//     DisplayText myDisplayText;
//     myDisplayText.title = "Otway-Rees";
//     myDisplayText.notification = "10";
//     myDisplayText.header = "Header: This is the header";
//     myDisplayText.body = "Body: This is the body. Line 1.\nLine 2.\nLine 3.\nLine 4.\nLine 5.\nLine 6.\nLine 7.\nLine 8.\nLine 9.";
//     myDisplayText.footer = "Footer: This is the footer";

//     // Main loop to refresh the screen
//     while (running) {
//         // Call the refreshScreen function to update the display
//         refreshScreen(myDisplayText);

//         // Wait for user input before exiting
//         cout << "Press Enter to exit...";
//         cin.get();
//         running = false;
//     }

//     return 0;
// }

//     // Convert full key string to Integer
//     Integer fullKey = StringToInteger(fullKeyStr);

//     // Convert full key string to SecByteBlock
//     SecByteBlock fullKeyBlock = StringToSecByteBlock(fullKeyStr);

//     // Output the full key
//     std::cout << "Full Key (Integer): " << fullKey << std::endl;
//     std::cout << "Full Key (SecByteBlock): " << SecByteBlockToString(fullKeyBlock) << std::endl;

//     return 0;
// }



int main() {
    // startup();
    serverIpAddress = "127.0.0.1";
    serverPort = 9134;
    // system("clear");
    
    std::cout << "Welcome! This is a client program.\n";
    std::cout << "Please enter your name: ";
    
    std::string clientName;
    std::getline(std::cin, clientName);

    // system("clear");

    // Concatenate strings using std::string
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


#include <iostream>
#include <string>
#include <cryptopp/osrng.h>
#include <cryptopp/integer.h>
#include <cryptopp/aes.h>
#include <cryptopp/modes.h>
#include <cryptopp/filters.h>
#include <cryptopp/hex.h>
#include <cryptopp/secblock.h>
#include <string>

using namespace CryptoPP;
using namespace std;

SecByteBlock GenerateAESKey(int keySize)
{
    AutoSeededRandomPool prng;
    SecByteBlock key(keySize);
    prng.GenerateBlock(key, keySize);
    return key;
}

string Encrypt(const string& plain, const SecByteBlock& key)
{
    string cipher;

    try
    {
        ECB_Mode<AES>::Encryption e;
        e.SetKey(key, key.size());

        // The StreamTransformationFilter adds padding
        // as required. ECB and CBC Mode must be padded
        // to the block size of the cipher.
        StringSource ss1(plain, true,
            new StreamTransformationFilter(e,
                new StringSink(cipher)
            ) // StreamTransformationFilter      
        ); // StringSource
    }
    catch (CryptoPP::Exception& e)
    {
        cerr << e.what() << endl;
        exit(1);
    }

    return cipher;
}

string Decrypt(const string& cipher, const SecByteBlock& key)
{
    string recovered;

    try
    {
        ECB_Mode<AES>::Decryption d;
        // ECB Mode does not use an IV
        d.SetKey(key, key.size());

        // The StreamTransformationFilter removes
        // padding as required.
        StringSource ss3(cipher, true,
            new StreamTransformationFilter(d,
                new StringSink(recovered)
            ) // StreamTransformationFilter
        ); // StringSource
    }
    catch (CryptoPP::Exception& e)
    {
        cerr << e.what() << endl;
        exit(1);
    }

    return recovered;
}

SecByteBlock StringToSecByteBlock(const string& str)
{
    return SecByteBlock(reinterpret_cast<const CryptoPP::byte*>(str.data()), str.size());
}
string SecByteBlockToString(const SecByteBlock& block)
{
    string str;
    HexEncoder encoder(new StringSink(str));

    encoder.Put(block, block.size());
    encoder.MessageEnd();

    return str;
}

// int main()
// {
//     int keySize = AES::DEFAULT_KEYLENGTH;
//     SecByteBlock key = GenerateAESKey(keySize);
//     cout << "Generated AES Key: " << SecByteBlockToString(key) << endl;

//     string plainText = "Hello, world!";
//     cout << "Plain text: " << plainText << endl;

//     string cipherText = Encrypt(plainText, key);
//     cout << "Cipher text: " << SecByteBlockToString(StringToSecByteBlock(cipherText)) << endl;

//     string decryptedText = Decrypt(cipherText, key);
//     cout << "Decrypted text: " << decryptedText << endl;

//     // Testing conversion from string to SecByteBlock
//     string testString = "This is a test string.";
//     SecByteBlock testBlock = StringToSecByteBlock(testString);
//     cout << "Test string: " << testString << endl;
//     cout << "Test SecByteBlock: " << SecByteBlockToString(testBlock) << endl;

//     return 0;
// }
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "encryption_algorithm.h"

std::vector<std::vector<std::string>> stringToArrayBase(const std::string& str) {
    std::vector<std::vector<std::string>> result;
    std::stringstream ss(str);
    std::string temp;
  
    // Remove square brackets if present
    if (!str.empty() && str.front() == '[' && str.back() == ']') {
        temp = str.substr(1, str.size() - 2); // Remove the first and last characters
    } else {
        temp = str; // No square brackets, use the original string
    }

    std::stringstream ss_temp(temp);
    std::string token;

    // Split the remaining string by commas
    while (std::getline(ss_temp, token, ',')) {
        // Remove leading and trailing whitespace and quotes
        token.erase(0, token.find_first_not_of(" '"));
        token.erase(token.find_last_not_of(" '") + 1);
        result.push_back({token});
    }
    
    return result;
}
std::vector<std::vector<std::string>> stringToArray(const std::string& str) {
    std::vector<std::vector<std::string>> result;
    std::stringstream ss(str);
    std::string substr;

    while (std::getline(ss, substr, '|')) {
        std::vector<std::vector<std::string>> temp = stringToArrayBase(substr);

        for (const auto& element : temp) {
            for (const auto& str : element) {
                std::cout << str << " ";
            }
            // std::cout << std::endl;
        }
        std::cout << std::endl;

        result.insert(result.end(), temp.begin(), temp.end());
    }

    return result;
}
// std::pair<std::vector<std::vector<std::string>>, std::vector<std::vector<std::string>>> splitHeadAndBody(const std::string& data) {
//     std::vector<std::string> sections;
//     std::string delimiter = ":::::::";
    
//     // Make a copy of the data string since we cannot modify the original const string
//     std::string tempData = data;

//     size_t pos = 0;
//     std::string token;
//     while ((pos = tempData.find(delimiter)) != std::string::npos) {
//         token = tempData.substr(0, pos);
//         sections.push_back(token);
//         tempData.erase(0, pos + delimiter.length());
//     }
//     sections.push_back(tempData);  // Add the remaining data after the last delimiter

//     if (sections.size() == 2) {
//         // Extract head and body sections
//         std::vector<std::vector<std::string>> head = stringToArray(sections[0]);
//         std::vector<std::vector<std::string>> body = stringToArray(sections[1]);
        
//         return {head, body};
//     } else {
//         // If there are not exactly two sections, return empty vectors
//         return {{}, {}};
//     }
// }


// Function to split the concatenated string into head and body sections
std::pair<std::vector<std::vector<std::string>>,std::vector<std::vector<std::string>>> splitHeadAndBody(const std::string& data) {
    std::vector<std::string> sections;
    std::string delimiter = ":::::::";
    
    // Make a copy of the data string since we cannot modify the original const string
    std::string tempData = data;

    size_t pos = 0;
    std::string token;
    while ((pos = tempData.find(delimiter)) != std::string::npos) {
        token = tempData.substr(0, pos);
        sections.push_back(token);
        tempData.erase(0, pos + delimiter.length());
    }
    sections.push_back(tempData);  // Add the remaining data after the last delimiter

    if (sections.size() == 2) {
        // Extract head and body sections
        std::vector<std::vector<std::string>>head = stringToArray(sections[0]);
        std::vector<std::vector<std::string>> body = stringToArray(sections[1]);
        
        return {head, body};
    } else {
        // If there are not exactly two sections, return empty vectors
        return {{}, {}};
    }
}

// Function to convert an array of elements to a string representation
template<typename T>
std::string arrayToStringBase(const std::vector<T>& arr) {
    std::stringstream ss;
    ss << "[";
    for (size_t i = 0; i < arr.size(); ++i) {
        // ss << "\\\"" << arr[i] << "\\\"";
        ss << "'" << arr[i] << "'";
        if (i != arr.size() - 1) {
            ss << ",";
        }
    }
    ss << "]";
    return ss.str();
}

// Function to convert a vector of vectors to a string representation, each inner vector represents an array 
template<typename T>
std::string arrayToString(const std::vector<std::vector<T>>& arr) {
    std::stringstream ss;
    for (size_t i = 0; i < arr.size(); ++i) {
        ss << arrayToStringBase(arr[i]);
        if (i != arr.size() - 1) {
            ss << "|";
        }
    }
    return ss.str();
}

// Function to concatenate head and body arrays with delimiter
template<typename T>
std::string concatenateHeadAndBody(const std::vector<T>& head, const std::vector<std::vector<T>>& body) {
    // Convert head to string representation
    std::string headString = arrayToStringBase(head);

    // Convert body to string representation
    std::string bodyString = arrayToString(body);

    // Concatenate head and body with delimiter
    return headString + ":::::::" + bodyString;
}
std::pair<std::string, std::vector<std::string>> recv_0(const std::vector<std::vector<std::string>>& head, const std::vector<std::vector<std::string>>& body) {
    std::pair<std::string, std::vector<std::string>> result;
    std::cout << "Received message with code 0:" << std::endl;
    
    // Processing Header
    std::vector<std::string> headerData;
    for (const auto& vec : head) {
        for (const auto& str : vec) {
            headerData.push_back(str);
        }
    }
    result.first = "Header";
    result.second = headerData;

    // Decrypting and Processing Body if it's not empty
    // if (!body.empty()) {
    //     std::vector<std::string> decryptedBody;
    //     for (const auto& vec : body) {
    //         for (const auto& str : vec) {
    //             // Decrypt the string (assuming Decrypt function exists)
    //             std::string decryptedStr = Decrypt(str, "TestKey");
    //             // Process the decrypted string
    //             // For example, add it to the decrypted body vector
    //             decryptedBody.push_back(decryptedStr);
    //         }
    //     }

    //     // Printing Decrypted Body
    //     std::cout << "Decrypted Body: [";
    //     for (const auto& str : decryptedBody) {
    //         std::cout << str << ", ";
    //     }
    //     std::cout << "]" << std::endl;

    //     // Assigning the decrypted body to the result
    //     result.second = decryptedBody;
    // } else {
    //     std::cout << "Empty body received." << std::endl;
    // }

    return result;
}


std::pair<std::string, std::vector<std::string>> recv_100(const std::vector<std::vector<std::string>>& head, const std::vector<std::vector<std::string>>& body) {
    std::pair<std::string, std::vector<std::string>> result;
    std::cout << "Received message with code 100:" << std::endl;
    
    // Processing Header
    std::vector<std::string> headerData;
    for (const auto& vec : head) {
        for (const auto& str : vec) {
            headerData.push_back(str);
        }
    }
    result.first = "Header";
    result.second = headerData;

    // Printing Body
    std::cout << "Body: [";
    for (const auto& vec : body) {
        std::cout << "[";
        for (const auto& str : vec) {
            std::cout << str << ", ";
        }
        std::cout << "], ";
    }
    std::cout << "]" << std::endl;

    return result;
}

std::pair<std::string, std::vector<std::string>> send_100(const std::vector<std::vector<std::string>>& head, const std::vector<std::vector<std::string>>& body) {
    std::pair<std::string, std::vector<std::string>> result;
    std::cout << "Received message with code 100:" << std::endl;
    
    // Processing Header
    std::vector<std::string> headerData;
    for (const auto& vec : head) {
        for (const auto& str : vec) {
            headerData.push_back(str);
        }
    }
    result.first = "Header";
    result.second = headerData;

    // Printing Body
    std::cout << "Body: [";
    for (const auto& vec : body) {
        std::cout << "[";
        for (const auto& str : vec) {
            std::cout << str << ", ";
        }
        std::cout << "], ";
    }
    std::cout << "]" << std::endl;

    return result;
}

// // 200 received
// std::string recv_200(const std::vector<std::vector<std::string>>& head, const std::vector<std::vector<std::string>>& body) {
//     std::string result;
//     std::cout << "Received message with code 200:" << std::endl;

//     return result;
// }


// // 300 received
// std::string recv_300(const std::vector<std::vector<std::string>>& head, const std::vector<std::vector<std::string>>& body) {
//     std::string result;
//     std::cout << "Received message with code 300:" << std::endl;
//     // Handle code 300
//     return result;
// }

// // 100 Send
// std::string send_100(const std::vector<std::vector<std::string>>& head, const std::vector<std::vector<std::string>>& body) {
//     std::string result;
//     std::cout << "Received message with code 300:" << std::endl;
//     // Hand le code 300
//     return result;
// }

// // 200 Send
// std::string send_200(const std::vector<std::vector<std::string>>& head, const std::vector<std::vector<std::string>>& body) {
//     std::string result;
//     std::cout << "Received message with code 300:" << std::endl;
//     // Handle code 300
//     return result;
// }

// // 300 Send
// std::string send_300(const std::vector<std::vector<std::string>>& head, const std::vector<std::vector<std::string>>& body) {
//     std::string result;
//     std::cout << "Received message with code 300:" << std::endl;
//     // Handle code 300
//     return result;
// }



// std::string send_600(const std::vector<std::vector<std::string>>& head, const std::vector<std::vector<std::string>>& body) {
//     std::string result;
//     std::cout << "Received message with code 300:" << std::endl;
//     // Handle code 300
//     return result;
// }

// std::string processMessage(const std::string& message) {
//     std::string result;

//     // Split the message into header and body
//     auto splitResult = splitHeadAndBody(message);
//     std::vector<std::vector<std::string>>& header = splitResult.first;
//     std::vector<std::vector<std::string>>& body = splitResult.second;

//     // Check if the header has at least 5 elements
//     if (!header.empty() && header.size() >= 5) {
//         std::string messageCode;
//         if (header[4].size() >= 1) {  // Check if the 5th element has at least 1 string
//             messageCode = header[4][0]; // Accessing the first string of the 5th element as message code
//             std::cout << "Received message with code: " << messageCode << std::endl;

//             // Call appropriate function based on message code
//             if (messageCode == "100") {
//                 result = recv_100(header, body);
//             } else if (messageCode == "200") {
//                 result = recv_200(header, body);
//             } else if (messageCode == "300") {
//                 result = recv_300(header, body);
//             } else {
//                 std::cerr << "Unknown message code: " << messageCode << std::endl;
//             }
//         } else {
//             std::cerr << "Invalid message code in header" << std::endl;
//         }
//     } else {
//         std::cerr << "Invalid header format" << std::endl;
//     }

//     return result;
// }

std::pair<std::vector<std::string>, std::string> processMessage(const std::string& message) {
    std::string result;
    std::vector<std::string> additionalData; // Vector to hold additional data

    // Split the message into header and body
    auto splitResult = splitHeadAndBody(message);
    std::vector<std::vector<std::string>>& header = splitResult.first;
    std::vector<std::vector<std::string>>& body = splitResult.second;

    // Check if the header has at least 5 elements
    if (!header.empty() && header.size() >= 5) {
        std::string messageCode;
        if (header[4].size() >= 1) {  // Check if the 5th element has at least 1 string
            messageCode = header[4][0]; // Accessing the first string of the 5th element as message code
            std::cout << "Received message with code: " << messageCode << std::endl;

            // Call appropriate function based on message code
            if (messageCode == "100") {
                std::pair<std::string, std::vector<std::string>> recvResult = recv_100(header, body);
                result = recvResult.first;
                additionalData = recvResult.second;
            }
            // else  if (messageCode == "0") {
            //     std::pair<std::string, std::vector<std::string>> recvResult = recv_0(header, body);
            //     result = recvResult.first;
            //     additionalData = recvResult.second;
            // }
            //  else if (messageCode == "200") {
            //     std::pair<std::string, std::vector<std::string>> recvResult = recv_200(header, body);
            //     result = recvResult.first;
            //     additionalData = recvResult.second;
            // } else if (messageCode == "300") {
            //     std::pair<std::string, std::vector<std::string>> recvResult = recv_300(header, body);
            //     result = recvResult.first;
            //     additionalData = recvResult.second;
            // } else {
            //     std::cerr << "Unknown message code: " << messageCode << std::endl;
            // }
        } else {
            std::cerr << "Invalid message code in header" << std::endl;
        }
    } else {
        std::cerr << "Invalid header format" << std::endl;
    }

    return std::make_pair(additionalData, result);
}






// int main() {

//     std::vector<std::string> head = {"192.168.0.0", "8080", "192.168.0.1", "9090","100"};
//     std::vector<std::vector<std::string>> body = {{"dnjsadbajsd", "o", "d", "y"}, {"B", "o", "d", "y"}};
//     std::string concatenatedString = concatenateHeadAndBody(head, body);
//     std::cout << "Concatenated string: " << concatenatedString << std::endl;



//     // Test message
//     // std::string message = "[ \"H\",\"e\",\"a\",\"d\" ]:::::::[ \"dnjsadbajsd\",\"o\",\"d\",\"y\" ]|[ \"B\",\"o\",\"d\",\"y\" ]";
    
//     // Process the message
//     processMessage(concatenatedString);
    
//     return 0;
// }
