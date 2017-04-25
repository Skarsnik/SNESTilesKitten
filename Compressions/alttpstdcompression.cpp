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
    normal.canCompress = false;
    normal.shortDescription = "Standard zelda3 compression";
    normal.description = "Standard compression used by Zelda3, called lz2 in lunar compress, more info could be found on the alttp hacking wiki";
    toret << normal;

    return toret;
}

char *AlttpCompression::compress(QString name, const char *data, unsigned int start, unsigned int lenght, unsigned int* compressedSize)
{
    if (name == "zelda3")
    {
        return ::compress(data, start, lenght, compressedSize);
    }
    return NULL;
}

char *AlttpCompression::unCompress(QString name, const char *data, unsigned int start, unsigned int *uncompressedSize)
{
    char *toret = NULL;
    qDebug() << "Uncompress";
    if (name == "zelda3") {
        toret = decompress(data, start, uncompressedSize);
    }
    qDebug() << "End decompress";
    return toret;
}
