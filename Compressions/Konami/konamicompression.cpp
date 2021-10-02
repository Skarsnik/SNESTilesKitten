#include "konamicompression.h"
#include "Comp.h"

KonamiCompression::KonamiCompression()
{

}

QList<CompressionInfo> KonamiCompression::compressionList()
{
    QList<CompressionInfo> toret;
    CompressionInfo konami1;
    konami1.name = "Konami1";
    konami1.canCompress = true;
    konami1.shortDescription = "Konami1 compression, used by lot of Konami games";
    konami1.description = "Konami1 compression, used by lot of Konami games ";
    toret << konami1;
    return toret;
}

QString KonamiCompression::compressionError()
{
    return QString();
}

QString KonamiCompression::decompressionError()
{
    return QString();
}

char *KonamiCompression::compress(QString name, const char *data, const size_t start, const unsigned int lenght, unsigned int* compressedSize)
{
    // if (name == "Konami1") {
    //     char* toret = konami1_compress(data + start, lenght, compressedSize);
    //     return toret;
    // }
    return nullptr;
}

char *KonamiCompression::unCompress(QString name, const char* data, const size_t start, const unsigned int lenght, unsigned int* uncompressedSize, unsigned int* compressedLength)
{
    if (name == "Konami1") {
        Comp    lzkn1Comp;
        lzkn1Comp.Init((u8*) data);
        unsigned int size = lenght;
        char* toret = (char*) lzkn1Comp.UncompressData(start, &size);
        *uncompressedSize = size;
        return toret;
    }
    return nullptr;
}
