TEMPLATE = app

CONFIG += console c++20
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += src

SOURCES += \
    src/OpenSSL-CPP/BigNum.cpp \
    src/OpenSSL-CPP/BinaryIO.cpp \
    src/OpenSSL-CPP/CharacterBuffer.cpp \
    src/OpenSSL-CPP/RSA.cpp \
    src/Sec-CPP/FileUtilities.cpp \
    src/Sec-CPP/KeyPair.cpp \
    src/Sec-CPP/Sec_CPP.cpp \
    src/Sec-CPP/StringVector.cpp \
    examples/SimpleExample.cpp

HEADERS += \
    src/OpenSSL-CPP/BigNum.h \
    src/OpenSSL-CPP/BinaryIO.h \
    src/OpenSSL-CPP/CharacterBuffer.h \
    src/OpenSSL-CPP/RSA.h \
    src/Sec-CPP/CommonUsing.h \
    src/Sec-CPP/FileUtilities.h \
    src/Sec-CPP/KeyPair.h \
    src/Sec-CPP/Sec_CPP.h \
    src/Sec-CPP/StringVector.h
