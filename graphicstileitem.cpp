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
    QPixmap pix = image.scaled(tileZoom, tileZoom);
    return pix.rect();
}

void GraphicsTileItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPixmap pix = image.scaled(tileZoom, tileZoom);
    painter->drawPixmap(0, 0, pix);
}
