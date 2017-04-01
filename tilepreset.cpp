#include "tilepreset.h"

#include <QFile>
#include <QSettings>

TilePreset::TilePreset()
{
    name = "";
    romName = "";
    romType = "";

    pcTilesLocation = 0;
    SNESTilesLocation = 0;
    length = 0;
    bpp = 0;
    compression = "None";
    tilesPerRow = 0;
    pcPaletteLocation = 0;
    SNESPaletteLocation = 0;
}

bool TilePreset::save(QString file)
{
    QSettings pFile(file, QSettings::IniFormat);

    if (pFile.isWritable() == false)
        return false;

    pFile.setValue("_/name", name);
    pFile.setValue("rom/name", romName);
    pFile.setValue("rom/type", romType);

    pFile.setValue("tiles/pc_location", QString::number(pcTilesLocation, 16));
    pFile.setValue("tiles/snes_location", QString::number(SNESTilesLocation, 16));
    pFile.setValue("tiles/length", length);
    pFile.setValue("tiles/bpp", bpp);
    pFile.setValue("tiles/compression", compression);

    pFile.setValue("tiles_arrangement/tiles_per_row", tilesPerRow);

    pFile.setValue("palette/pc_location", QString::number(pcPaletteLocation, 16));
    pFile.setValue("palette/snes_location", QString::number(SNESPaletteLocation, 16));

    return true;
}

bool TilePreset::load(QString file)
{
    QSettings   pFile(file, QSettings::IniFormat);

    /* Meh solution to know if the file is right*/
    if (pFile.value("_/name").toString().isEmpty())
        return false;
    name = pFile.value("_/name").toString();
    romName = pFile.value("rom/name").toString();
    romType = pFile.value("rom/type").toString();

    /* Locations are stored in a hex string */
    bool ok;
    pcTilesLocation = pFile.value("tiles/pc_location").toString().toUInt(&ok, 16);
    SNESTilesLocation = pFile.value("tiles/snes_location").toString().toUInt(&ok, 16);
    length = pFile.value("tiles/length").toInt();
    bpp = pFile.value("tiles/bpp").toInt();
    compression = pFile.value("tiles/bpp").toString();

    tilesPerRow = pFile.value("tiles_arrrangement/tiles_per_row").toInt();

    pcPaletteLocation = pFile.value("palette/pc_location").toString().toUInt(&ok, 16);
    SNESPaletteLocation = pFile.value("palette/snes_location").toString().toUInt(&ok, 16);

    return true;
}
