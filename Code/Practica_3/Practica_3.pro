TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        bin_manager.cpp \
        codificacionchar.cpp \
        crypto_bin.cpp \
        main.cpp \
        metodos_codificacion.cpp \
        selector_codificacion.cpp \
        sudo_lib.cpp

HEADERS += \
    bin_manager.h \
    codificacionchar.h \
    crypto_bin.h \
    metodos_codificacion.h \
    selector_codificacion.h \
    sudo_lib.h
