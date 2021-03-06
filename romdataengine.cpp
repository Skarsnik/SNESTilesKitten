#include "romdataengine.h"
#include "lowlevelstuff/src/rommapping.h"
#include "lowlevelstuff/src/palette.h"

#include <QPluginLoader>
#include <QDebug>
#include <QtMath>

bool ROMDataEngine::loadedCompressionPlugins = false;
QMap<QString, CompressionInterface*> ROMDataEngine::m_availableCompressions;
QMap<QString, CompressionInfo>       ROMDataEngine::m_compressionsInfo;

ROMDataEngine::ROMDataEngine()
{
    overrideHeaderInfo = false;
    lastUnCompressSize = 0;
    lastCompressedSize = 0;
    lastCompressSize = 0;
}

bool ROMDataEngine::loadCompressionPlugins(QDir pluginsDir)
{
    if (loadedCompressionPlugins == true)
    {
        qCritical() << "Compressions Plugings already loaded";
        return false;
    }
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
                    m_compressionsInfo[info.name] = info;
                }
            }
            else
            {
                qCritical() << "Loaded pluging(" << fileName << ") is not a compression plugin";
                return false;
            }
        }
        else {
            qCritical() << "Can't load " << fileName;
            return false;
        }
    }
    loadedCompressionPlugins = true;
    return true;
}


QList<tile8> ROMDataEngine::extractTiles(TilePreset &preset)
{
    QList<tile8>    rawTiles;
    uint    filePos = 0;
    unsigned int size = preset.length;
    QFile romQFile(romFile);
    romQFile.open(QIODevice::ReadOnly);
    filePos = getRomPosition(preset, preset.pcTilesLocation, preset.SNESTilesLocation);
    qDebug() << "Location" << QString::number(filePos, 16);
    romQFile.seek(filePos);
    unsigned int readLength = 5000;
    if (preset.length > readLength || romQFile.size() < readLength)
        readLength = preset.length;
    QByteArray qdata = romQFile.read(preset.length);
    char*   data = qdata.data();
    QString compressionSelected = preset.compression;
    lastUnCompressSize = 0;
    if (compressionSelected != "None") {
        qDebug() << "Using " << compressionSelected << " compression";
        data = m_availableCompressions[compressionSelected]->unCompress(compressionSelected, data, 0, preset.length, &size, &lastCompressedSize);
        if (data == NULL)
        {
            lastDecompressionError = m_availableCompressions[compressionSelected]->decompressionError();
            return rawTiles;
        }
        lastDecompressionError = QString();
        lastUnCompressSize = size;
    } else {
        lastDecompressionError = QString();
    }
    qDebug() << "Size : " << size;
    unsigned tileCpt = 0;
    for (unsigned int tilePos = 0; tilePos < size; tilePos += preset.bpp * 8)
    {
        tile8   newTile = unpack_bpp_tile(data, tilePos, preset.bpp);
        newTile.id = tileCpt;
        rawTiles.append(newTile);
        tileCpt++;
    }
    if (compressionSelected != "None")
        free(data);
    return rawTiles;
}

SNESPalette ROMDataEngine::extractPalette(TilePreset &preset)
{
    QFile plop(romFile);
    unsigned int filePos = getRomPosition(preset, preset.pcPaletteLocation, preset.SNESPaletteLocation);
    qDebug() << "Palette pos : " << QString::number(filePos, 16);
    plop.open(QIODevice::ReadOnly);
    plop.seek(filePos);
    unsigned int palette_size = qPow(2, preset.bpp);// - 1;
    QByteArray ab = plop.read(palette_size * 2);
    qDebug() << ab;
    const char* data = ab.constData();
    SNESPalette pal(ab);
    if (preset.paletteNoZeroColor)
    {
        SNESColor col;

        col.setRgb(qRgb(0x99, 0x99, 0x99));
        pal.colors.prepend(col);
        pal.colors.removeAt(pal.colors.size() - 1);
    }
    return pal;
}

