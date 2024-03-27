#include <iostream>
#include <vector>
#include <string>
#include "../message_helper/message_helper.h"
#include "../encryption_algorithm/encryption_algorithm.h"
#include "../diffiehelman/diffiehelman.h"
#include "message_300.h"

std::pair<std::vector<std::string>, std::string> send_300(const std::string& senderip, const std::string& senderport, const std::string& receiverip, const std::string& receiverport) {
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
std::pair<std::vector<std::string>,std::string> recv_300(const std::vector<std::string>& head, const std::vector<std::vector<std::string>>& body) {
    std::pair<std::vector<std::string>,std::string > result;

    cout<<"recieived 100";
    // result=send_101(head[2], head[3], head[0], head[1], clientName , keysize);

    return result;
}

std::pair<std::vector<std::string>, std::string> send_301(const std::string& senderip, const std::string& senderport, const std::string& receiverip, const std::string& receiverport) {
    std::pair<std::vector<std::string>, std::string> result;

    return result;
}

std::pair<std::vector<std::string>,std::string> recv_301(const std::vector<std::string>& head, const std::vector<std::vector<std::string>>& body) {
    std::pair<std::vector<std::string>,std::string > result;

    cout<<"recieived 100";
    // result=send_101(head[2], head[3], head[0], head[1], clientName , keysize);

    return result;
}

std::pair<std::vector<std::string>, std::string> send_302(const std::string& senderip, const std::string& senderport, const std::string& receiverip, const std::string& receiverport) {
    std::pair<std::vector<std::string>, std::string> result;

    return result;
}
std::pair<std::vector<std::string>,std::string> recv_302(const std::vector<std::string>& head, const std::vector<std::vector<std::string>>& body) {
    std::pair<std::vector<std::string>,std::string > result;

    cout<<"recieived 100";
    // result=send_101(head[2], head[3], head[0], head[1], clientName , keysize);

    return result;
}

std::pair<std::vector<std::string>, std::string> send_303(const std::string& senderip, const std::string& senderport, const std::string& receiverip, const std::string& receiverport) {
    std::pair<std::vector<std::string>, std::string> result;

    return result;
}
std::pair<std::vector<std::string>,std::string> recv_303(const std::vector<std::string>& head, const std::vector<std::vector<std::string>>& body) {
    std::pair<std::vector<std::string>,std::string > result;

    cout<<"recieived 100";
    // result=send_101(head[2], head[3], head[0], head[1], clientName , keysize);

    return result;
}

std::pair<std::vector<std::string>, std::string> send_304(const std::string& senderip, const std::string& senderport, const std::string& receiverip, const std::string& receiverport) {
    std::pair<std::vector<std::string>, std::string> result;

    return result;
}
std::pair<std::vector<std::string>,std::string> recv_304(const std::vector<std::string>& head, const std::vector<std::vector<std::string>>& body) {
    std::pair<std::vector<std::string>,std::string > result;

    cout<<"recieived 100";
    // result=send_101(head[2], head[3], head[0], head[1], clientName , keysize);

    return result;
}




std::pair<std::vector<std::string>,std::string> processMessage_300s(std::vector<std::string>& header, std::vector<std::vector<std::string>>& body, int messageCode) {
    // std::string senderip = header[0];
    // std::string senderport = header[1];
    // std::string receiverip = header[2];
    // std::string receiverport = header[3];

    // Call the appropriate send function based on the message code
    if (messageCode == 300) {
        // Call send_100
        return recv_300(header,body);
    } else if (messageCode == 301) {
        // Call send_100
        return recv_301(header,body);
    } else if (messageCode == 302) {
        // Call send_100
        return recv_302(header,body);
    } else if (messageCode == 303) {
        // Call send_100
        return recv_303(header,body);
    } else if (messageCode == 304) {
        // Call send_100
        return recv_304(header,body);
    }
    else {
        // Handle unknown message code
        std::cerr << "Unknown message code: " << messageCode << std::endl;
        return std::make_pair(std::vector<std::string>(), "");
    }
}