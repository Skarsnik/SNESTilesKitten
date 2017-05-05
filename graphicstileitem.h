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
    QPixmap image;
    tile8   rawTile;
    bool    t_enabled;
    unsigned int tileZoom;
};

#endif // GRAPHICSTILEITEM_H
