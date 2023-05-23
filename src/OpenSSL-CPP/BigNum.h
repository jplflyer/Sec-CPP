#pragma once

#include <openssl/bn.h>

namespace Sec_CPP {
    class BigNum;
}

class Sec_CPP::BigNum
{
public:
    BigNum();
    BigNum(const BigNum &) = delete;
    BigNum & operator=(const BigNum &) = delete;

    virtual ~BigNum();

    BIGNUM & operator*() { return *bignum; }
    BIGNUM & operator->() { return *bignum; }

    void clear() { BN_clear(bignum); }
    void zero() { BN_zero(bignum); }
    void one() { BN_one(bignum); }
    void setWord(BN_ULONG w) { BN_set_word(bignum, w); }

protected:
    BIGNUM * bignum = nullptr;
};

