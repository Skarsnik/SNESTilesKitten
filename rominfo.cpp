#include "rominfo.h"
#include <QDebug>
#include <QFile>

ROMInfo::ROMInfo(QString romFile)
{
    if (romFile.isEmpty())
            return;
    QFile f(romFile);
    f.open(QIODevice::ReadOnly);
    if (f.size() & 0x200)
    {
        qDebug() << "ROM has header";
        hasHeader = true;
    }
    else {
        hasHeader = false;
        qDebug() << "ROM has no header";
    }
    /* 0xFC0 - 21 game title
       0xFD5 -  rom makeup?
       0xFD6 - rom type (rom/ram/sram)
    */
    unsigned int header_offset = hasHeader ? 0x200 : 0;
    f.seek(0x7FC0 + header_offset);
    QByteArray mromTitle = f.read(21);
    f.seek(0x7FD5 + header_offset);
    QByteArray romMakeUp = f.read(1);
    if (romMakeUp[0] & 0x1) // bit for HiROM set
    {
        //This is unconsistent, let's try HiROM
        f.seek(0xFFC0 + header_offset);
        mromTitle = f.read(21);
        f.seek(0xFFD5 + header_offset);
        romMakeUp = f.read(1);
        if (romMakeUp[0] & 0x1)
        {
            qDebug() << "ROM is HiROM";
            romType = "HiROM";
        }
    } else {
        qDebug() << "ROM is LoROM";
        romType = "LoROM";
    }
    romTitle = mromTitle;
}
