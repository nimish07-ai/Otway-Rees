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