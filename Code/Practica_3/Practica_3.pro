TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
        metodos_codificacion.cpp \
        sudo_lib.cpp

HEADERS += \
    metodos_codificacion.h \
    sudo_lib.h
