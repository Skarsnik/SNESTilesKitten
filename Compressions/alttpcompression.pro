#-------------------------------------------------
#
# Project created by QtCreator 2017-03-20T13:38:16
#
#-------------------------------------------------

QT       += core gui

TARGET = alttpcompression
TEMPLATE = lib
CONFIG += plugin

DESTDIR = ../plugins

SOURCES += alttpcompression.cpp \
    alttpstddecompress.c

HEADERS += alttpcompression.h \
    alttpstddecompress.h

DISTFILES += alttpcompression.json



