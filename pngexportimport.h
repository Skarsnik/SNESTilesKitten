#ifndef PNGEXPORTIMPORT_H
#define PNGEXPORTIMPORT_H


#include <QImage>
#include <c_stuff/tile.h>

QImage  mergeTilesToImage(const QList<tile8> tiles, const QVector<QRgb> &palette, const unsigned int tilesPerRow);
bool    saveToPNG(const QImage& image, QString path);

#endif // PNGEXPORTIMPORT_H
