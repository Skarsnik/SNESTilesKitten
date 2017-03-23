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
    bool    header;

    unsigned int tilesLocation;
    unsigned int pcTilesLocation;
    unsigned int paletteLocation;
    unsigned int pcPaletteLocation;
    unsigned int size;

    unsigned int bpp;
    QString compression;
};

#endif // TILEPRESET_H
