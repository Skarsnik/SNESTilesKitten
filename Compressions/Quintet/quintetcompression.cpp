#include "quintetcompression.h"
#include "quintet1_compression.h"
#include "Comp.h"

QuintetCompression::QuintetCompression()
{

}

QList<CompressionInfo> QuintetCompression::compressionList()
{
    QList<CompressionInfo> toret;
    CompressionInfo normal;
    normal.name = "Terranigma";
    normal.canCompress = false;
    normal.shortDescription = "Standard Terranigma compression";
    normal.description = "Standard compression used by Terranigma";
    toret << normal;
    CompressionInfo quintet1;
    quintet1.name = "Quintet1";
    quintet1.canCompress = true;
    quintet1.shortDescription = "Quintet1 compression, used by lot of Quintet games";
    quintet1.description = "Quintet1 compression, used by lot of Quintet games: Actraiser 1 & 2, Robotrek, Soul Blazer, Illusion of Gaia ";
    toret << quintet1;
    return toret;
}

QString QuintetCompression::compressionError()
{
    return QString();
}

QString QuintetCompression::decompressionError()
{
    return QString();
}

char *QuintetCompression::compress(QString name, const char *data, const size_t start, const unsigned int lenght, unsigned int* compressedSize)
{
    if (name == "Quintet1") {
        char* toret = quintet1_compress(data + start, lenght, compressedSize);
        return toret;
    }
    return nullptr;
}

char *QuintetCompression::unCompress(QString name, const char* data, const size_t start, const unsigned int lenght, unsigned int* uncompressedSize, unsigned int* compressedLength)
{
    if (name == "Terranigma") {
        Comp    terraComp;
        terraComp.Init((u8*) data);
        unsigned int size = lenght;
        char* toret = (char*) terraComp.UncompressData(start, &size);
        *uncompressedSize = size;
        return toret;
    }
    if (name == "Quintet1") {
        char* toret = quintet1_decompress(data, uncompressedSize);
        return toret;
    }
    return nullptr;
}
