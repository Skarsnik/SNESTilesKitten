#ifndef ROMDATAENGINE_H
#define ROMDATAENGINE_H

#include "tilepreset.h"
#include "lowlevelstuff/src/tile.h"
#include "rominfo.h"
#include "compressioninterface.h"

#include <QDir>
#include <QRgb>

class ROMDataEngine
{
public:
    ROMDataEngine();
    static bool            loadCompressionPlugins(QDir pluginsDir);
    QList<tile8>    extractTiles(TilePreset& preset);
    QVector<QRgb>   extractPalette(TilePreset& preset);

    unsigned int    injectTiles(const QList<tile8>& rawTiles, const TilePreset &preset);
    bool            injectPalette(const QVector<QRgb>& mPalette, const TilePreset& preset);

    ROMInfo romInfo;
    bool    overrideHeaderInfo;
    bool    overridenHeaderInfo;
    unsigned int    lastUnCompressSize;
    unsigned int    lastCompressedSize;
    unsigned int    lastCompressSize;

    QString getRomFile() const;
    void setRomFile(const QString &value);

    static QMap<QString, CompressionInterface *> availableCompressions();
    static QMap<QString, CompressionInfo>        compressionInfos();

private:
    unsigned int getRomPosition(const TilePreset &preset, unsigned int directAddr, unsigned int snesAddr);
    QString romFile;

    static bool                                 loadedCompressionPlugins;
    static QMap<QString, CompressionInterface*> m_availableCompressions;
    static QMap<QString, CompressionInfo>       m_compressionsInfo;

};

#endif // ROMDATAENGINE_H
