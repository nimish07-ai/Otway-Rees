#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <algorithm> // for std::replace
#include "../../global/global.h"

std::vector<std::vector<std::string>> stringToArrayBase(const std::string& str) {
    std::vector<std::vector<std::string>> result;
    std::stringstream ss(str);
    std::string token;

    // Replace '[' and ']' with space
    std::string cleanedStr = str;
    std::replace(cleanedStr.begin(), cleanedStr.end(), '[', ' ');
    std::replace(cleanedStr.begin(), cleanedStr.end(), ']', ' ');

    // Tokenize the cleaned string
    std::stringstream cleanedSS(cleanedStr);

    while (std::getline(cleanedSS, token, ',')) {
        // Remove leading and trailing whitespace and quotes
        token.erase(0, token.find_first_not_of(" '"));
        token.erase(token.find_last_not_of(" '") + 1);
        result.push_back({token});
    }
    
    return result;
}


// Function to split a string into a vector of vectors of strings based on delimiter '|'
std::vector<std::vector<std::string>> stringToArray(const std::string& str) {
    std::vector<std::vector<std::string>> result;
    std::stringstream ss(str);
    std::string substr;

    while (std::getline(ss, substr, '|')) {
        std::vector<std::vector<std::string>> temp = stringToArrayBase(substr);
        result.insert(result.end(), temp.begin(), temp.end());
    }

    return result;
}

// Function to split the concatenated string into head and body sections
std::pair<std::vector<std::vector<std::string>>, std::vector<std::vector<std::string>>> splitHeadAndBody(const std::string& data) {
    std::pair<std::vector<std::vector<std::string>>, std::vector<std::vector<std::string>>> result;
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
        result.first = stringToArray(sections[0]);
        result.second = stringToArray(sections[1]);
    }

    return result;
}

// Function to convert a vector of elements to a string representation
template<typename T>
std::string arrayToStringBase(const std::vector<T>& arr) {
    std::stringstream ss;
    ss << "[";
    for (size_t i = 0; i < arr.size(); ++i) {
        ss << "'" << arr[i] << "'";
        if (i != arr.size() - 1) {
            ss << ",";
        }
    }
    ss << "]";
    return ss.str();
}

// Function to convert a vector of vectors to a string representation
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
// template<typename T>
std::string concatenateHeadAndBody(const std::vector<std::string>& head, const std::vector<std::vector<std::string>>& body) {
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

void sendMessage_preprocessor(const std::pair<std::vector<std::string>, std::string>& processedMessage,int sock)
{
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



// int main() {
//     // Example usage:
//     std::vector<std::string> head = generate_head("192.168.1.2", "8080", "192.168.1.3", "9090", "100");
//     std::vector<std::vector<std::string>> body = {{"data1", "data2"}, {"data3", "data4"}};
//     std::string concatenatedString = concatenateHeadAndBody(head, body);
//     std::cout << "Concatenated String: " << concatenatedString << std::endl;

//     // Splitting concatenated string into head and body
//     auto splitResult = splitHeadAndBody(concatenatedString);
//     std::cout << "Head:" << std::endl;
//     for (const auto& vec : splitResult.first) {
//         for (const auto& str : vec) {
//             std::cout << str << " ";
//         }
//         std::cout << std::endl;
//     }
//     std::cout << "Body:" << std::endl;
//     for (const auto& vec : splitResult.second) {
//         for (const auto& str : vec) {
//             std::cout << str << " ";
//         }
//         std::cout << std::endl;
//     }

//     return 0;
// }





// #include <iostream>
// #include <sstream>
// #include <string>
// #include <vector>


// std::vector<std::vector<std::string>> stringToArrayBase(const std::string& str) {
//     std::vector<std::vector<std::string>> result;
//     std::stringstream ss(str);
//     std::string temp;
  
//     // Remove square brackets if present
//     if (!str.empty() && str.front() == '[' && str.back() == ']') {
//         temp = str.substr(1, str.size() - 2); // Remove the first and last characters
//     } else {
//         temp = str; // No square brackets, use the original string
//     }

//     std::stringstream ss_temp(temp);
//     std::string token;

//     // Split the remaining string by commas
//     while (std::getline(ss_temp, token, ',')) {
//         // Remove leading and trailing whitespace and quotes
//         token.erase(0, token.find_first_not_of(" '"));
//         token.erase(token.find_last_not_of(" '") + 1);
//         result.push_back({token});
//     }
    
//     return result;
// }
// std::vector<std::vector<std::string>> stringToArray(const std::string& str) {
//     std::vector<std::vector<std::string>> result;
//     std::stringstream ss(str);
//     std::string substr;

//     while (std::getline(ss, substr, '|')) {
//         std::vector<std::vector<std::string>> temp = stringToArrayBase(substr);

//         for (const auto& element : temp) {
//             for (const auto& str : element) {
//                 std::cout << str << " ";
//             }
//             // std::cout << std::endl;
//         }
//         std::cout << std::endl;

//         result.insert(result.end(), temp.begin(), temp.end());
//     }

//     return result;
// }


// // Function to split the concatenated string into head and body sections
// std::pair<std::vector<std::vector<std::string>>,std::vector<std::vector<std::string>>> splitHeadAndBody(const std::string& data) {
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
//         std::vector<std::vector<std::string>>head = stringToArray(sections[0]);
//         std::vector<std::vector<std::string>> body = stringToArray(sections[1]);
        
//         return {head, body};
//     } else {
//         // If there are not exactly two sections, return empty vectors
//         return {{}, {}};
//     }
// }

// // Function to convert an array of elements to a string representation
// template<typename T>
// std::string arrayToStringBase(const std::vector<T>& arr) {
//     std::stringstream ss;
//     ss << "[";
//     for (size_t i = 0; i < arr.size(); ++i) {
//         // ss << "\\\"" << arr[i] << "\\\"";
//         ss << "'" << arr[i] << "'";
//         if (i != arr.size() - 1) {
//             ss << ",";
//         }
//     }
//     ss << "]";
//     return ss.str();
// }

// // Function to convert a vector of vectors to a string representation, each inner vector represents an array 
// template<typename T>
// std::string arrayToString(const std::vector<std::vector<T>>& arr) {
//     std::stringstream ss;
//     for (size_t i = 0; i < arr.size(); ++i) {
//         ss << arrayToStringBase(arr[i]);
//         if (i != arr.size() - 1) {
//             ss << "|";
//         }
//     }
//     return ss.str();
// }

// // Function to concatenate head and body arrays with delimiter
// template<typename T>
// std::string concatenateHeadAndBody(const std::vector<T>& head, const std::vector<std::vector<T>>& body) {
//     // Convert head to string representation
//     std::string headString = arrayToStringBase(head);

//     // Convert body to string representation
//     std::string bodyString = arrayToString(body);

//     // Concatenate head and body with delimiter
//     return headString + ":::::::" + bodyString;
// }

// // Function to generate the header based on sender and receiver information
// std::vector<std::string> generate_head(const std::string& senderip, const std::string& senderport, const std::string& receiverip, const std::string& receiverport, const std::string& code) {
//     std::vector<std::string> head;
//     head.push_back(receiverip);
//     head.push_back(receiverport);
//     head.push_back(senderip);
//     head.push_back(senderport);
//     head.push_back(code);
//     return head;
// }