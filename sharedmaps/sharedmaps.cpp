#include <iostream>
#include <string>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <mutex>
#include <condition_variable>
#include <map>
#include <vector>
#include <any>
#include <random>

using namespace std;
map<string, vector<string>> Random_client_map;

string getCurrentTimestamp() {
    auto now = chrono::system_clock::now();
    time_t currentTime = chrono::system_clock::to_time_t(now);
    stringstream ss;
    ss << put_time(localtime(&currentTime), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

std::string generateUUID() {
    // Generate a timestamp-based unique identifier
    auto now = std::chrono::system_clock::now();
    auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
    auto epoch = now_ms.time_since_epoch();
    auto value = std::chrono::duration_cast<std::chrono::milliseconds>(epoch);
    long duration = value.count();

    // Generate a random number
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 999999);

    // Concatenate timestamp and random number to create UUID
    return std::to_string(duration) + std::to_string(dis(gen));
}

mutex clientsMapMutex;
std::map<std::string, std::vector<std::any>> clientsMap;
void addClientToMap(const string& clientName,const std::string& clientIP, int clientPort) {
    std::lock_guard<std::mutex> lock(clientsMapMutex);
    std::vector<std::any> clientInfo;
    clientInfo.push_back(clientName);
    clientInfo.push_back(clientIP);
    clientInfo.push_back(std::to_string(clientPort)); // Convert int to string
    clientsMap[generateUUID()] = clientInfo;
}

std::mutex kdcMapMutex;
std::map<std::string, std::vector<std::any>> kdcMap;

void addKDCToMap(const std::string& clientName, const std::string& clientIP, int clientPort) {
    std::lock_guard<std::mutex> lock(kdcMapMutex);
    std::vector<std::any> clientInfo;
    clientInfo.push_back(clientIP);
    clientInfo.push_back(clientPort);
    kdcMap[clientName] = clientInfo;
}




mutex sskMapMutex;
std::map<string, string> ssk_map;
void addsskToMap(const string& clientName, const string& key) {
    lock_guard<mutex> lock(sskMapMutex);
    ssk_map[clientName] = key;
}

mutex kdc_longeterm_ssk_mutex;
map<string, vector<string>> kdc_longeterm_ssk_map;
void addkdc_sskToMap(const string& clientName, const string& prime, const string& generator, const string& privateKey, const string& ssk) {
    lock_guard<mutex> lock(kdc_longeterm_ssk_mutex);
    vector<string> entry {prime, generator, privateKey, ssk};
    kdc_longeterm_ssk_map[clientName] = entry;
}

mutex Message_poolMutex; 
map<string, vector<string>> Message_pool;

void addMessagepool(const string& message, const string& name, const int msgcode) {
    lock_guard<mutex> lock(Message_poolMutex);
    vector<string> entry {name, to_string(msgcode), message};
    Message_pool[getCurrentTimestamp()] = entry;
}