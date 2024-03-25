#ifndef MESSAGE_PROCESSOR_H
#define MESSAGE_PROCESSOR_H

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

std::vector<std::vector<std::string>> stringToArrayBase(const std::string& str);
std::vector<std::vector<std::string>> stringToArray(const std::string& str);
std::pair<std::vector<std::vector<std::string>>, std::vector<std::vector<std::string>>> splitHeadAndBody(const std::string& data);
std::string arrayToStringBase(const std::vector<std::string>& arr);
std::string arrayToString(const std::vector<std::vector<std::string>>& arr);
std::string concatenateHeadAndBody(const std::vector<std::string>& head, const std::vector<std::vector<std::string>>& body);

std::pair<std::string, std::vector<std::string>> recv_0(const std::vector<std::vector<std::string>>& head, const std::vector<std::vector<std::string>>& body);
std::pair<std::string, std::vector<std::string>> recv_100(const std::vector<std::vector<std::string>>& head, const std::vector<std::vector<std::string>>& body);
std::pair<std::string, std::vector<std::string>> send_100(const std::vector<std::vector<std::string>>& head, const std::vector<std::vector<std::string>>& body);

std::pair<std::vector<std::string>, std::string> processMessage(const std::string& message,const std::string& clientName,const int keysize);


#endif // MESSAGE_PROCESSOR_H
