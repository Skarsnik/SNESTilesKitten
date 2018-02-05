#-------------------------------------------------
#
# Project created by QtCreator 2017-03-20T13:38:16
#
#-------------------------------------------------

QT       += core gui

TARGET = nintendocompression
TEMPLATE = lib
CONFIG += plugin

QMAKE_CFLAGS += "-std=c99"

DESTDIR = ../../plugins

SOURCES += nintendo.cpp \
    ../../lowlevelstuff/src/compressions/stdnintendo.c

HEADERS += nintendo.h \
    ../../lowlevelstuff/src/compressions/stdnintendo.h

DISTFILES += nintendo.json



