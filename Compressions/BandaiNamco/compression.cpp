#include "compression.h"
#include "Comp.h"

BandaiNamco::BandaiNamco()
{

}

QList<CompressionInfo> BandaiNamco::compressionList()
{
    QList<CompressionInfo> toret;
    CompressionInfo bandainamco;
    bandainamco.name = "BandaiNamco";
    bandainamco.canCompress = true;
    bandainamco.shortDescription = "BandaiNamco compression, used by lot of Bandai / Namco games";
    bandainamco.description = "BandaiNamco compression, used by lot of Bandai / Namco games ";
    toret << bandainamco;
    return toret;
}

QString BandaiNamco::compressionError()
{
    return QString();
}

QString BandaiNamco::decompressionError()
{
    return QString();
}

char *BandaiNamco::compress(QString name, const char *data, const size_t start, const unsigned int lenght, unsigned int* compressedSize)
{
    // if (name == "Konami1") {
    //     char* toret = konami1_compress(data + start, lenght, compressedSize);
    //     return toret;
    // }
    return nullptr;
}

char *BandaiNamco::unCompress(QString name, const char* data, const size_t start, const unsigned int lenght, unsigned int* uncompressedSize, unsigned int* compressedLength)
{
    if (name == "BandaiNamco") {
        Comp    bnComp;
        bnComp.Init((u8*) data);
        unsigned int size = lenght;
        char* toret = (char*) bnComp.UncompressData(start, &size);
        *uncompressedSize = size;
        return toret;
    }
    return nullptr;
}
