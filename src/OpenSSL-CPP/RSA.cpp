#include <openssl/pem.h>

#include <Sec-CPP/CommonUsing.h>

#include "BinaryIO.h"
#include "CharacterBuffer.h"
#include "RSA.h"

Sec_CPP::RSA::RSA()
    : rsa(RSA_new())
{
}

Sec_CPP::RSA::~RSA() {
    if (rsa != nullptr) {
        RSA_free(rsa);
        rsa = nullptr;
    }
}

Sec_CPP::KeyPair Sec_CPP::RSA::generateKey(int bits, Sec_CPP::BigNum & bn) {
    RSA_generate_key_ex(rsa, bits, &(*bn), NULL);

    BinaryIO bpPublic;
    BinaryIO bpPrivate;

    // Note use of RSA_PUBKEY instead of RSA_PublicKey
    PEM_write_bio_RSA_PUBKEY( bpPublic.rawPtr(), rsa );

    PEM_write_bio_RSAPrivateKey( bpPrivate.rawPtr(), rsa, nullptr, nullptr, 0, nullptr, nullptr );

    size_t privateLen = BIO_pending( bpPrivate.rawPtr() );
    size_t publicLen = BIO_pending( bpPublic.rawPtr() );
    CharacterBuffer privateBuffer(privateLen + 1);
    CharacterBuffer publicBuffer(publicLen + 1);

    BIO_read(bpPrivate.rawPtr(), privateBuffer.data(), privateLen);
    BIO_read(bpPublic.rawPtr(), publicBuffer.data(), publicLen);

    privateBuffer[privateLen] = 0;
    publicBuffer[publicLen] = 0;

    KeyPair pair(publicBuffer.data(), privateBuffer.data() );
    return pair;
}
