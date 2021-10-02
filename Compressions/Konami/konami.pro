
QT       += core gui

TARGET = konamicompression
TEMPLATE = lib
CONFIG += plugin

DESTDIR = ../../plugins

SOURCES += Global.cpp \
           Comp.cpp \
           konamicompression.cpp

HEADERS += Global.h \
           Comp.h \
           konamicompression.h

DISTFILES += konami.json
