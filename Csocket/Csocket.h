#ifndef CSOCKET_H
#define CSOCKET_H

#include <string>
#include <netinet/in.h>

int createSocket(const char* ipAddress, int port);
void startServer(void (*callback)(const std::string&, const std::string&, int), int sock);
int sendMessage(int sock, const sockaddr_in& clientAddr, const char* message);
int initializeClient(const char* serverIpAddress, int serverPort, void (*menuDrivenCallback)(int), void (*startServerCallback)(const std::string&, const std::string&, int));

#endif // CSOCKET_H
