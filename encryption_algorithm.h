#ifndef ENCRYPTION_ALGORITHM_H
#define ENCRYPTION_ALGORITHM_H

#include <string>
#include <cryptopp/secblock.h>

CryptoPP::SecByteBlock GenerateAESKey(int keySize);
std::string Encrypt(const std::string& plain, const CryptoPP::SecByteBlock& key);
std::string Decrypt(const std::string& cipher, const CryptoPP::SecByteBlock& key);
CryptoPP::SecByteBlock StringToSecByteBlock(const std::string& str);
std::string SecByteBlockToString(const CryptoPP::SecByteBlock& block);


#endif // ENCRYPTION_ALGORITHM_H
