# Sec-CPP
This is a small library to provide some simple support for JWT and public/private keys and other very simple uses of OpenSSL, all from C++. It's minimalistic, and I'm only making it at all because some of this stuff takes too much time to figure out.

# Dependencies

 - OpenSSL
 - libcrypto
 - jwt-cpp from https://github.com/Thalhammer/jwt-cpp
 - A modern C++ compiler and GNU make

# Build and Install
This should build and install on Linux and Mac.

    make dependencies
    make
    sudo make install

`make dependencies` verifies jwt-cpp is installed or installs it locally otherwise. This is only required once.

The Makefile targets C++ 17. At the time of this writing, g++ still reports C++ 20 support as experimental.

# Contributing
I'm not opposed to improvements. Feel free to fork the repo, make enhancements, and issue pull requests.

# License
This library is placed under the very permissive MIT license, with the details in the LICENSE file.
