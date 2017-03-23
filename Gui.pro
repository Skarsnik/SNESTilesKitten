#-------------------------------------------------
#
# Project created by QtCreator 2017-03-02T13:33:21
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SNESTilesKitten
TEMPLATE = app


SOURCES += main.cpp\
        mainui.cpp \
    tilepreset.cpp \
    c_stuff/tile.c \
    c_stuff/mapping_hirom.c \
    c_stuff/mapping_lorom.c \
    c_stuff/rommapping.c \
    c_stuff/palette.c \
    graphicspalettecoloritem.cpp

HEADERS  += mainui.h \
    tilepreset.h \
    c_stuff/tile.h \
    c_stuff/rommapping.h \
    compressioninterface.h \
    compressioninfo.h \
    c_stuff/palette.h \
    graphicspalettecoloritem.h

FORMS    += mainui.ui
