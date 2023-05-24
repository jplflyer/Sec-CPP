#include <OpenSSL-CPP/RSA.h>
#include <OpenSSL-CPP/BigNum.h>

#include "Sec_CPP.h"

Sec_CPP::KeyPair Sec_CPP::generateKeys() {
    RSA rsa;
    BigNum bne;

    bne.setWord(RSA_F4);

    KeyPair pair = rsa.generateKey(2048, bne);
    return pair;
}
