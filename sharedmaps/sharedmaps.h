#ifndef SHARED_MAPS_H
#define SHARED_MAPS_H

#include <string>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <mutex>
#include <condition_variable>
#include <map>
#include <vector>

extern std::mutex inputMutex;
extern std::condition_variable inputCV;
extern std::string userInput;
extern bool isStartServerTurn;

extern std::mutex clientsMapMutex;
extern std::mutex sskMapMutex;
extern std::mutex Message_poolMutex;
extern std::map<std::string, std::pair<std::string, int>> clientsMap;
extern std::map<std::string, std::string> ssk_map;
extern std::map<std::string, std::vector<std::string>> Message_pool;
extern std::map<std::string, std::pair<std::string, int>> kdcMap;
extern std::map<std::string, std::vector<std::string>> kdc_longeterm_ssk_map;
extern std::mutex kdcMapMutex;
extern std::mutex kdc_longeterm_ssk_mutex;

std::string getCurrentTimestamp();
void addClientToMap(const std::string& clientName, const std::string& clientIP, int clientPort);
void addsskToMap(const std::string& clientName, const std::string& key);
void addKDCToMap(const std::string& clientName, const std::string& clientIP, int clientPort);
void addkdc_sskToMap(const std::string& clientName, const std::string& prime, const std::string& generator, const std::string& privateKey, const std::string& ssk);
void addMessagepool(const std::string& message, const std::string& name, const int msgcode);

#endif // SHARED_MAPS_H
