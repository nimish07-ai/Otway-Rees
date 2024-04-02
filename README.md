       .aMMMb dMMMMMMP dMP dMP dMP .aMMMb  dMP dMP 
      dMP"dMP   dMP   dMP dMP dMP dMP"dMP dMP.dMP  
     dMP dMP   dMP   dMP dMP dMP dMMMMMP  VMMMMP   
    dMP.aMP   dMP   dMP.dMP.dMP dMP dMP dA .dMP    
    VMMMP"   dMP    VMMMPVMMP" dMP dMP  VMMMP"     
                                                   
        dMMMMb  dMMMMMP dMMMMMP .dMMMb             
       dMP.dMP dMP     dMP     dMP" VP             
      dMMMMK" dMMMP   dMMMP    VMMMb               
     dMP"AMF dMP     dMP     dP .dMP               
    dMP dMP dMMMMMP dMMMMMP  VMMMP"                

===============================================
Implementing the Otway-Rees Protocol in C++17
===============================================

This project implements the Otway-Rees protocol in C++17 without relying on any external libraries. The Otway-Rees protocol is a key exchange protocol that allows two parties to establish a shared secret key over an insecure network.

Table of Contents
-----------------
1. Overview
2. Requirements
3. Usage
4. Implementation Details
```
1. Overview
-----------
The Otway-Rees protocol provides a secure way for two parties to authenticate each other and establish a shared secret key, which can then be used for secure communication. This README provides an overview of our C++ implementation of the protocol.
```
```
2. Requirements
---------------
To compile and run this project, you need:
- A C++17 compatible compiler (e.g., GCC, Clang)
- A development environment (e.g., Visual Studio Code, Xcode, or any IDE of your choice)
```

```
3. Usage
--------
To use this implementation of the Otway-Rees protocol:
1. Clone this repository to your local machine.
2. Compile the source files using your C++17 compiler.
3. Execute the compiled binary to run the protocol.
```
```
4. Implementation Details
-------------------------
Our implementation follows the specifications of the Otway-Rees protocol and is written entirely in C++17. Here are some key points about the implementation:
```

### Message Format

The message format follows the structure:
```
"head :::::::: body"
```
```
 "[ \"receiverip_addres\",\"reciver_port\",\"sender_ip_address\",\"sender_port\",\"message code\" ]:::::::[ \"message1\" ]|[ \"message2\" ]"
```

#### Message Code

- **100 series**: For long-term shared secret key establishment between KDC and client.
  - `100`: Client sends KDC request to register and receives KDC's global parameters.
  - `101`: KDC sends back its global parameters and half key.
  - `102`: Client sends its half key.
  - `103`: KDC sends a nonce encrypted with the full key.
  - `104`: Client decrements the nonce by 1 and returns the result encrypted with SSK.
  - `105`: KDC sends OK message.

- **200 series**: For establishing a session key between two clients.
  - `200`: Client 1 sends a request to client 2 to establish a key.
  - `201`: Client 2 sends a request to KDC if it trusts the mentioned KDC.
  - `203`: KDC verifies the encrypted message and sends back necessary details.
  - `204`: Client 2 sends necessary details to client 1.
  - `205`: Client 1 sends a done message encrypted with the session key.

- **300 series**: For indicating error messages. Details to be elaborated later.
  - `300`: incorrect message format received.
  - `301`: incorrect message_code -message code dosenot exist .
  - `302`: kdc sends a resoponse to Client 2  telling erro cant establish ssk with client 1.
  - `303`: Client 2 sends error response received by kdc while establishing ssk.
  - `304`: Some internal error at client end.
 


### Descriotion of each folder
**Csocket** :
  THis handels every thing from socket binding and thread heandelign for request acceptance and user interface operations

**displayFunctions** : 
  THis bueatifiies the User Input Screen by defining rules and data structure to print data on user screen
**logger** : 
  This Lodges data into a logger map based on 4 level whih user can view latter
**message** : 
  This is the main block in whole paradigm of handiling message request the whole paradig is setuped in middleware formart 
  firstly when a packet is arrived it is parrsed to convert it into header vector , body vector and message code ,
  now according to meessage code it is latter sent to there specific module to be handled and the return value from that module is paseed back as response for request

**message_helper** : 
  it contain important methods required for message handeling such as string_to_message_data_tpe message_data_type_to_string,header formatter

**mes_100** : 
  all meesage with code >= 100 and <200 are handele here
  process_100s is the method which decides what is to be donr when req has been sent here
**mes_200** :
  all meesage with code >= 200 and <300 are handele here
  process_100s is the method which decides what is to be donr when req has been sent here
**mes_300** : 
  all meesage with code >= 300 and <400 are handele here
  process_300s is the method which decides what is to be donr when req has been sent here
**diffiehelman** :
  Here we have functions related to handeling diffie helman protocol such as parameter creation as well as half key and full key genration
**encryption_algorithm** :
  All methods realted to encpytion are performed herer
**sharedmaps** :
  Shared maps are the maps sahred among all the above modules for memmory storage and modules to complete the working og it


#start.sh

it takes 2 arguments filenemae compile output name

eg:
  ./start.sh client.cpp client
