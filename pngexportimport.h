#ifndef PNGEXPORTIMPORT_H
#define PNGEXPORTIMPORT_H


#include <QImage>
#include <QRgb>
#include <lowlevelstuff/src/tile.h>

QImage  mergeTilesToImage(const QList<tile8> tiles, const QVector<QRgb> &palette, const unsigned int tilesPerRow);
bool    saveToPNG(const QImage& image, QString path);

QList<tile8> tilesFromPNG(const QString file);
QVector<QRgb>   paletteFromPNG(const QString file);

#endif // PNGEXPORTIMPORT_H
