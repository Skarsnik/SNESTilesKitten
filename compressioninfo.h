#ifndef COMPRESSIONINFO_H
#define COMPRESSIONINFO_H

#include <QString>

struct CompressionInfo
{
    QString name;
    QString shortDescription;
    QString description;
    bool    canCompress;
};

#endif // COMPRESSIONINFO_H
