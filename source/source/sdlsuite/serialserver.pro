QT       += core websockets serialport
QT       -= gui

TARGET = serialserver
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += main.cpp serialserver.cpp \
    serialportmanager.cpp \
    networkworker.cpp \
    settings.cpp \
    logger.cpp

HEADERS += serialserver.h \
    serialportmanager.h \
    networkworker.h \
    mac.h \
    define.h \
    datatypes.h \
    settings.h \
    logger.h

