#ifndef GLOBALS_H
#define GLOBALS_H

#include <iostream>
#include <string>
#include <thread>
#include <cstring>
#include <unistd.h>
#include <map>
#include <mutex>       
#include <iostream>    
#include <string>      
#include <thread>      
#include <arpa/inet.h> 
#include <cstring>     
#include <unistd.h>    

#include "../Csocket/Csocket.h" 
#include "../message/message_helper/message_helper.h"
#include "../message/main_message.h"
#include "../displayFunctions/displayFunctions.h"
#include "../logger/logger.h"
#include "../sharedmaps/sharedmaps.h"
#include "../sharedmaps/shared_maps_methods.h"


extern std::string clientName;
extern const int keysize;
extern DisplayText myDisplayText;
extern const char* serverIpAddress;
extern int serverPort;

#endif
