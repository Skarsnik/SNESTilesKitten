#ifndef TILEPRESET_H
#define TILEPRESET_H

#include <QString>

class TilePreset
{
public:
    TilePreset();

    bool    save(QString file);
    bool    load(QString file);

    QString name;
    QString romName;
    QString romType;

    unsigned int SNESTilesLocation;
    unsigned int pcTilesLocation;
    unsigned int SNESPaletteLocation;
    unsigned int pcPaletteLocation;
    unsigned int length;
    unsigned int tilesPerRow;

    unsigned int bpp;
    QString compression;
};

#endif // TILEPRESET_H
