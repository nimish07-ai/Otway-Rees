#ifndef MESSAGE_200_H
#define MESSAGE_200_H
// #include "../../gloabl.h"
#include "../../global/global.h"

#include <vector>
#include <string>


// Function declaration for processMessage_500s
std::pair<std::vector<std::string>, std::string> processMessage_200s(std::vector<std::string>& header, std::vector<std::vector<std::string>>& body, int messageCode);

#endif // MESSAGE_500_H
