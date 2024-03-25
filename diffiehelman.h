#ifndef DIFFIEHELMAN_H
#define DIFFIEHELMAN_H

#include <iostream>
#include <vector>
#include <string>
#include <cryptopp/dh.h>
#include <cryptopp/osrng.h>
#include <cryptopp/hex.h> 

using namespace CryptoPP;
using namespace std;

// Convert Integer to String
std::string IntegerToString(const CryptoPP::Integer& p);

// Generate global parameters and half key
std::vector<std::string> GenerateGlobalParametersAndHalfKey(int keySize);

// Generate full key from parameters for KDC
std::string GenerateFullKeyFromParameters_kdc(const std::vector<std::string>& paramsAndHalfKey, const std::string& privateKeyStr);

// Generate full key from parameters
std::vector<std::string> GenerateFullKeyFromParameters(const std::vector<std::string>& paramsAndHalfKey);

// Convert String to Integer
CryptoPP::Integer StringToInteger(const std::string& str);

#endif // DIFFIEHELMAN_H
