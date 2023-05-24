#pragma once

#include <string>

#include "StringVector.h"

namespace Sec_CPP {
    namespace FileUtilities {
        bool fileExists(const std::string &filename);

        std::string readFile(const std::string &filename);
        StringVector readLines(const std::string &filename);

        void writeFile(const std::string &filename, const std::string &contents);
    }
}
