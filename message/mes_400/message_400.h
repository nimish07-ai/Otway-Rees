#ifndef MESSAGE_400_H
#define MESSAGE_400_H

#include "../../global/global.h"
#include <vector>
#include <string>


std::pair<std::vector<std::string>, std::string> send_400(const std::string& senderip, const std::string& senderport, const std::string& receiverip, const std::string& receiverport,const std::string& message);

std::pair<std::vector<std::string>, std::string> send_401(const std::string& senderip, const std::string& senderport, const std::string& receiverip, const std::string& receiverport);

// Function declaration for processMessage_300s
std::pair<std::vector<std::string>, std::string> processMessage_400s(std::vector<std::string>& header, std::vector<std::vector<std::string>>& body, int messageCode);

#endif // MESSAGE_300_H
