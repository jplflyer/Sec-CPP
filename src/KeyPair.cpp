
#include "CommonUsing.h"
#include "FileUtilities.h"

#include "KeyPair.h"

/**
 * Default constructor.
 */
Sec_CPP::KeyPair::KeyPair() {
}

/**
 * Construct from our values.
 */
Sec_CPP::KeyPair::KeyPair(const string & pubKey, const string &privKey )
    : publicKey(pubKey), privateKey(privKey)
{
}

/**
 * Write the keys to these files. They will be in PEM format.
 */
void Sec_CPP::KeyPair::writeToFiles(const std::string publicFileName, const std::string &privateFileName)
{
    FileUtilities::writeFile(publicFileName, publicKey);
    FileUtilities::writeFile(privateFileName, privateKey);
}

/**
 * Read the keys from these files. They are assumed in PEM format.
 */
bool Sec_CPP::KeyPair::readFromFiles(const std::string publicFileName, const std::string &privateFileName)
{
    if (!FileUtilities::fileExists(publicFileName) || !FileUtilities::fileExists(privateFileName)) {
        cerr << "KeyPair::readFromFiles() but one of the files does not exist.\n";
        return false;
    }

    publicKey = FileUtilities::readFile(publicFileName);
    privateKey = FileUtilities::readFile(privateFileName);

    return true;
}
