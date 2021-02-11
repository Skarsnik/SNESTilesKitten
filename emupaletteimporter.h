#ifndef EMUPALETTEIMPORTER_H
#define EMUPALETTEIMPORTER_H

#include <QString>
#include <QVector>
#include <QVersionNumber>
#include "snespalette.h"

class EmuPaletteImporter
{
public:
    struct EmulatorInfos
    {
        QString         name;
        QString         identifier;
        QVersionNumber  version;
    };
    EmuPaletteImporter();
    QList<EmulatorInfos>    getAvailableEmu();
    QVector<SNESPalette>    getPalettes(QString identifier);
};

#endif // EMUPALETTEIMPORTER_H
