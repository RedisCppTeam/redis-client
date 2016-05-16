#-------------------------------------------------
#
# Project created by QtCreator 2016-05-16T11:14:45
#
#-------------------------------------------------

QT       -= core gui

TARGET = redis-client
TEMPLATE = lib

DEFINES += REDISCLIENT_LIBRARY

SOURCES += Redisclient.cpp

HEADERS += Redisclient.h\
        redis-client_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
