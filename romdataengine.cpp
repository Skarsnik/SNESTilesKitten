#include "romdataengine.h"
#include "lowlevelstuff/src/rommapping.h"
#include "lowlevelstuff/src/palette.h"

#include <QPluginLoader>
#include <QDebug>
#include <QtMath>

ROMDataEngine::ROMDataEngine()
{
    overrideHeaderInfo = false;
}

bool ROMDataEngine::loadCompressionPlugins(QDir pluginsDir)
{
    foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
        if (!fileName.contains(QRegExp(".*\\.(dll|so)$")))
           continue;
        QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(fileName));
        QObject *plugin = pluginLoader.instance();
        if (plugin) {
            CompressionInterface*   ci = qobject_cast<CompressionInterface *>(plugin);
            if (ci) {
                qDebug() << "Loaded " << fileName;
                foreach(CompressionInfo info, ci->compressionList()) {
                    qDebug() << "  " << info.name << " : " << info.shortDescription;
                    m_availableCompressions[info.name] = ci;
                }
            }
            else
            {
                qCritical() << "Loaded pluging(" << fileName << ")is not a compression plugin";
                return false;
            }
        }
        else {
            qCritical() << "Can't load " << fileName;
            return false;
        }
    }
    return true;
}

unsigned int getRomPosition(TilePreset &preset, unsigned int directAddr, unsigned int snesAddr, bool romHasHeader)
{
    unsigned int filePos = -1;
    enum rom_type rType = HiROM;
    if (preset.romType == "LoROM")
        rType = LoROM;
    if (directAddr == 0) {
        filePos = rommapping_snes_to_pc(snesAddr, rType, romHasHeader);
    } else {
        qDebug() << "direct file location";
        filePos = directAddr;
        if (romHasHeader)
            filePos += 0x200;
    }
    return filePos;
}

QList<tile8> ROMDataEngine::extractTiles(TilePreset &preset)
{
    QList<tile8>    rawTiles;
    bool    romHasHeader = romInfo.hasHeader;
    if (overrideHeaderInfo)
        romHasHeader = overridenHeaderInfo;
    uint    filePos = 0;
    unsigned int size = preset.length;
    QFile romQFile(romFile);
    romQFile.open(QIODevice::ReadOnly);
    filePos = getRomPosition(preset, preset.pcTilesLocation, preset.SNESTilesLocation, romHasHeader);
    qDebug() << "Location" << QString::number(filePos, 16);
    romQFile.seek(filePos);
    QByteArray qdata = romQFile.read(preset.length);
    char*   data = qdata.data();
    QString compressionSelected = preset.compression;
    if (compressionSelected != "None") {
        qDebug() << "Using " << compressionSelected << " compression";
        data = m_availableCompressions[compressionSelected]->unCompress(compressionSelected, data, 0, &size);
        if (data == NULL)
            return rawTiles;
    }
    qDebug() << "Size : " << size;
    for (unsigned int tilePos = 0; tilePos < size; tilePos += preset.bpp * 8) {
        tile8   newTile = unpack_bpp_tile(data, tilePos, preset.bpp);
        rawTiles.append(newTile);
    }
    return rawTiles;
}

QVector<QRgb> ROMDataEngine::extractPalette(TilePreset &preset)
{
    QVector<QRgb> mPalette;
    QFile plop(romFile);
    bool    romHasHeader = romInfo.hasHeader;
    if (overrideHeaderInfo)
        romHasHeader = overridenHeaderInfo;
    unsigned int filePos = getRomPosition(preset, preset.pcPaletteLocation, preset.SNESPaletteLocation, romHasHeader);
    plop.open(QIODevice::ReadOnly);
    plop.seek(filePos);
    unsigned int palette_size = qPow(2, preset.bpp);// - 1;
    QByteArray ab = plop.read(palette_size * 2);
    qDebug() << ab;
    char* data = ab.data();
    if (preset.paletteNoZeroColor)
    {
        mPalette.append(qRgb(0x99, 0x99, 0x99));
        palette_size--;
    }
    r_palette* raw_pal = extract_palette(data, 0, palette_size);
    for (unsigned int i = 0; i < palette_size; i++)
    {
        m_color col = raw_pal->colors[i];
        //qDebug() << QString::number(qRgb(col.red, col.green, col.blue), 16);
        mPalette.append(qRgb(col.red, col.green, col.blue));
    }
    return mPalette;
}

QString ROMDataEngine::getRomFile() const
{
    return romFile;
}

void ROMDataEngine::setRomFile(const QString &value)
{
    romFile = value;
    ROMInfo romI(romFile);
    romInfo = romI;
}

QMap<QString, CompressionInterface *> ROMDataEngine::availableCompressions() const
{
    return m_availableCompressions;
}
