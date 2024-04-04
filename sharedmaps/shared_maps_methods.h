#ifndef SHARED_MAPS_METHODS
#define SHARED_MAPS_METHODS

#include "../global/global.h"

void add_client();
void establish_session_key_with_client(int sock);
void Message_Pool_check();

void Send_Message_To_Client(int sock);
void add_kdc();
void Establish_SSK_With_KDC();

#endif