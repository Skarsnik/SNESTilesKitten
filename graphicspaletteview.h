#ifndef GRAPHICSPALETTEVIEW_H
#define GRAPHICSPALETTEVIEW_H

#include "graphicspalettecoloritem.h"

#include <QGraphicsView>
#include <QMouseEvent>
#include <QObject>
#include <QWidget>

class GraphicsPaletteView : public QGraphicsView
{
    Q_OBJECT
public:
    GraphicsPaletteView(QWidget* parent);
    GraphicsPaletteColorItem* getClickedItem();

signals:
    void    itemClicked();
protected:
    void    mouseReleaseEvent(QMouseEvent* env);

private:
    GraphicsPaletteColorItem* m_current_item;
};

#endif // GRAPHICSPALETTEVIEW_H
