#include "graphicstileitem.h"

GraphicsTileItem::GraphicsTileItem(const QPixmap& img, const tile8& tile)
{
    image = img;
    rawTile = tile;
    t_enabled = true;
    tileZoom = 1;
}

QRectF GraphicsTileItem::boundingRect() const
{
    return imageScaled.rect();
}

void GraphicsTileItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->drawPixmap(0, 0, imageScaled);
}

void GraphicsTileItem::setTileZoom(unsigned int z)
{
    tileZoom = z;
    imageScaled = image.scaled(tileZoom, tileZoom);
}
