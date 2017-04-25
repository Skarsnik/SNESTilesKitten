#include "pngexportimport.h"
#include <QDebug>


QImage mergeTilesToImage(const QList<tile8> tiles, const QVector<QRgb>& palette, const unsigned int tilesPerRow)
{
    unsigned int nbTileCol = (tiles.size() + 0.5) / tilesPerRow;
    unsigned int height = nbTileCol * 8;
    //qDebug() << height;
    QImage newImage(8 * tilesPerRow, height, QImage::Format_Indexed8);
    newImage.setColorCount(palette.size());
    for (unsigned int i = 0; i < palette.size(); i++)
    {
        newImage.setColor(i, palette[i]);
    }
    QString plop = "";

    unsigned int tileNum = 0;
    unsigned int colTile = 0;
    foreach(const tile8& tile, tiles)
    {
        unsigned int pixPos = 0;
        unsigned int yPos = 0;
        for (unsigned int i = 0; i < 64; i++)
        {
            QPoint p(pixPos + (tileNum % tilesPerRow) * 8, colTile * 8 + yPos);
            newImage.setPixel(p,  tile.data[i]);
            plop.append(QString("[%1, %2] ").arg(p.x()).arg(p.y()));
            pixPos++;
            if (pixPos % 8 == 0)
            {
                pixPos = 0;
                yPos++;
            }
        }
        //qDebug() << plop;
        plop = "";
        tileNum++;
        if (tileNum % tilesPerRow == 0)
            colTile++;
    }
    //qDebug() << "Hello";
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