unsigned int ROMDataEngine::injectTiles(const QList<tile8> &rawTiles, const TilePreset& preset)
{
    QFile file(romFile);
    qDebug() << "File size is : " << file.size();
    unsigned int filePos = getRomPosition(preset, preset.pcTilesLocation, preset.SNESTilesLocation);
    if (!file.open(QIODevice::WriteOnly | QIODevice::ReadOnly))
    {
        qCritical() << "Can't write on romFile" << file.error();
        return false;
    }
    file.seek(filePos);

    unsigned int tileChunkSize = preset.bpp * 8;
    unsigned int tileStringLenght = tileChunkSize * rawTiles.size();
    char* tilesString = (char*) malloc(tileStringLenght);

    unsigned int tileStringPos = 0;
    unsigned int piko;
    foreach(tile8 tile, rawTiles)
    {
        char* tileS = pack_bpp_tile(tile, preset.bpp, &piko);
        memcpy(tilesString + tileStringPos, tileS, piko);
        free(tileS);
        tileStringPos += piko;
    }
    unsigned int writeLenght = 0;
    QString compression = preset.compression;
    if (compression == "None")
    {
        file.write(tilesString, tileStringLenght);
        qDebug() << tileStringLenght << " bytes of data writen to ROM";
        writeLenght = tileStringLenght;
        lastCompressionError = QString();
    } else
    {
        qDebug() << "Using " << compression << " compression";
        if (!m_compressionsInfo[compression].canCompress)
        {
            qCritical() << "Compression : " << compression << m_compressionsInfo[compression].shortDescription << "does not support compression";
            free(tilesString);
            return 0;
        }
        unsigned int compressedSize;
        char* compressedTiles = m_availableCompressions[compression]->compress(compression, tilesString, 0, tileStringLenght, &compressedSize);
        if (compressedTiles == NULL)
        {
            qCritical() << "Error with compression";
            lastCompressionError = m_availableCompressions[compression]->compressionError();
            free(tilesString);
            return 0;
        } else {
            lastCompressionError = QString();
        }
        lastCompressSize = compressedSize;
        file.write(compressedTiles, compressedSize);
        free(compressedTiles);
        writeLenght = compressedSize;
    }
    file.close();
    free(tilesString);
    return writeLenght;
}

bool    ROMDataEngine::injectPalette(SNESPalette pal, const TilePreset& preset)
{
    QFile file(romFile);
    unsigned int filePos = getRomPosition(preset, preset.pcPaletteLocation, preset.SNESPaletteLocation);
    if (!file.open(QIODevice::WriteOnly | QIODevice::ReadOnly))
    {
        qCritical() << "Can't write on romFile" << file.error();
        return false;
    }
    file.seek(filePos);
    if (preset.paletteNoZeroColor)
        pal.colors.removeAt(0);
    file.write(pal.encode());
    return true;
}

QString ROMDataEngine::getRomFile() const
{
    return romFile;
}

void ROMDataEngine::setRomFile(const QString &value)
{
    qDebug() << "Loading rom " << value;
    romFile = value;
    ROMInfo romI(romFile);
    qDebug() << romI.size;
    romInfo = romI;
}

QMap<QString, CompressionInterface *> ROMDataEngine::availableCompressions()
{
    return m_availableCompressions;
}

QMap<QString, CompressionInfo> ROMDataEngine::compressionInfos()
{
    return m_compressionsInfo;
}

unsigned int ROMDataEngine::pcToSnes(unsigned int pcAddr, QString romType)
{
    enum rom_type rType = HiROM;
    if (romType == "LoROM")
        rType = LoROM;
    return rommapping_pc_to_snes(pcAddr, rType, false);
}

unsigned int ROMDataEngine::snesToPC(unsigned int romAddr, QString romType)
{
    enum rom_type rType = HiROM;
    if (romType == "LoROM")
        rType = LoROM;
    return rommapping_snes_to_pc(romAddr, rType, false);
}

unsigned int ROMDataEngine::getRomPosition(const TilePreset &preset, int directAddr, unsigned int snesAddr)
{
    bool    romHasHeader = romInfo.hasHeader;
    if (overrideHeaderInfo)
        romHasHeader = overridenHeaderInfo;
    unsigned int filePos = -1;
    enum rom_type rType = HiROM;
    if (preset.romType == "LoROM")
        rType = LoROM;
    qDebug() << directAddr;
    if (directAddr == -1) {
        filePos = rommapping_snes_to_pc(snesAddr, rType, romHasHeader);
    } else {
        filePos = directAddr;
        if (romHasHeader)
            filePos += 0x200;
    }
    return filePos;
}

QDebug &operator<<(QDebug debug, const tile8 &tile)
{
    QDebugStateSaver saver(debug);
    debug.nospace() << "{T:" << tile.id << "}";
    return debug;
}
