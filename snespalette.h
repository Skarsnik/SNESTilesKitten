#ifndef SNESPALETTE_H
#define SNESPALETTE_H

#include <QRgb>
#include <QVector>
#include <QtGlobal>

// Snes color are encoded on 2 bytes, 0BBBBBGG GGGRRRRR
struct SNESColor
{
    SNESColor();
    quint16 snes;
    QRgb    rgb;
    void    setRgb(QRgb);
    void    setSNES(quint16);
    quint16 approxSNES();
    QRgb    approxRGB();
};

class SNESPalette
{
public:
    SNESPalette();
    SNESPalette(quint8 mSize);
    SNESPalette(QByteArray snesPal);
    SNESPalette(QVector<QRgb>);

    QByteArray          encode();
    quint8              size;
    QVector<SNESColor>  colors;
};

#endif // SNESPALETTE_H
