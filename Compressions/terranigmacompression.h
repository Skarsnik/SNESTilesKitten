#ifndef TERRANIGMACOMPRESSION_H
#define TERRANIGMACOMPRESSION_H


#include <QObject>
#include <../compressioninterface.h>

class TerranigmaCompression : public QObject, public CompressionInterface
{
    Q_OBJECT
    Q_INTERFACES(CompressionInterface)
    Q_PLUGIN_METADATA(IID "fr.nyo.SNESTilesKitten.AlttpCompression" FILE "alttpcompression.json")

public:
    TerranigmaCompression();
    QList<CompressionInfo>  compressionList();
    char*   compress(QString name, const char *data, unsigned int start, unsigned int lenght, unsigned int *compressedSize);
    char*   unCompress(QString name, const char *data, unsigned int start, unsigned int *uncompressedSize);

};
#endif // TERRANIGMACOMPRESSION_H
