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

using namespace std;



mutex clientsMapMutex;
mutex sskMapMutex;
mutex Message_poolMutex; 
mutex kdcMapMutex;
mutex kdc_longeterm_ssk_mutex;

map<string, pair<string, int>> clientsMap;
map<string, string> ssk_map;
map<string, vector<string>> Message_pool;

map<string, pair<string, int>> kdcMap;
map<string, vector<string>> kdc_longeterm_ssk_map;
map<string, vector<string>> Random_client_map;




string getCurrentTimestamp() {
    auto now = chrono::system_clock::now();
    time_t currentTime = chrono::system_clock::to_time_t(now);
    stringstream ss;
    ss << put_time(localtime(&currentTime), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}


void addClientToMap(const string& clientName, const string& clientIP, int clientPort) {
    lock_guard<mutex> lock(clientsMapMutex);
    clientsMap[clientName] = make_pair(clientIP, clientPort);
}

void addsskToMap(const string& clientName, const string& key) {
    lock_guard<mutex> lock(sskMapMutex);
    ssk_map[clientName] = key;
}

void addKDCToMap(const string& clientName, const string& clientIP, int clientPort) {
    lock_guard<mutex> lock(kdcMapMutex);
    kdcMap[clientName] = make_pair(clientIP, clientPort);
}

void addkdc_sskToMap(const string& clientName, const string& prime, const string& generator, const string& privateKey, const string& ssk) {
    lock_guard<mutex> lock(kdc_longeterm_ssk_mutex);
    vector<string> entry {prime, generator, privateKey, ssk};
    kdc_longeterm_ssk_map[clientName] = entry;
}

void addMessagepool(const string& message, const string& name, const int msgcode) {
    lock_guard<mutex> lock(Message_poolMutex);
    vector<string> entry {name, to_string(msgcode), message};
    Message_pool[getCurrentTimestamp()] = entry;
}



