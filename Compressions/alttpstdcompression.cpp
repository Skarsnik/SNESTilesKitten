#include "alttpstdcompression.h"
#include "../lowlevelstuff/src/compressions/alttpcompression.h"
#include <QDebug>


AlttpCompression::AlttpCompression()
{

}

QList<CompressionInfo> AlttpCompression::compressionList()
{
    QList<CompressionInfo> toret;
    CompressionInfo normal;
    normal.name = "zelda3";
    normal.canCompress = true;
    normal.shortDescription = "Standard zelda3 compression";
    normal.description = "Standard compression used by Zelda3, called lz2 in lunar compress, more info could be found on the alttp hacking wiki";
    toret << normal;

    return toret;
}

char *AlttpCompression::compress(QString name, const char *data, const unsigned int start, unsigned int lenght, unsigned int* compressedSize)
{
    if (name == "zelda3")
    {
        return alttp_compress(data, start, lenght, compressedSize);
    }
    return NULL;
}

char *AlttpCompression::unCompress(QString name, const char *data, const unsigned int start, unsigned int *uncompressedSize, unsigned int *compressedLength)
{
    char *toret = NULL;
    if (name == "zelda3") {
        qDebug() << QString::number((unsigned int) data, 16) << start;
        toret = alttp_decompress(data, start, uncompressedSize, compressedLength);
        if (toret != NULL)
        {
            qDebug() << *uncompressedSize;
            qDebug() << "Checksum : " << qChecksum(toret, *uncompressedSize);
        }
    }
    qDebug() << "End decompress";
    return toret;
}
