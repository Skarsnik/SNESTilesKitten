#include "nintendo.h"
#include "stdnintendo.h"
#include <QDebug>


NintendoCompression::NintendoCompression()
{

}

QList<CompressionInfo> NintendoCompression::compressionList()
{
    QList<CompressionInfo> toret;
    CompressionInfo nintendo1;
    nintendo1.name = "nintendo1";
    nintendo1.description = "Nintendo compression mode 1, used by Super Mario World and other. Called lz1 in lunar compress.";
    nintendo1.canCompress = true;
    nintendo1.shortDescription = "Nintendo compression 1";
    CompressionInfo nintendo2;
    nintendo2.name = "nintendo2";
    nintendo2.description = "Nintendo compression mode 2, used by Zelda 3 gfx. Called lz2 in lunar compress.";
    nintendo2.canCompress = true;
    nintendo2.shortDescription = "Nintendo compression 2";
    CompressionInfo zelda3;
    zelda3.name = "zelda3";
    zelda3.canCompress = true;
    zelda3.shortDescription = "Standard zelda3 gfx compression";
    zelda3.description = "Standard gfx compression used by Zelda3, Called lz2 in lunar compress, more info could be found on the alttp hacking wiki";
    CompressionInfo smw;
    smw.name = "smw";
    smw.canCompress = true;
    smw.shortDescription = "Standard SMW compression";
    smw.description = "Standard Super Mario World gfx compression. Called lz1 in lunar compress, more info could be found on the smw hacking wiki";
    toret << nintendo1 << zelda3 << nintendo2 << smw;

    return toret;
}

char *NintendoCompression::compress(QString name, const char *data, const size_t start, const unsigned int lenght, unsigned int* compressedSize)
{
    if (name == "zelda3" || name == "nintendo2")
    {
        return std_nintendo_compress(data, start, lenght, compressedSize, D_NINTENDO_C_MODE2);
    }
    if (name == "smw" || name == "nintendo1")
        return std_nintendo_compress(data, start, lenght, compressedSize, D_NINTENDO_C_MODE1);
    return NULL;
}

char *NintendoCompression::unCompress(QString name, const char *data, const size_t start, const unsigned int lenght, unsigned int *uncompressedSize, unsigned int *compressedLength)
{
    Q_UNUSED(lenght);
    char *toret = NULL;
    if (name == "zelda3" || name == "nintendo2")
    {
        toret = std_nintendo_decompress(data, start, 0, uncompressedSize, compressedLength, D_NINTENDO_C_MODE2);
    }
    if (name == "smw" || name == "nintendo1")
    {
        toret = std_nintendo_decompress(data, start, 0, uncompressedSize, compressedLength, D_NINTENDO_C_MODE1);
    }
    if (toret != NULL)
    {
        qDebug() << *uncompressedSize;
        qDebug() << "Checksum : " << qChecksum(toret, *uncompressedSize);
    } else
    {
        m_decompressionError = std_nintendo_decompression_error;
        qDebug() << m_decompressionError;
    }
    qDebug() << "End decompress";
    return toret;
}

QString NintendoCompression::compressionError()
{
    return QString();
}

QString NintendoCompression::decompressionError()
{
    return m_decompressionError;
}
