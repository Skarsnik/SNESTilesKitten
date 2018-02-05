#include "graphicstileitem.h"
#include "mgraphicsview.h"
#include <QDebug>

MGraphicsView::MGraphicsView(QWidget *parent) : QGraphicsView(parent)
{
    m_tileUnderCursor = NULL;
    setMouseTracking(true);
}

GraphicsTileItem *MGraphicsView::tileUnderCursor()
{
    return m_tileUnderCursor;
}

void MGraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    if (scene() == NULL)
        return;
    QGraphicsItem* item = scene()->itemAt(mapToScene(event->pos()), transform());
    if (item != NULL)
    {
        GraphicsTileItem* gItem = (GraphicsTileItem*) item;
        if (gItem != m_tileUnderCursor)
        {
            m_tileUnderCursor = gItem;
            emit tileUnderCursorChanged();
        }
    }
    //event->accept();
}
