
QT       += core gui

TARGET = compression
TEMPLATE = lib
CONFIG += plugin

DESTDIR = ../../plugins

SOURCES += Global.cpp \
           Comp.cpp \
           compression.cpp

HEADERS += Global.h \
           Comp.h \
           compression.h

DISTFILES += konami.json
