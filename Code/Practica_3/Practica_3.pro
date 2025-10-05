TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        codificacionchar.cpp \
        main.cpp \
        metodos_codificacion.cpp \
        sudo_lib.cpp

HEADERS += \
    codificacionchar.h \
    metodos_codificacion.h \
    sudo_lib.h
