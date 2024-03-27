#ifndef GLOBALS_H
#define GLOBALS_H

#include <iostream>
#include <string>
#include <thread>
#include <cstring>
#include <unistd.h>
#include <map>
#include <mutex>       // For mutex
#include <iostream>    // For std::cerr
#include <thread>      // For std::thread
#include <arpa/inet.h> // For sockaddr_in, inet_addr, htons
#include <unistd.h>    // For close



extern std::string clientName;
extern const int keysize;

#endif
