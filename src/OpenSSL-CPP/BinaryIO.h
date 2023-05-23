#pragma once

#include <openssl/bio.h>

namespace Sec_CPP {
    class BinaryIO;
}

/**
 * Wraps BIO. Currently we only construct to use a memory buffer, but
 * if you need to write to a file, we'll just need to create a constructor to support it.
 */
class Sec_CPP::BinaryIO
{
public:
    BinaryIO();
    BinaryIO(const BinaryIO &) = delete;
    BinaryIO & operator=(const BinaryIO &) = delete;

    virtual ~BinaryIO();

    /** Return the BIO reference. Exists for our other classes. */
    BIO & operator*() { return *bio; }

    /** Return the BIO reference. Exists for our other classes. */
    BIO & operator->() { return *bio; }

    /** Access the raw pointer. This really exists to make things easier for our other classes. */
    BIO * rawPtr() { return bio; }

protected:
    BIO * bio = nullptr;
};

