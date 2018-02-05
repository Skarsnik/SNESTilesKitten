#include "graphicstileitem.h"
#include "graphicstilesscene.h"
#include <QDebug>

GraphicsTilesScene::GraphicsTilesScene(QObject* parent) : QGraphicsScene(parent)
{
    tilesZoom = 30;
}

void GraphicsTilesScene::buildScene(const QList<tile8> &tiles, const QVector<QRgb> mPalette, const TilesPattern &tp)
{
    arrangedTiles = TilesPattern::transform(tp, tiles);
    tilesPattern = tp;
    allTiles = tiles;
    clear();
    setBackgroundBrush(Qt::blue);
    for (unsigned int j = 0; j < arrangedTiles.size(); j++)
    {
        for (unsigned int i = 0; i < arrangedTiles[0].size(); i++)
        {
            tile8 tile = arrangedTiles[j][i];
            QImage newImage(8, 8, QImage::Format_Indexed8);
            newImage.setColorCount(mPalette.size());
            for (int i = 0; i < mPalette.size(); i++)
            {
                newImage.setColor(i, mPalette[i]);
            }
            for (int i = 0; i < 8; i++)
            {
                for (int j = 0; j < 8; j++)
                   newImage.setPixel(i, j, tile.data[i + j * 8]);
            }
            QPixmap m;
            m.convertFromImage(newImage);
            imagesCache[tile.id] = m;
            GraphicsTileItem *newTileItem = new GraphicsTileItem(m, tile);
            addItem(newTileItem);
            newTileItem->setTileZoom(tilesZoom);
            newTileItem->setPos(i * newTileItem->boundingRect().width() + i, j * newTileItem->boundingRect().width() + j);
        }
    }
    unsigned max_w = items()[0]->boundingRect().width() * arrangedTiles[0].size() + arrangedTiles[0].size();
    unsigned max_h = items()[0]->boundingRect().width() * arrangedTiles.size() + arrangedTiles.size();
    setSceneRect(QRect(0, 0, max_w, max_h));
}

void GraphicsTilesScene::updateScene()
{
    qDebug() << "Update scene";
    unsigned int itemCpt = 0;
    for (unsigned int j = 0; j < arrangedTiles.size(); j++)
    {
        for (unsigned int i = 0; i < arrangedTiles[0].size(); i++)
        {
            tile8 tile = arrangedTiles[j][i];
            QPixmap m = imagesCache[tile.id];
            GraphicsTileItem *tileItem = (GraphicsTileItem*) items()[itemCpt];
            tileItem->image = m;
            tileItem->rawTile = tile;
            tileItem->setTileZoom(tilesZoom);
            tileItem->setPos(i * tileItem->boundingRect().width() + i, j * tileItem->boundingRect().width() + j);
            itemCpt++;
        }
    }
    unsigned max_w = items()[0]->boundingRect().width() * arrangedTiles[0].size() + arrangedTiles[0].size();
    unsigned max_h = items()[0]->boundingRect().width() * arrangedTiles.size() + arrangedTiles.size();
    setSceneRect(QRect(0, 0, max_w, max_h));
}

void GraphicsTilesScene::setTilesZoom(unsigned int tileZoom)
{

    tilesZoom = tileZoom;
    if (!items().isEmpty())
        updateScene();
}

void GraphicsTilesScene::setTilesPattern(TilesPattern tp)
{
    tilesPattern = tp;
}

