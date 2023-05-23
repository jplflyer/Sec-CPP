#include "BigNum.h"

/**
 * Constructor.
 */
Sec_CPP::BigNum::BigNum()
    : bignum(BN_new())
{
}

Sec_CPP::BigNum::~BigNum() {
    if (bignum != nullptr) {
        clear();	// For security.
        BN_free(bignum);
        bignum = nullptr;
    }
}
