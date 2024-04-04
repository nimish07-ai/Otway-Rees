#include <iostream>
#include <vector>
#include <string>
#include "../message_helper/message_helper.h"
#include "../encryption_algorithm/encryption_algorithm.h"
#include "../diffiehelman/diffiehelman.h"
#include "message_100.h"

std::pair<std::vector<std::string>, std::string> send_100(const std::string& senderip, const std::string& senderport, const std::string& receiverip, const std::string& receiverport) {
    std::pair<std::vector<std::string>, std::string> result;
    std::cout << "Received message with code 100:" << std::endl;

    // Generate the header
    std::vector<std::string> head = generate_head(senderip, senderport, receiverip, receiverport, "100");

    // Generate the body
    std::vector<std::vector<std::string>> body = {{"nimish"}};

    // Concatenate head and body
    std::string concatenatedString = concatenateHeadAndBody(head, body);

    // Set the result string
    result.second = concatenatedString;

    // Set the result vector
    std::vector<std::string> entry;
    entry.push_back(receiverip);
    entry.push_back(receiverport);
    result.first = entry;

    return result;
}

std::pair<std::vector<std::string>,std::string> recv_100(const std::vector<std::vector<std::string>>& head, const std::vector<std::vector<std::string>>& body) {
    std::pair<std::vector<std::string>,std::string > result;

    cout<<"recieived 100";
    // result=send_101(head[2], head[3], head[0], head[1], clientName , keysize);

    return result;
}



// Function to send a message with code 101
std::pair<std::vector<std::string>, std::string> send_101(const std::string& senderip, const std::string& senderport, const std::string& receiverip, const std::string& receiverport, const std::string& name, int keysize) {
    std::pair<std::vector<std::string>, std::string> result;

    // Generate the header
    std::vector<std::string> head = generate_head(senderip, senderport, receiverip, receiverport, "101");

    // Generate global parameters and half key
    std::vector<std::string> proxy = GenerateGlobalParametersAndHalfKey(keysize); // Assuming this function returns a vector of strings

    // Generate the body
    std::vector<std::vector<std::string>> body = {{name}, proxy};

    // Concatenate head and body
    std::string concatenatedString = concatenateHeadAndBody(head, body);

    // Set the result string
    result.second = concatenatedString;

    // Set the result vector (sender IP and port)
    result.first = {senderip, senderport};

    return result;
}

std::pair<std::vector<std::string>,std::string>  recv_101(const std::vector<std::vector<std::string>>& head, const std::vector<std::vector<std::string>>& body) {
    std::pair<std::vector<std::string>,std::string > result;
    
    // result=send_102(head[2], head[3], head[0], head[1], clientName , body[1]);

    return result;
}

std::pair<std::vector<std::string>, std::string> send_102(const std::string& senderip, const std::string& senderport, const std::string& receiverip, const std::string& receiverport,const std::string& name,const std::vector<std::string>& paramsAndHalfKey) {
     std::pair<std::vector<std::string>, std::string> result;

    // Generate the header
    std::vector<std::string> head = generate_head(senderip, senderport, receiverip, receiverport, "101");

    // Generate global parameters and half key
    std::vector<std::string> proxy = GenerateFullKeyFromParameters(paramsAndHalfKey); // Assuming this function returns a vector of strings

    // Generate the body
    std::vector<std::vector<std::string>> body = {{name}, proxy};

    // Concatenate head and body
    std::string concatenatedString = concatenateHeadAndBody(head, body);

    // Set the result string
    result.second = concatenatedString;

    // Set the result vector (sender IP and port)
    result.first = {senderip, senderport};

    return result;
}
std::pair<std::vector<std::string>,std::string>  recv_102(const std::vector<std::vector<std::string>>& head, const std::vector<std::vector<std::string>>& body) {
    std::pair< std::vector<std::string>,std::string> result;
    
    // result=send_103(head[2], head[3], head[0], head[1], clientName , body[1]);

    return result;
}

std::pair<std::vector<std::string>, std::string> send_103(const std::string& senderip, const std::string& senderport, const std::string& receiverip, const std::string& receiverport,const std::string& name,const std::string& key,const std::string& num) {
     std::pair<std::vector<std::string>, std::string> result;

    // Generate the header
    std::vector<std::string> head = generate_head(senderip, senderport, receiverip, receiverport, "101");

    // Generate global parameters and half key
    // std::vector<std::string> proxy = ; // Assuming this function returns a vector of strings

    // Generate the body
    std::vector<std::vector<std::string>> body = {{name},  {Encrypt(num,StringToSecByteBlock(key))}};

    // Concatenate head and body
    std::string concatenatedString = concatenateHeadAndBody(head, body);

    // Set the result string
    result.second = concatenatedString;

    // Set the result vector (sender IP and port)
    result.first = {senderip, senderport};

    return result;
}
std::pair<std::vector<std::string>,std::string>  recv_103(const std::vector<std::vector<std::string>>& head, const std::vector<std::vector<std::string>>& body) {
    std::pair< std::vector<std::string>,std::string> result;
    
    // result=send_103(head[2], head[3], head[0], head[1], clientName , body[1]);

    return result;
}

std::pair<std::vector<std::string>, std::string> send_104(const std::string& senderip, const std::string& senderport, const std::string& receiverip, const std::string& receiverport,const std::string& name,const std::string& key,const std::string& num) {
     std::pair<std::vector<std::string>, std::string> result;

    // Generate the header
    std::vector<std::string> head = generate_head(senderip, senderport, receiverip, receiverport, "101");

    // Generate global parameters and half key
    // std::vector<std::string> proxy = ; // Assuming this function returns a vector of strings

    // Generate the body
    std::vector<std::vector<std::string>> body = {{name},  {Encrypt(num,StringToSecByteBlock(key))}};

    // Concatenate head and body
    std::string concatenatedString = concatenateHeadAndBody(head, body);

    // Set the result string
    result.second = concatenatedString;

    // Set the result vector (sender IP and port)
    result.first = {senderip, senderport};

    return result;
}
std::pair<std::vector<std::string>,std::string>  recv_104(const std::vector<std::vector<std::string>>& head, const std::vector<std::vector<std::string>>& body) {
    std::pair< std::vector<std::string>,std::string> result;
    
    // result=send_103(head[2], head[3], head[0], head[1], clientName , body[1]);

    return result;
}

std::pair<std::vector<std::string>, std::string> processMessage_100s(std::vector<std::vector<std::string>>& header, std::vector<std::vector<std::string>>& body, int messageCode) {

    // Call the appropriate send function based on the message code
    if (messageCode == 100) {
        // Call send_100
        return recv_100(header, body);
    } else if (messageCode == 101) {
        // Call send_100
        return recv_101(header, body);
    }
    
     else if (messageCode == 102) {
        // Call send_100
        return recv_102(header, body);
    } else if (messageCode == 103) {
        // Call send_100
        return recv_103(header, body);
    } else if (messageCode == 104) {
        // Call send_100
        return recv_104(header, body);
    } 
    else {
        // retuen 301
        // Handle unknown message code
        std::cerr << "Unknown message code: " << messageCode << std::endl;
        // Return an empty pair
        return std::make_pair(std::vector<std::string>(), "");
    }
}
