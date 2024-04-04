#ifndef MESSAGE_100
#define MESSAGE_100

#include <vector>
#include <string>
#include "../../global/global.h"

// extern  std::string clientName;
// extern DisplayText myDisplayText;
// extern   int keysize;

// Function to send a message with code 100
std::pair<std::vector<std::string>, std::string> send_100(const std::string& senderip, const std::string& senderport, const std::string& receiverip, const std::string& receiverport);

// Function to send a message with code 101
std::pair<std::vector<std::string>, std::string> send_101(const std::string& senderip, const std::string& senderport, const std::string& receiverip, const std::string& receiverport, const std::string& name, int keysize);


// Function to send a message with code 102
std::pair<std::vector<std::string>, std::string> send_102(const std::string& senderip, const std::string& senderport, const std::string& receiverip, const std::string& receiverport,const std::string& name,const std::vector<std::string>& paramsAndHalfKey);


// Function to send a message with code 103
std::pair<std::vector<std::string>, std::string> send_103(const std::string& senderip, const std::string& senderport, const std::string& receiverip, const std::string& receiverport,const std::string& name,const std::string& key,const std::string& num);

// Function to send a message with code 104
std::pair<std::vector<std::string>, std::string> send_104(const std::string& senderip, const std::string& senderport, const std::string& receiverip, const std::string& receiverport,const std::string& name,const std::string& key,const std::string& num);

// Declaration of processMessage_100s
std::pair<std::vector<std::string>, std::string> processMessage_100s(std::vector<std::vector<std::string>>& header, std::vector<std::vector<std::string>>& body, int messageCode);

#endif // MESSAGE_FUNCTIONS_H
