#-------------------------------------------------
#
# Project created by QtCreator 2017-03-20T13:38:16
#
#-------------------------------------------------

QT       += core gui

TARGET = alttpcompression
TEMPLATE = lib
CONFIG += plugin

QMAKE_CFLAGS += "-std=c99"

DESTDIR = ../plugins

SOURCES += alttpstdcompression.cpp \
    ../lowlevelstuff/src/compressions/alttpcompression.c

HEADERS += alttpstdcompression.h \
    ../lowlevelstuff/src/compressions/alttpcompression.h

DISTFILES += alttpcompression.json



