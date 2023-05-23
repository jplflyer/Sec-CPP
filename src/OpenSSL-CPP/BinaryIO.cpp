#include "BinaryIO.h"

/**
 * Default constructor.
 */
Sec_CPP::BinaryIO::BinaryIO()
    : bio( BIO_new(BIO_s_mem() ))
{
}

/**
 * Destructor.
 */
Sec_CPP::BinaryIO::~BinaryIO()
{
    if (bio != nullptr) {
        BIO_free(bio);
        bio = nullptr;
    }
}
