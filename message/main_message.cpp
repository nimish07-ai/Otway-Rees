#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "encryption_algorithm.h"
#include "diffiehelman.h"


std::pair<std::vector<std::string>, std::string> processMessage(const std::string& message,const std::string& clientName,const int keysize) {
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

            // Call appropriate function based on message code
            if (messageCode == "100") {
                std::pair<std::vector<std::string>,std::string> recvResult = recv_100(header[0], body,clientName,keysize);
                result = recvResult.second;
                additionalData = recvResult.first;
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
