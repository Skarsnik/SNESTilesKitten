#ifndef COMPRESSIONINTERFACE_H
#define COMPRESSIONINTERFACE_H

#include <qplugin.h>
#include <QString>
#include <QMap>

#include "compressioninfo.h"

class CompressionInterface {
  public:
    virtual ~CompressionInterface() {}
    virtual QList<CompressionInfo> compressionList() = 0;
    virtual char* compress(QString name, const char* data, const size_t start, const unsigned int lenght, unsigned int *compressedSize) = 0;
    virtual char* unCompress(QString name, const char* data, const size_t start, const unsigned int lenght, unsigned int* uncompressedSize, unsigned int* compressedLength) = 0;
    virtual QString compressionError() = 0;
    virtual QString decompressionError() = 0;
};

Q_DECLARE_INTERFACE(CompressionInterface, "fr.nyo.SNESTilesKitten.CompressionInterface")

#endif // COMPRESSIONINTERFACE_H
