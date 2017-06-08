#include "terranigmacompression.h"
#include "Terranigma/Comp.h"

TerranigmaCompression::TerranigmaCompression()
{

}

QList<CompressionInfo> TerranigmaCompression::compressionList()
{
    QList<CompressionInfo> toret;
    CompressionInfo normal;
    normal.name = "Terranigma";
    normal.canCompress = false;
    normal.shortDescription = "Standard Terranigma compression";
    normal.description = "Standard compression used by Terranigma";
    toret << normal;

    return toret;
}

QString TerranigmaCompression::compressionError()
{
    return QString();
}

QString TerranigmaCompression::decompressionError()
{
    return QString();
}

char *TerranigmaCompression::compress(QString name, const char *data, const size_t start, const unsigned int lenght, unsigned int* compressedSize)
{
    return NULL;
}

char *TerranigmaCompression::unCompress(QString name, const char* data, const size_t start, const unsigned int lenght, unsigned int* uncompressedSize, unsigned int* compressedLength)
{
    if (name == "Terranigma") {
        Comp    terraComp;
        terraComp.Init((u8*) data);
        unsigned int size = lenght;
        char* toret = (char*) terraComp.UncompressData(start, &size);
        *uncompressedSize = size;
        return toret;
    }
    return NULL;
}
