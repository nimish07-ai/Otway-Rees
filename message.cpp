#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "encryption_algorithm.h"
#include "diffiehelman.h"

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

// Function to generate the header based on sender and receiver information
std::vector<std::string> generate_head(const std::string& senderip, const std::string& senderport, const std::string& receiverip, const std::string& receiverport, const std::string& code) {
    std::vector<std::string> head;
    head.push_back(receiverip);
    head.push_back(receiverport);
    head.push_back(senderip);
    head.push_back(senderport);
    head.push_back(code);
    return head;
}

// Function to send a message with code 100
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

std::pair<std::vector<std::string>,std::string> recv_100(const std::vector<std::string>& head, const std::vector<std::vector<std::string>>& body, const std::string& clientName, int keysize) {
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

std::pair<std::vector<std::string>,std::string>  recv_101(const std::vector<std::string>& head, const std::vector<std::vector<std::string>>& body) {
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
std::pair<std::vector<std::string>,std::string>  recv_102(const std::vector<std::string>& head, const std::vector<std::vector<std::string>>& body) {
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






//100 series - diffe helman establishment
//100 sent by client to kdc req to send half key and global parameter
//101 sent by kdc it contains half key and global parameter
//102 sent by client containes his part of half key
//103 sent by kdc nonce encrypted usning ssk established
//104 sent by client nonce decdresd by 1 encypted by ssk














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












// int main() {
                // // receiverip,reciverport,senderip,senderport ,code
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





