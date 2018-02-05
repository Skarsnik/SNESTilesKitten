#ifndef ALTTPCOMPRESSION_H
#define ALTTPCOMPRESSION_H

#include <QObject>
#include "../../compressioninterface.h"

class NintendoCompression : public QObject, public CompressionInterface
{
    Q_OBJECT
    Q_INTERFACES(CompressionInterface)
    Q_PLUGIN_METADATA(IID "fr.nyo.SNESTilesKitten.NintendoCompression" FILE "nintendo.json")

public:
    NintendoCompression();
    QList<CompressionInfo>  compressionList();
    char*   compress(QString name, const char *data, const size_t start, const unsigned int lenght, unsigned int *compressedSize);
    char*   unCompress(QString name, const char *data, const size_t start, const unsigned int lenght, unsigned int *uncompressedSize, unsigned int* compressedLength);
    QString compressionError();
    QString decompressionError();

private:
    QString m_compressionError;
    QString m_decompressionError;

};

#endif // ALTTPCOMPRESSION_H
