
QT       += core gui

TARGET = terranigmacompression
TEMPLATE = lib
CONFIG += plugin

DESTDIR = ../plugins

SOURCES += Terranigma/Global.cpp \
           Terranigma/Comp.cpp \
    terranigmacompression.cpp

HEADERS += Terranigma/Global.h \
           Terranigma/Comp.h \
    terranigmacompression.h

DISTFILES += terranigmacompression.json
