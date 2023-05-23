#include <algorithm>
#include <iostream>
#include <string>
#include <string.h>
#include <vector>

#include "StringVector.h"

using namespace Sec_CPP;

using std::string;

/** Constructor. */
StringVector::StringVector() {
}

/**
 * Constructor.
 */
StringVector::StringVector(const std::string &str, const char splitter) {
    tokenize(str, splitter);
}


/*
 * We actually do proper destruction knowing that we hold pointers.
 */
StringVector::~StringVector() {
}

void
StringVector::add(const string &value) {
    push_back( std::make_shared<string>(value) );
}

void
StringVector::addFront(const string &value) {
    insert(begin(), std::make_shared<string>(value) );
}

void
StringVector::remove(const string &value) {
    iterator it = std::remove_if(begin(), end(), [=](const std::shared_ptr<string> &ptr){
        return ptr != nullptr && *ptr == value;
    } );

    erase(it, end());
}

/**
 * Populate ourself by splitting this string at any of these delimiters.
 */
void
StringVector::tokenize(const char *str, const char *splitter) {
	char * dup = strdup(str);
	char * dupPtr = dup;
    char * context = nullptr;
	char * ptr;

    while ((ptr = strtok_r(dupPtr, splitter, &context)) != nullptr) {
        dupPtr = nullptr;	// For subsequent calls

        push_back(std::make_shared<string>(ptr));
	}

	// Free our duplicate copy.
	free(dup);
}

void
StringVector::tokenize(const std::string &str, const char splitter) {
    size_t lastPos = 0;
    size_t pos;

    while ((pos = str.find(splitter, lastPos)) != string::npos) {
        string thisStr = str.substr(lastPos, pos - lastPos);
        push_back(std::make_shared<string>(thisStr));
        lastPos = pos + 1;
    }
    if (lastPos == 0) {
        // No newlines at all -- just add the entire string.
        push_back(std::make_shared<string>(str));
    }
    else {
        // There was at least one, so push everything after the last newline.
        push_back(std::make_shared<string>(str.substr(lastPos)));
    }
}

/**
 * Do we have this string?
 */
bool
StringVector::contains(const std::string &str) {
    return findIf([&](const Pointer &ptr) { return *ptr == str; } ) != nullptr;
}

/**
 * Finder with a comparator.
 */
StringVector::Pointer StringVector::findIf(StringVector::Comparator comp) const {
    auto objIter = std::find_if(this->cbegin(), this->cend(), comp);

    return (objIter == this->cend()) ? nullptr : *objIter;
}

std::ostream &
operator<<(std::ostream &ostr, const Sec_CPP::StringVector &vec) {
    string delim = "";

    for (const StringVector::Pointer &ptr: vec) {
        ostr << delim << *ptr;
        delim = ", ";
    }

    return ostr;
}
