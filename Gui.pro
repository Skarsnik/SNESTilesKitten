#-------------------------------------------------
#
# Project created by QtCreator 2017-03-02T13:33:21
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SNESTilesKitten
TEMPLATE = app
QMAKE_CFLAGS += "-std=c99"

SOURCES += main.cpp\
        mainui.cpp \
    tilepreset.cpp \
    lowlevelstuff/src/tile.c \
    lowlevelstuff/src/mapping_hirom.c \
    lowlevelstuff/src/mapping_lorom.c \
    lowlevelstuff/src/rommapping.c \
    lowlevelstuff/src/palette.c \
    graphicspalettecoloritem.cpp \
    rominfo.cpp \
    presetsavedialog.cpp \
    pngexportimport.cpp \
    romdataengine.cpp \
    graphicstileitem.cpp \
    injectdialog.cpp \
    graphicstilesscene.cpp

HEADERS  += mainui.h \
    tilepreset.h \
    lowlevelstuff/src/tile.h \
    lowlevelstuff/src/rommapping.h \
    compressioninterface.h \
    compressioninfo.h \
    lowlevelstuff/src/palette.h \
    graphicspalettecoloritem.h \
    rominfo.h \
    presetsavedialog.h \
    pngexportimport.h \
    romdataengine.h \
    graphicstileitem.h \
    injectdialog.h \
    graphicstilesscene.h

FORMS    += mainui.ui \
    presetsavedialog.ui \
    injectdialog.ui
