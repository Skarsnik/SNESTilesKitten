#ifndef ROMINFO_H
#define ROMINFO_H

#include <QString>

class ROMInfo
{
public:
    ROMInfo(QString romFile = QString());

    bool    hasHeader;
    QString romType;
    QString romTitle;
    unsigned int size;

};

#endif // ROMINFO_H
