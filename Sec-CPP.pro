TEMPLATE = app

CONFIG += console c++20
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += src

SOURCES += \
        src/FileUtilities.cpp \
        src/KeyPair.cpp \
        src/OpenSSL-CPP/BigNum.cpp \
        src/OpenSSL-CPP/BinaryIO.cpp \
        src/OpenSSL-CPP/CharacterBuffer.cpp \
        src/OpenSSL-CPP/RSA.cpp \
        src/StringVector.cpp

HEADERS += \
    src/CommonUsing.h \
    src/FileUtilities.h \
    src/KeyPair.h \
    src/OpenSSL-CPP/BigNum.h \
    src/OpenSSL-CPP/BinaryIO.h \
    src/OpenSSL-CPP/CharacterBuffer.h \
    src/OpenSSL-CPP/RSA.h \
    src/StringVector.h
