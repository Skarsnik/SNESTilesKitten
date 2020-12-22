
QT       += core gui

TARGET = quintetcompression
TEMPLATE = lib
CONFIG += plugin

DESTDIR = ../../plugins

SOURCES += Global.cpp \
           Comp.cpp \
           ../../lowlevelstuff/src/compressions/quintet1/quintet1_compression.c \
           ../../lowlevelstuff/src/compressions/quintet1/bitstream/src/bitstream.c \
           quintetcompression.cpp

HEADERS += Global.h \
           Comp.h \
           ../../lowlevelstuff/src/compressions/quintet1/quintet1_compression.h \
           quintetcompression.h \
           ../../lowlevelstuff/src/compressions/quintet1/bitstream/include/bitstream.h

INCLUDEPATH += ../../lowlevelstuff/src/compressions/quintet1/ \
               ../../lowlevelstuff/src/compressions/quintet1/bitstream/include/

DISTFILES += quintet.json
