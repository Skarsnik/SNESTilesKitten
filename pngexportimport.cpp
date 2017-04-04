#include "pngexportimport.h"


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
