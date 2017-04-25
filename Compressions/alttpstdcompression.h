#ifndef ALTTPCOMPRESSION_H
#define ALTTPCOMPRESSION_H

#include <QObject>
#include <../compressioninterface.h>

class AlttpCompression : public QObject, public CompressionInterface
{
    Q_OBJECT
    Q_INTERFACES(CompressionInterface)
    Q_PLUGIN_METADATA(IID "fr.nyo.SNESTilesKitten.AlttpCompression" FILE "alttpcompression.json")

public:
    AlttpCompression();
    QList<CompressionInfo>  compressionList();
    char*   compress(QString name, const char *data, unsigned int start, unsigned int lenght, unsigned int *compressedSize);
    char*   unCompress(QString name, const char *data, unsigned int start, unsigned int *uncompressedSize);

};

#endif // ALTTPCOMPRESSION_H
