#ifndef CSOCKET_H
#define CSOCKET_H

#include <iostream> 
#include <string> 
#include <map> 
#include <mutex> 
#include <netinet/in.h> 
#include <arpa/inet.h> 
#include <sys/socket.h> 
#include <cstring> 
#include <condition_variable>
#include <vector> 
#include <unistd.h> 
#include <thread> 
#include <sstream> 
#include <iomanip>
#include <random>
#include <chrono>
#include <ctime>

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
extern std::map<std::string,std::vector<std::string>> kdc_longeterm_ssk_map;


extern std::mutex kdcMapMutex;
extern std::mutex kdc_longeterm_ssk_mutex;

std::string getCurrentTimestamp();

void addClientToMap(const std::string& clientName, const std::string& clientIP, int clientPort);
void addsskToMap(const std::string& clientName, const std::string& key);
void addKDCToMap(const std::string& clientName, const std::string& clientIP, int clientPort);
void addkdc_sskToMap(const std::string& clientName, const std::string& key);
void addMessagepool(const std::string& message, const std::string& name, const int msgcode);

int createSocket(const char* ipAddress, int port);
void startServer(void (*callback)(const std::string&, const std::string&, int), int sock);
int sendMessage(int sock, const sockaddr_in& clientAddr, const char* message);
int initializeClient(const char* serverIpAddress, int serverPort, void (*menuDrivenCallback)(int), void (*startServerCallback)(const std::string&, const std::string&, int));

#endif // CSOCKET_H
