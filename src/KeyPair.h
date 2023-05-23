//
// This got moved in from OpenSSL so that RESTServer can use it.
//
#pragma once

#include <memory>
#include <string>

namespace Sec_CPP {
    class KeyPair;
}

/**
 * This stores a public / private key pair in raw PEM text form.
 */
class Sec_CPP::KeyPair
{
public:
    typedef std::shared_ptr<KeyPair> Pointer;

    KeyPair();
    KeyPair(const std::string &pubKey, const std::string &privKey);

    const std::string & getIssuer() const     { return issuer; }
    const std::string & getPublicKey() const  { return publicKey; }
    const std::string & getPrivateKey() const { return privateKey; }

    KeyPair & setIssuer(const std::string &value) { issuer = value; return *this; }
    KeyPair & setPublicKey(const std::string &value) { publicKey = value; return *this; }
    KeyPair & setPrivateKey(const std::string &value) { privateKey = value; return *this; }

    void writeToFiles(const std::string publicFileName, const std::string & privateFileName);
    bool readFromFiles(const std::string publicFileName, const std::string & privateFileName);

protected:
    std::string issuer;
    std::string publicKey;
    std::string privateKey;
};

