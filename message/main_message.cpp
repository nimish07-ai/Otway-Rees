#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "encryption_algorithm/encryption_algorithm.h"
#include "diffiehelman/diffiehelman.h"
#include "mes_100/message_100.h"
#include "mes_200/message_200.h"
#include "mes_300/message_300.h"
#include "mes_400/message_400.h"
#include "message_helper/message_helper.h"

std::pair<std::vector<std::string>, std::string> processMessage(const std::string& message) {
    std::string result;
    std::vector<std::string> additionalData; // Vector to hold additional data

    // Split the message into header and body
    std::pair<std::vector<std::vector<std::string>>,std::vector<std::vector<std::string>>> splitResult = splitHeadAndBody(message);
    std::vector<std::vector<std::string>>& header = splitResult.first;
    std::vector<std::vector<std::string>>& body = splitResult.second;

    // Check if the header has at least 5 elements
    if (!header.empty() && header.size() >= 5) {
        std::string messageCode;
        if (header[4].size() >= 1) {  // Check if the 5th element has at least 1 string
            messageCode = header[4][0]; // Accessing the first string of the 5th element as message code
            std::cout << "Received message with code: " << messageCode << std::endl;
            std::string messageCodeString = messageCode;
            // int messageCode = std::stoi(messageCode)    ; // Convert message code to integer

            std::cout << "Received message with code: " << messageCode << std::endl;
            int messageCodeInt = std::stoi(messageCode);

            // Call appropriate function based on message code range
            if (messageCodeInt >= 100 && messageCodeInt < 200) {
                return processMessage_100s(header[0], body,messageCodeInt);
            } 
            else if (messageCodeInt >= 200 && messageCodeInt < 300) {
                return processMessage_200s(header[0], body,messageCodeInt);
            } else if (messageCodeInt >= 300 && messageCodeInt < 400) {
                return processMessage_300s(header[0], body,messageCodeInt);
            } else if (messageCodeInt >= 400 && messageCodeInt < 500) {
                return processMessage_400s( header[0], body, messageCodeInt );
            } 
            else {
                // return 300
                std::cerr << "Unknown message code: " << messageCodeInt << std::endl;
            }
            } else {
                // return 301
                std::cerr << "Invalid message code in header" << std::endl;
            }
    } else {
        // return 301
        std::cerr << "Invalid header format" << std::endl;
    }

    return std::make_pair(additionalData, result);
}
