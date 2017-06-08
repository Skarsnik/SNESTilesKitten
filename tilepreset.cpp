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
    pcPaletteLocation = 0;
    SNESPaletteLocation = 0;
    paletteNoZeroColor = false;
}

bool TilePreset::save(const QString &file)
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
    pFile.setValue("tiles/pattern", tilesPattern.name);

    pFile.setValue("palette/pc_location", QString::number(pcPaletteLocation, 16));
    pFile.setValue("palette/snes_location", QString::number(SNESPaletteLocation, 16));
    pFile.setValue("palette/nozerocolor", paletteNoZeroColor);

    return true;
}

bool TilePreset::load(const QString& file)
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
    compression = pFile.value("tiles/compression").toString();
    QString patternName = pFile.value("tiles/pattern").toString();
    if (patternName.isEmpty())
        patternName = "normal";
    tilesPattern = TilesPattern::pattern(patternName);

    pcPaletteLocation = pFile.value("palette/pc_location").toString().toUInt(&ok, 16);
    SNESPaletteLocation = pFile.value("palette/snes_location").toString().toUInt(&ok, 16);
    paletteNoZeroColor = pFile.value("palette/nozerocolor").toBool();


    return true;
}
