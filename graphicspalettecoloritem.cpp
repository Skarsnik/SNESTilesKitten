#include "graphicspalettecoloritem.h"

#include <QPainter>
#include <QFont>

GraphicsPaletteColorItem::GraphicsPaletteColorItem()
{

}

QRectF GraphicsPaletteColorItem::boundingRect() const
{
    return QRect(-10, -10, 20,  20);
}

void GraphicsPaletteColorItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(color);

    painter->fillRect(boundingRect(), color);
    painter->setPen(Qt::black);
    //painter->setBrush(Qt::black);
    painter->setFont(QFont("Bitstream Bold", 13));
    painter->drawText(boundingRect(), Qt::AlignCenter, QString::number(iColor));
    painter->setPen(Qt::white);
    painter->setFont(QFont("Bitstream Bold", 10));
    painter->drawText(boundingRect(), Qt::AlignCenter, QString::number(iColor));
}
