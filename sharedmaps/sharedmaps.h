#ifndef SHARED_MAPS_H
#define SHARED_MAPS_H

#include <iostream>
#include <string>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <mutex>
#include <map>
#include <vector>
#include <any>
#include <random>

extern std::map<std::string, std::vector<std::string>> Random_client_map;

std::string getCurrentTimestamp();

std::string generateUUID();

extern std::mutex clientsMapMutex;
extern std::map<std::string, std::vector<std::any>> clientsMap;
void addClientToMap(const std::string& clientName, const std::string& clientIP, int clientPort);

extern std::mutex kdcMapMutex;
extern std::map<std::string, std::vector<std::any>> kdcMap;
void addKDCToMap(const std::string& clientName, const std::string& clientIP, int clientPort);

extern std::mutex sskMapMutex;
extern std::map<std::string, std::string> ssk_map;
void addsskToMap(const std::string& clientName, const std::string& key);

extern std::mutex kdc_longeterm_ssk_mutex;
extern std::map<std::string, std::vector<std::string>> kdc_longeterm_ssk_map;
void addkdc_sskToMap(const std::string& clientName, const std::string& prime, const std::string& generator, const std::string& privateKey, const std::string& ssk);

extern std::mutex Message_poolMutex;
extern std::map<std::string, std::vector<std::string>> Message_pool;
void addMessagepool(const std::string& message, const std::string& name, int msgcode);

#endif // SHARED_MAPS_H
