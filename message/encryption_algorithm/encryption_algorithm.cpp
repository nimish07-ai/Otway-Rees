#include <iostream>
#include <string>
#include <cryptopp/osrng.h>
#include <cryptopp/integer.h>
#include <cryptopp/aes.h>
#include <cryptopp/modes.h>
#include <cryptopp/filters.h>
#include <cryptopp/hex.h>
#include <cryptopp/secblock.h>
#include <string>

using namespace CryptoPP;
using namespace std;

SecByteBlock GenerateAESKey(int keySize)
{
    AutoSeededRandomPool prng;
    SecByteBlock key(keySize);
    prng.GenerateBlock(key, keySize);
    return key;
}

string Encrypt(const string& plain, const SecByteBlock& key)
{
    string cipher;

    try
    {
        ECB_Mode<AES>::Encryption e;
        e.SetKey(key, key.size());

        // The StreamTransformationFilter adds padding
        // as required. ECB and CBC Mode must be padded
        // to the block size of the cipher.
        StringSource ss1(plain, true,
            new StreamTransformationFilter(e,
                new StringSink(cipher)
            ) // StreamTransformationFilter      
        ); // StringSource
    }
    catch (CryptoPP::Exception& e)
    {
        cerr << e.what() << endl;
        exit(1);
    }

    return cipher;
}

string Decrypt(const string& cipher, const SecByteBlock& key)
{
    string recovered;

    try
    {
        ECB_Mode<AES>::Decryption d;
        // ECB Mode does not use an IV
        d.SetKey(key, key.size());

        // The StreamTransformationFilter removes
        // padding as required.
        StringSource ss3(cipher, true,
            new StreamTransformationFilter(d,
                new StringSink(recovered)
            ) // StreamTransformationFilter
        ); // StringSource
    }
    catch (CryptoPP::Exception& e)
    {
        cerr << e.what() << endl;
        exit(1);
    }

    return recovered;
}

SecByteBlock StringToSecByteBlock(const string& str)
{
    return SecByteBlock(reinterpret_cast<const CryptoPP::byte*>(str.data()), str.size());
}
string SecByteBlockToString(const SecByteBlock& block)
{
    string str;
    HexEncoder encoder(new StringSink(str));

    encoder.Put(block, block.size());
    encoder.MessageEnd();

    return str;
}

// int main()
// {
//     int keySize = AES::DEFAULT_KEYLENGTH;
//     SecByteBlock key = GenerateAESKey(keySize);
//     cout << "Generated AES Key: " << SecByteBlockToString(key) << endl;

//     string plainText = "Hello, world!";
//     cout << "Plain text: " << plainText << endl;

//     string cipherText = Encrypt(plainText, key);
//     cout << "Cipher text: " << SecByteBlockToString(StringToSecByteBlock(cipherText)) << endl;

//     string decryptedText = Decrypt(cipherText, key);
//     cout << "Decrypted text: " << decryptedText << endl;

//     // Testing conversion from string to SecByteBlock
//     string testString = "This is a test string.";
//     SecByteBlock testBlock = StringToSecByteBlock(testString);
//     cout << "Test string: " << testString << endl;
//     cout << "Test SecByteBlock: " << SecByteBlockToString(testBlock) << endl;

//     return 0;
// }
