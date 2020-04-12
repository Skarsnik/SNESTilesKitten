
QT       += core gui

TARGET = quintetcompression
TEMPLATE = lib
CONFIG += plugin

DESTDIR = ../../plugins

SOURCES += Global.cpp \
           Comp.cpp \
           ../../lowlevelstuff/src/compressions/quintet1/quintet1_compression.c \
           bitstream.c \
           quintetcompression.cpp

HEADERS += Global.h \
           Comp.h \
           ../../lowlevelstuff/src/compressions/quintet1/quintet1_compression.h \
           quintetcompression.h \
           bitstream.h

INCLUDEPATH += ../../lowlevelstuff/src/compressions/quintet1/

DISTFILES += quintet.json
