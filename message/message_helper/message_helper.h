#ifndef MESSAGE_HELPER_H
#define MESSAGE_HELPER_H

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// Function declarations
std::vector<std::vector<std::string>> stringToArrayBase(const std::string& str);
std::vector<std::vector<std::string>> stringToArray(const std::string& str);
std::pair<std::vector<std::vector<std::string>>,std::vector<std::vector<std::string>>> splitHeadAndBody(const std::string& data);
template<typename T>
std::string arrayToStringBase(const std::vector<T>& arr);
template<typename T>
std::string arrayToString(const std::vector<std::vector<T>>& arr);

std::string concatenateHeadAndBody(const std::vector<std::string>& head, const std::vector<std::vector<std::string>>& body);
std::vector<std::string> generate_head(const std::string& senderip, const std::string& senderport, const std::string& receiverip, const std::string& receiverport, const std::string& code);

// Function declaration for sendMessage_preprocessor
void sendMessage_preprocessor(const std::pair<std::vector<std::string>, std::string>& processedMessage, int sock);

#endif // MESSAGE_HELPER_H
