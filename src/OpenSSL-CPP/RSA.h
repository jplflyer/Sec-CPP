#pragma once

#include <openssl/rsa.h>
#include <Sec-CPP/KeyPair.h>

#include "BigNum.h"

namespace Sec_CPP {
    class RSA;
}

class Sec_CPP::RSA
{
public:
    RSA();
    RSA(const RSA &) = delete;
    RSA & operator=(const RSA &) = delete;

    virtual ~RSA();

    ::RSA & operator*() { return *rsa; }
    ::RSA & operator->() { return *rsa; }

    KeyPair generateKey(int bits, BigNum &bn);

protected:
    ::RSA * rsa = nullptr;
};

