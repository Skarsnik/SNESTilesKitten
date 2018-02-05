
QT       += core gui

TARGET = terranigmacompression
TEMPLATE = lib
CONFIG += plugin

DESTDIR = ../../plugins

SOURCES += Global.cpp \
           Comp.cpp \
    terranigmacompression.cpp

HEADERS += Global.h \
           Comp.h \
    terranigmacompression.h

DISTFILES += terranigma.json
