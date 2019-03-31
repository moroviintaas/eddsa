TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
LIBS   += -lgmp -lgmpxx -lcrypto

SOURCES += \
        main.cpp \
    ed_point.cpp \
    conversions.cpp

HEADERS += \
    ed25519_params.h \
    ed_point.h \
    conversions.h
