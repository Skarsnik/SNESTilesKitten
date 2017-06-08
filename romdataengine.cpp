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
    QByteArray qdata = romQFile.read(preset.length);
    char*   data = qdata.data();
    QString compressionSelected = preset.compression;
    if (compressionSelected != "None") {
        qDebug() << "Using " << compressionSelected << " compression";
        data = m_availableCompressions[compressionSelected]->unCompress(compressionSelected, data, 0, &size, &lastCompressedSize);
        if (data == NULL)
            return rawTiles;
        lastUnCompressSize = size;
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

QVector<QRgb> ROMDataEngine::extractPalette(TilePreset &preset)
{
    QVector<QRgb> mPalette;
    QFile plop(romFile);
    unsigned int filePos = getRomPosition(preset, preset.pcPaletteLocation, preset.SNESPaletteLocation);
    plop.open(QIODevice::ReadOnly);
    plop.seek(filePos);
    unsigned int palette_size = qPow(2, preset.bpp);// - 1;
    QByteArray ab = plop.read(palette_size * 2);
    char* data = ab.data();
    if (preset.paletteNoZeroColor)
    {
        mPalette.append(qRgb(0x99, 0x99, 0x99));
        palette_size--;
    }
    r_palette* raw_pal = palette_extract(data, 0, palette_size);
    for (unsigned int i = 0; i < palette_size; i++)
    {
        m_color col = raw_pal->colors[i];
        //qDebug() << QString::number(qRgb(col.red, col.green, col.blue), 16);
        mPalette.append(qRgb(col.red, col.green, col.blue));
    }
    palette_free(raw_pal);
    return mPalette;
}

unsigned int ROMDataEngine::injectTiles(const QList<tile8> &rawTiles, const TilePreset& preset)
{
    QFile file(romFile);
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
            free(tilesString);
            return 0;
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

bool    ROMDataEngine::injectPalette(const QVector<QRgb>& mPalette, const TilePreset& preset)
{
    QFile file(romFile);
    unsigned int filePos = getRomPosition(preset, preset.pcPaletteLocation, preset.SNESPaletteLocation);
    if (!file.open(QIODevice::WriteOnly | QIODevice::ReadOnly))
    {
        qCritical() << "Can't write on romFile" << file.error();
        return false;
    }
    file.seek(filePos);
    unsigned int palette_size = mPalette.size();
    if (preset.paletteNoZeroColor)
        palette_size--;
    r_palette* pal = palette_create(palette_size, 0);
    unsigned int pos = 0;
    bool first = true;
    foreach (QRgb color, mPalette) {
        if (first && preset.paletteNoZeroColor)
        {
            first = false;
            continue;
        }
        pal->colors[pos].blue = qBlue(color);
        pal->colors[pos].green = qGreen(color);
        pal->colors[pos].red = qRed(color);
        pos++;
    }
    char* snes_string = palette_convert(*pal);
    file.write(snes_string, pal->size * 2);
    palette_free(pal);
    file.close();
    return true;
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

QMap<QString, CompressionInterface *> ROMDataEngine::availableCompressions()
{
    return m_availableCompressions;
}

QMap<QString, CompressionInfo> ROMDataEngine::compressionInfos()
{
    return m_compressionsInfo;
}

unsigned int ROMDataEngine::getRomPosition(const TilePreset &preset, unsigned int directAddr, unsigned int snesAddr)
{
    bool    romHasHeader = romInfo.hasHeader;
    if (overrideHeaderInfo)
        romHasHeader = overridenHeaderInfo;
    unsigned int filePos = -1;
    enum rom_type rType = HiROM;
    if (preset.romType == "LoROM")
        rType = LoROM;
    if (directAddr == 0) {
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
