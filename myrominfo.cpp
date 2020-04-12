#include "myrominfo.h"
#include "lowlevelstuff/src/rominfo.h"
#include <QDebug>
#include <QFile>
#include <QFileInfo>

ROMInfo::ROMInfo(QString romFile)
{
    romType = "LoROM";
    romTitle = "Not A ROM";
    hasHeader = false;
    if (romFile.isEmpty())
            return;
    QFileInfo fi(romFile);
    size = fi.size();
    if (fi.size() < 0x7C0)
        return ;
    QFile f(romFile);
    f.open(QIODevice::ReadOnly);
    if (f.size())
    {
        if (f.size() & 0x200)
        {
            qDebug() << "ROM has header";
            hasHeader = true;
        } else {
            hasHeader = false;
            qDebug() << "ROM has no header";
        }
    }
    /* 0xFC0 - 21 game title
       0xFD5 -  rom makeup?
       0xFD6 - rom type (rom/ram/sram)
    */
    unsigned int header_offset = hasHeader ? 0x200 : 0;
    f.seek(0x7FC0 + header_offset);
    QByteArray headerData = f.read(32);
    rom_infos* crom_info = get_rom_info(headerData.data());
    qDebug() << QString::number(crom_info->checksum, 16) << QString::number(crom_info->checksum_comp, 16);
    qDebug() << crom_info->title;
    if (!crom_info->make_sense)
    {
        f.seek(0xFFC0 + header_offset);
        QByteArray headerData = f.read(32);
        free(crom_info);
        crom_info = get_rom_info(headerData.data());
        qDebug() << QString::number(crom_info->checksum, 16) << QString::number(crom_info->checksum_comp, 16);
        qDebug() << crom_info->title;
    }
    if (crom_info->type == HiROM)
        romType = "HiROM";
    romTitle = crom_info->title;
    free(crom_info);
    qDebug() << romFile << "is " << romType << hasHeader << size;
}
