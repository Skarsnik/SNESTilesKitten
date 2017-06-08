#include "pngexportimport.h"
#include <QDebug>


QImage mergeTilesToImage(const QList<tile8> tiles, const QVector<QRgb>& palette, const TilesPattern& tilesPattern)
{
    QVector<QVector<tile8> > arrangedTiles = TilesPattern::transform(tilesPattern, tiles);
    unsigned tHeight = arrangedTiles.size();
    unsigned tWidth = arrangedTiles[0].size();
    //qDebug() << height;
    QImage newImage(tWidth * 8, tHeight * 8, QImage::Format_Indexed8);
    newImage.setColorCount(palette.size());
    for (unsigned int i = 0; i < palette.size(); i++)
    {
        newImage.setColor(i, palette[i]);
    }
    QString plop = "";



    for (unsigned int j = 0; j < tHeight; j++)
    {
        for (unsigned int i = 0; i < tWidth; i++)
        {
            tile8& tile = arrangedTiles[j][i];
            for (unsigned int tPos = 0; tPos < 64; tPos++)
            {
                QPoint p(i * 8 + (tPos % 8), j * 8 + tPos / 8);
                newImage.setPixel(p, tile.data[tPos]);
            }
        }
    }
    return newImage;
}

bool saveToPNG(const QImage &image, QString path)
{
    return image.save(path, "PNG");
}

QList<tile8> tilesFromPNG(const QString file)
{
    QList<tile8> toret;

    QImage image(file);
    if (image.format() != QImage::Format_Indexed8)
    {
        qCritical() << file << "is not an indexed PNG file";
        return toret;
    }
    if (image.colorCount() > 16)
    {
        qCritical() << "Color count in" << file << "is superior to 16";
        return toret;
    }
    unsigned int width = image.width();
    unsigned int height = image.height();
    qDebug() << "Image is " << width << "x" << height << "Should contain " << (width / 8) * (height / 8);
    for (unsigned int posImageH = 0; posImageH < height / 8; posImageH++)
    {
        for (unsigned int posImageW = 0; posImageW < width / 8; posImageW++)
        {
            tile8 newTile;
            for (unsigned int i = 0 ; i < 64; i++)
            {
                QPoint p(posImageW * 8 + i % 8, posImageH * 8 + i / 8);
                newTile.data[i] = image.pixelIndex(p);
            }
            toret.append(newTile);
        }
    }
    return toret;
}

QVector<QRgb> paletteFromPNG(const QString file)
{
    QVector<QRgb> toret;
    QImage image(file);
    if (image.format() != QImage::Format_Indexed8)
    {
        qCritical() << file << "is not an indexed PNG file";
        return toret;
    }
    if (image.colorCount() > 16)
    {
        qCritical() << "Color count in" << file << "is superior to 16";
        return toret;
    }
    toret = image.colorTable();
    return toret;
}
