#include <iostream>
#include <vector>
#include <string>
#include "../message_helper/message_helper.h"
#include "../encryption_algorithm/encryption_algorithm.h"
#include "../diffiehelman/diffiehelman.h"
#include "message_200.h"

std::pair<std::vector<std::string>, std::string> send_200(const std::string& senderip, const std::string& senderport, const std::string& receiverip, const std::string& receiverport) {
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

std::pair<std::vector<std::string>,std::string> recv_200(const std::vector<std::vector<std::string>>& head, const std::vector<std::vector<std::string>>& body) {
    std::pair<std::vector<std::string>,std::string > result;

    cout<<"recieived 100";
    // result=send_101(head[2], head[3], head[0], head[1], clientName , keysize);

    return result;
}


std::pair<std::vector<std::string>, std::string> send_201(const std::string& senderip, const std::string& senderport, const std::string& receiverip, const std::string& receiverport) {
    std::pair<std::vector<std::string>, std::string> result;

    return result;
}
std::pair<std::vector<std::string>,std::string> recv_201(const std::vector<std::vector<std::string>>& head, const std::vector<std::vector<std::string>>& body) {
    std::pair<std::vector<std::string>,std::string > result;

    cout<<"recieived 100";
    // result=send_101(head[2], head[3], head[0], head[1], clientName , keysize);

    return result;
}

std::pair<std::vector<std::string>, std::string> send_202(const std::string& senderip, const std::string& senderport, const std::string& receiverip, const std::string& receiverport) {
    std::pair<std::vector<std::string>, std::string> result;

    return result;
}

std::pair<std::vector<std::string>,std::string> recv_202(const std::vector<std::vector<std::string>>& head, const std::vector<std::vector<std::string>>& body) {
    std::pair<std::vector<std::string>,std::string > result;

    cout<<"recieived 100";
    // result=send_101(head[2], head[3], head[0], head[1], clientName , keysize);

    return result;
}

std::pair<std::vector<std::string>, std::string> send_203(const std::string& senderip, const std::string& senderport, const std::string& receiverip, const std::string& receiverport) {
    std::pair<std::vector<std::string>, std::string> result;

    return result;
}
std::pair<std::vector<std::string>,std::string> recv_203(const std::vector<std::vector<std::string>>& head, const std::vector<std::vector<std::string>>& body) {
    std::pair<std::vector<std::string>,std::string > result;

    cout<<"recieived 100";
    // result=send_101(head[2], head[3], head[0], head[1], clientName , keysize);

    return result;
}

std::pair<std::vector<std::string>, std::string> send_204(const std::string& senderip, const std::string& senderport, const std::string& receiverip, const std::string& receiverport) {
    std::pair<std::vector<std::string>, std::string> result;

    return result;
}
std::pair<std::vector<std::string>,std::string> recv_204(const std::vector<std::vector<std::string>>& head, const std::vector<std::vector<std::string>>& body) {
    std::pair<std::vector<std::string>,std::string > result;

    cout<<"recieived 100";
    // result=send_101(head[2], head[3], head[0], head[1], clientName , keysize);

    return result;
}

std::pair<std::vector<std::string>, std::string> send_205(const std::string& senderip, const std::string& senderport, const std::string& receiverip, const std::string& receiverport) {
    std::pair<std::vector<std::string>, std::string> result;

    return result;
}
std::pair<std::vector<std::string>,std::string> recv_205(const std::vector<std::vector<std::string>>& head, const std::vector<std::vector<std::string>>& body) {
    std::pair<std::vector<std::string>,std::string > result;

    cout<<"recieived 100";
    // result=send_101(head[2], head[3], head[0], head[1], clientName , keysize);

    return result;
}



std::pair<std::vector<std::string>,std::string> processMessage_200s(std::vector<std::vector<std::string>>& header, std::vector<std::vector<std::string>>& body, int messageCode) {
  
    // Call the appropriate send function based on the message code
     if (messageCode == 200) {
        // Call send_100
        return recv_200(header, body);
    } else
     if (messageCode == 201) {
        // Call send_100
        return recv_201(header, body);
    } else
     if (messageCode == 202) {
        // Call send_100
        return recv_202(header, body);
    } else
     if (messageCode == 203) {
        // Call send_100
        return recv_203(header, body);
    } else
         if (messageCode == 204) {
        // Call send_100
        return recv_204(header, body);
    } else
    if (messageCode == 205) {
        // Call send_100
        return recv_205(header, body);
    } else {
        // Handle unknown message code
        std::cerr << "Unknown message code: " << messageCode << std::endl;
        return std::make_pair(std::vector<std::string>(), "");
    }
}