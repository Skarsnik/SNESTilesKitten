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

char *TerranigmaCompression::compress(QString name, const char *data, unsigned int start, unsigned int lenght, unsigned int* compressedSize)
{
    return NULL;
}

char *TerranigmaCompression::unCompress(QString name, const char* data, const unsigned int start, unsigned int* uncompressedSize, unsigned int* compressedLength)
{
    if (name == "Terranigma") {
        Comp    terraComp;
        terraComp.Init((u8*) data);
        return (char*) terraComp.UncompressData(start, uncompressedSize);
    }
    return NULL;
}