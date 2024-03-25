#include "diffiehelman.h"
#include <iostream>
#include <vector>
#include <string>
#include <cryptopp/dh.h>
#include <cryptopp/osrng.h>
#include <cryptopp/hex.h> // Include the necessary Crypto++ header for HexEncoder and StringSink

using namespace CryptoPP;
using namespace std;

string IntegerToString(const Integer& p) {
    return IntToString<Integer>(p, 10);
}

std::vector<std::string> GenerateGlobalParametersAndHalfKey(int keySize) {
    AutoSeededRandomPool rnd;

    // Generate DH parameters
    DH dh;
    dh.AccessGroupParameters().GenerateRandomWithKeySize(rnd, keySize);

    // Generate a private key
    Integer privateKey;
    privateKey.Randomize(rnd, Integer::One(), dh.GetGroupParameters().GetSubgroupOrder() - Integer::One());

    // Compute the half key
    Integer halfKey = dh.GetGroupParameters().ExponentiateBase(privateKey);

    // Store global parameters, private key, and half key in a vector of strings
    std::vector<std::string> result;
    result.push_back("Prime: " + IntegerToString(dh.GetGroupParameters().GetModulus()));
    result.push_back("Generator: " + IntegerToString(dh.GetGroupParameters().GetGenerator()));
    result.push_back("Private Key: " + IntegerToString(privateKey));
    result.push_back("Half Key: " + IntegerToString(halfKey));

    return result;
}


std::string GenerateFullKeyFromParameters_kdc(const std::vector<std::string>& paramsAndHalfKey, const std::string& privateKeyStr) {
    // Retrieve parameters and half key from the vector
    const std::string& primeStr = paramsAndHalfKey[0].substr(paramsAndHalfKey[0].find(": ") + 2);
    const std::string& generatorStr = paramsAndHalfKey[1].substr(paramsAndHalfKey[1].find(": ") + 2);
    const std::string& halfKeyStr = paramsAndHalfKey[2].substr(paramsAndHalfKey[2].find(": ") + 2);

    // Convert parameters and half key back to their respective types
    Integer prime(primeStr.c_str());
    Integer generator(generatorStr.c_str());
    Integer halfKey(halfKeyStr.c_str());

    // Convert private key string to Integer
    Integer privateKey(privateKeyStr.c_str());

    // Create DH parameters
    DH dh;
    dh.AccessGroupParameters().Initialize(prime, generator);

    // Compute the full key
    Integer fullKey = dh.GetGroupParameters().ExponentiateBase(privateKey) + halfKey;

    // Convert the full key to a string
    std::string fullKeyStr = IntegerToString(fullKey);

    return fullKeyStr;
}

std::vector<std::string>  GenerateFullKeyFromParameters(const std::vector<std::string>& paramsAndHalfKey) {
    // Retrieve parameters and half key from the vector
    const std::string& primeStr = paramsAndHalfKey[0].substr(paramsAndHalfKey[0].find(": ") + 2);
    const std::string& generatorStr = paramsAndHalfKey[1].substr(paramsAndHalfKey[1].find(": ") + 2);
    const std::string& halfKeyStr = paramsAndHalfKey[2].substr(paramsAndHalfKey[2].find(": ") + 2);

    // Convert parameters and half key back to their respective types
    Integer prime(primeStr.c_str());
    Integer generator(generatorStr.c_str());
    Integer halfKey(halfKeyStr.c_str());

    // Create DH parameters
    DH dh;
    dh.AccessGroupParameters().Initialize(prime, generator);

    // Generate a private key
    AutoSeededRandomPool rnd;
    Integer privateKey;
    privateKey.Randomize(rnd, Integer::One(), dh.GetGroupParameters().GetSubgroupOrder() - Integer::One());

    // Generate the second half key
    Integer secondHalfKey;
    secondHalfKey.Randomize(rnd, Integer::One(), dh.GetGroupParameters().GetSubgroupOrder() - Integer::One());

    // Compute the full key
    Integer fullKey = dh.GetGroupParameters().ExponentiateBase(privateKey) + halfKey + secondHalfKey;

    // Convert the full key and second half key to strings
    std::string fullKeyStr = IntegerToString(fullKey);
    std::string secondHalfKeyStr = IntegerToString(secondHalfKey);

    // Prepare the result
    std::vector<std::string> result;
    result.push_back("Prime: " + IntegerToString(dh.GetGroupParameters().GetModulus()));
    result.push_back("Generator: " + IntegerToString(dh.GetGroupParameters().GetGenerator()));
    result.push_back("Private Key: " + IntegerToString(privateKey));
    result.push_back("Half Key: " + secondHalfKeyStr);
    result.push_back("Full Key: " + fullKeyStr);

    // Return the concatenated string
    // return arrayToStringBase(result);
    return result;
}





// std::string GenerateFullKeyFromParameters(const std::vector<std::string>& paramsAndHalfKey) {
//     // Retrieve parameters and half key from the vector
//     const std::string& primeStr = paramsAndHalfKey[0].substr(paramsAndHalfKey[0].find(": ") + 2);
//     const std::string& generatorStr = paramsAndHalfKey[1].substr(paramsAndHalfKey[1].find(": ") + 2);
//     const std::string& halfKeyStr = paramsAndHalfKey[2].substr(paramsAndHalfKey[2].find(": ") + 2);

//     // Convert parameters and half key back to their respective types
//     Integer prime(primeStr.c_str());
//     Integer generator(generatorStr.c_str());
//     Integer halfKey(halfKeyStr.c_str());

//     // Create DH parameters
//     DH dh;
//     dh.AccessGroupParameters().Initialize(prime, generator);

//     // Generate a private key
//     AutoSeededRandomPool rnd;
//     Integer privateKey;
//     privateKey.Randomize(rnd, Integer::One(), dh.GetGroupParameters().GetSubgroupOrder() - Integer::One());
//     //generate second halkf key
//     // Compute the full key
//     Integer fullKey = dh.GetGroupParameters().ExponentiateBase(privateKey) + halfKey;

//     // Convert the full key to a string
//     std::string fullKeyStr = IntegerToString(fullKey);

//     return fullKeyStr;
// }

Integer StringToInteger(const std::string& str) {
    return Integer(str.c_str());
}

