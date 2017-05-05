#include "graphicstileitem.h"
#include "graphicstilesscene.h"

GraphicsTilesScene::GraphicsTilesScene(QObject* parent) : QGraphicsScene(parent)
{
    tilesZoom = 30;
}

void GraphicsTilesScene::buildScene(const QList<QImage> &images, const QList<tile8> &tiles)
{
    clear();
    setBackgroundBrush(Qt::blue);
    unsigned int cpt = 0;
    foreach(QImage image, images)
    {
        QPixmap m;
        m.convertFromImage(image);
        GraphicsTileItem *newTileItem = new GraphicsTileItem(m, tiles[cpt]);
        addItem(newTileItem);
        cpt++;
    }
    updateScene();
}

void GraphicsTilesScene::updateScene()
{
    unsigned int i = 0;
    unsigned int j = 0;
    foreach(QGraphicsItem* item, items(Qt::AscendingOrder))
    {
        GraphicsTileItem* tItem = (GraphicsTileItem*) item;
        tItem->tileZoom = tilesZoom;
        tItem->setPos(i * tItem->boundingRect().width() + i, j * tItem->boundingRect().width() + j);
        i++;
        if (i % tilesPerRow == 0) {
            j++;
            i = 0;
        }
    }
    unsigned max_w = items()[0]->boundingRect().width() * tilesPerRow + tilesPerRow - 1;
    unsigned max_h = items()[0]->boundingRect().width() * j + j;
    setSceneRect(QRect(0, 0, max_w, max_h));
}

void GraphicsTilesScene::setTilesZoom(unsigned int tileZoom)
{

    tilesZoom = tileZoom;
    if (!items().isEmpty())
        updateScene();
}

void GraphicsTilesScene::setTilesPerRow(unsigned int value)
{
    tilesPerRow = value;
    if (!items().isEmpty())
        updateScene();
}
