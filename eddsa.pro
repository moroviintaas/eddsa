TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
LIBS   += -lgmp -lgmpxx -lcrypto -lssl -L/usr/local/lib
SOURCES += \
        main.cpp \
    ed_point.cpp \
    conversions.cpp \
    crypto_ops.cpp \
    math_ops.cpp

HEADERS += \
    ed25519_params.h \
    ed_point.h \
    conversions.h \
    crypto_ops.h \
    math_ops.h
