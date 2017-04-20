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
    bool            loadCompressionPlugins(QDir pluginsDir);
    QList<tile8>    extractTiles(TilePreset& preset);
    QVector<QRgb>   extractPalette(TilePreset& preset);

    ROMInfo romInfo;
    bool    overrideHeaderInfo;
    bool    overridenHeaderInfo;

    QString getRomFile() const;
    void setRomFile(const QString &value);

    QMap<QString, CompressionInterface *> availableCompressions() const;

private:
    QMap<QString, CompressionInterface*> m_availableCompressions;
    QString romFile;
};

#endif // ROMDATAENGINE_H
