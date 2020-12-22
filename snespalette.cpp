#include "snespalette.h"
#include "lowlevelstuff/src/palette.h"
#include <QColor>


SNESColor::SNESColor()
{
    rgb = 0;
    snes = 0;
}

void SNESColor::setRgb(QRgb val)
{
    rgb = val;
    m_color col;
    col.red = qRed(val);
    col.blue = qBlue(val);
    col.green = qGreen(val);
    snes = convertcolor_rgb_to_snes(col);
}

void SNESColor::setSNES(quint16 val)
{
    snes = val;
    m_color col = convertcolor_snes_to_rgb(val);
    rgb = qRgb(col.red, col.green, col.blue);
}


SNESPalette::SNESPalette()
{
    size = 0;
}

SNESPalette::SNESPalette(quint8 mSize)
{
    size = mSize;
    for (unsigned int i = 0; i < mSize; i++) {
        SNESColor col;
        colors.append(col);
    }
}

SNESPalette::SNESPalette(QByteArray data)
{
    assert((data.size() % 4 == 0) && data.size() <= 32);
    size = data.size() / 2;
    for (unsigned i = 0; i < data.size(); i += 2)
    {
        SNESColor col;
        col.snes = static_cast<uchar>(data[i + 1]) << 8;
        col.snes = col.snes | static_cast<uchar>(data[i]);
        m_color mColor = convertcolor_snes_to_rgb(col.snes);
        col.rgb = qRgb(mColor.red, mColor.green, mColor.blue);
        colors.append(col);
    }
}

SNESPalette::SNESPalette(QVector<QRgb> cols)
{
    foreach(QRgb col, cols) {
        SNESColor scol;
        scol.setRgb(col);
        colors.append(scol);
    }
    size = cols.size();
}

#include <QDebug>

QByteArray SNESPalette::encode()
{
    QByteArray data(size * 2, 0);
    for (unsigned int i = 0; i < size; i++)
    {
        qDebug() << QString::number(colors[i].snes, 16);
        data[i * 2] = (char)(colors[i].snes & 0xFF);
        data[i * 2 + 1] = (char)(colors[i].snes >> 8);
    }
    return data;
}

