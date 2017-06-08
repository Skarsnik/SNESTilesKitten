QT += core

CONFIG += c++11

TARGET = SNESTinyKitten
CONFIG += console
CONFIG -= app_bundle
INCLUDEPATH += ../

TEMPLATE = app

SOURCES += ../romdataengine.cpp \
    ../tilepreset.cpp \
    ../lowlevelstuff/src/tile.c \
    ../lowlevelstuff/src/mapping_hirom.c \
    ../lowlevelstuff/src/mapping_lorom.c \
    ../lowlevelstuff/src/rommapping.c \
    ../lowlevelstuff/src/palette.c \
    ../rominfo.cpp \
    ../pngexportimport.cpp \
    ../tilespattern.cpp \
    tinymain.cpp
