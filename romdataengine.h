#ifndef ROMDATAENGINE_H
#define ROMDATAENGINE_H

#include "tilepreset.h"
#include "lowlevelstuff/src/tile.h"
#include "myrominfo.h"
#include "compressioninterface.h"
#include "snespalette.h"

#include <QDir>
#include <QRgb>

class ROMDataEngine
{
public:
    ROMDataEngine();
    static bool            loadCompressionPlugins(QDir pluginsDir);
    QList<tile8>    extractTiles(TilePreset& preset);
    SNESPalette     extractPalette(TilePreset& preset);

    unsigned int    injectTiles(const QList<tile8>& rawTiles, const TilePreset &preset);
    bool            injectPalette(SNESPalette pal, const TilePreset& preset);

    ROMInfo         romInfo;
    bool            overrideHeaderInfo;
    bool            overridenHeaderInfo;
    unsigned int    lastUnCompressSize;
    unsigned int    lastCompressedSize;
    unsigned int    lastCompressSize;
    QString         lastCompressionError;
    QString         lastDecompressionError;

    QString getRomFile() const;
    void setRomFile(const QString &value);

    static QMap<QString, CompressionInterface *>    availableCompressions();
    static QMap<QString, CompressionInfo>           compressionInfos();
    static unsigned int                             pcToSnes(unsigned int pcAddr, QString romType);
    static unsigned int                             snesToPC(unsigned int romAddr, QString romType);

private:
    unsigned int getRomPosition(const TilePreset &preset, int directAddr, unsigned int snesAddr);
    QString romFile;

    static bool                                 loadedCompressionPlugins;
    static QMap<QString, CompressionInterface*> m_availableCompressions;
    static QMap<QString, CompressionInfo>       m_compressionsInfo;

};

QDebug &operator<<(QDebug debug, const tile8 &tile);

#endif // ROMDATAENGINE_H
