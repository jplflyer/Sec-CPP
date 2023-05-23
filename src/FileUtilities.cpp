#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sys/stat.h>

#include "CommonUsing.h"
#include "FileUtilities.h"
#include "StringVector.h"


using namespace Sec_CPP::FileUtilities;

/**
 * Does this file exist?
 */
bool fileExists(const std::string &filename) {
    struct stat buf;
    return (stat(filename.c_str(), &buf) == 0);
}

/**
 * Read file contents.
 */
string readFile(const string &filename)
{
	std::ifstream ifs(filename);
    if (!ifs.good()) {
        return "";
    }

	return string( (std::istreambuf_iterator<char>(ifs) ), (std::istreambuf_iterator<char>() ) );
}

/**
 * Read lines from the file, returning one line per entry in the vector.
 */
Sec_CPP::StringVector readLines(const std::string &filename) {
    Sec_CPP::StringVector vec;
    std::ifstream ifs(filename);
    string line;

    while (getline(ifs, line)) {
        vec.add(line);
    }

    return vec;
}

/**
 * Write the contents to the file.
 */
void writeFile(const string &filename, const string &contents) {
    std::ofstream ofs(filename);
    ofs << contents;
}
