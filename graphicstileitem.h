#ifndef GRAPHICSTILEITEM_H
#define GRAPHICSTILEITEM_H

#include <QObject>
#include <QGraphicsItem>
#include <QPainter>
#include "lowlevelstuff/src/tile.h"
#include <QPixmap>

class GraphicsTileItem : public QGraphicsItem
{
public:
    GraphicsTileItem(const QPixmap &img, const tile8 &tile);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                     QWidget *widget);
    void            setTileZoom(unsigned int z);
    unsigned int    tileZoom;
    tile8           rawTile;
    QPixmap         image;
    bool            t_enabled;
    QPixmap         imageScaled;
};

#endif // GRAPHICSTILEITEM_H
