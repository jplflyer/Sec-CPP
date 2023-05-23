#include <algorithm>

#include <cstring>

#include "CharacterBuffer.h"

/**
 * Constructor.
 */
Sec_CPP::CharacterBuffer::CharacterBuffer(size_t lengthIn)
    : length(lengthIn)
{
    if (length > 0) {
        buffer = new char[length];
    }
}

/**
 * Destructor.
 */
Sec_CPP::CharacterBuffer::~CharacterBuffer() {
    if (buffer != nullptr) {
        delete [] buffer;
        buffer = nullptr;
    }
}

/**
 * Resize with a memcpy of the old data (as much as will fit).
 */
void Sec_CPP::CharacterBuffer::resize(size_t newLength) {
    char * newBuffer = nullptr;

    if (length > 0 && newLength > 0) {
        newBuffer = new char[newLength];
        memcpy(newBuffer, buffer, std::min(length, newLength));
    }

    if (buffer != nullptr) {
        delete [] buffer;
    }

    buffer = newBuffer;
    length = newLength;
}

std::ostream & operator<<(std::ostream &str, const Sec_CPP::CharacterBuffer & cb) {
    str << cb.data();
    return str;
}
