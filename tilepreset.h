#ifndef TILEPRESET_H
#define TILEPRESET_H

#include <QString>
#include <QDebug>
#include "tilespattern.h"

class TilePreset
{
public:
    TilePreset();

    bool    save(const QString& file);
    bool    load(const QString& file);

    QString name;
    QString romName;
    QString romType;
    TilesPattern    tilesPattern;

    unsigned int    SNESTilesLocation;
             int    pcTilesLocation;
    unsigned int    SNESPaletteLocation;
    unsigned int    pcPaletteLocation;
    bool            paletteNoZeroColor;
    unsigned int    length;

    unsigned int bpp;
    QString compression;
};

QDebug operator<<(QDebug debug, const TilePreset &pre);

#endif // TILEPRESET_H
