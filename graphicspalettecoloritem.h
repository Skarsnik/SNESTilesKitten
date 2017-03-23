#ifndef GRAPHICSPALETTECOLORITEM_H
#define GRAPHICSPALETTECOLORITEM_H

#include <QObject>
#include <QGraphicsItem>
#include <QRgb>

class GraphicsPaletteColorItem : public QGraphicsItem
{
public:
    GraphicsPaletteColorItem();

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                     QWidget *widget);

    QRgb            color;
    unsigned int    iColor;


};

#endif // GRAPHICSPALETTECOLORITEM_H
