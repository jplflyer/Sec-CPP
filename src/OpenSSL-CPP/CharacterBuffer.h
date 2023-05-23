#pragma once

#include <iostream>
#include <string>

namespace Sec_CPP {
    class CharacterBuffer;
}

/**
 * This provides RAII around character buffers allocated with new.
 */
class Sec_CPP::CharacterBuffer
{
public:
    CharacterBuffer(size_t lengthIn = 0);
    CharacterBuffer(const CharacterBuffer &) = delete;
    CharacterBuffer & operator=(const CharacterBuffer &) = delete;

    virtual ~CharacterBuffer();

    char & operator*() { return *buffer; }
    char & operator->() { return *buffer; }

    char * data() { return buffer; }
    const char * data() const { return buffer; }

    char & operator[](size_t n) { return buffer[n]; }

    void resize(size_t newLength);

protected:
    size_t length = 0;
    char * buffer = nullptr;
};

std::ostream & operator<<(std::ostream &, const Sec_CPP::CharacterBuffer &);
