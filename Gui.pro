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

RC_FILE = snestileskitten.rc

SOURCES += main.cpp\
        mainui.cpp \
    tilepreset.cpp \
    lowlevelstuff/src/tile.c \
    lowlevelstuff/src/mapping_hirom.c \
    lowlevelstuff/src/mapping_lorom.c \
    lowlevelstuff/src/rommapping.c \
    lowlevelstuff/src/palette.c \
    lowlevelstuff/src/rominfo.c \
    graphicspalettecoloritem.cpp \
    presetsavedialog.cpp \
    pngexportimport.cpp \
    romdataengine.cpp \
    graphicstileitem.cpp \
    injectdialog.cpp \
    graphicstilesscene.cpp \
    tilespattern.cpp \
    tileinfow.cpp \
    mgraphicsview.cpp \
    myrominfo.cpp \
    paletteeditor.cpp \
    graphicspaletteview.cpp \
    snespalette.cpp \
    emupaletteimporter.cpp

HEADERS  += mainui.h \
    tilepreset.h \
    lowlevelstuff/src/tile.h \
    lowlevelstuff/src/rommapping.h \
    lowlevelstuff/src/rominfo.h \
    compressioninterface.h \
    compressioninfo.h \
    lowlevelstuff/src/palette.h \
    graphicspalettecoloritem.h \
    presetsavedialog.h \
    pngexportimport.h \
    romdataengine.h \
    graphicstileitem.h \
    injectdialog.h \
    tilespattern.h \
    tileinfow.h \
    mgraphicsview.h \
    myrominfo.h \
    paletteeditor.h \
    graphicspaletteview.h \
    snespalette.h \
    emupaletteimporter.h

FORMS    += mainui.ui \
    presetsavedialog.ui \
    injectdialog.ui \
    tileinfow.ui \
    paletteeditor.ui

DISTFILES += \
    snestileskitten.rc
