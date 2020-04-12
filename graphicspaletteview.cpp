#include "graphicspaletteview.h"


GraphicsPaletteView::GraphicsPaletteView(QWidget *parent) : QGraphicsView (parent)
{

}

GraphicsPaletteColorItem *GraphicsPaletteView::getClickedItem()
{
    return m_current_item;
}

void GraphicsPaletteView::mouseReleaseEvent(QMouseEvent *env)
{
    if (scene() == nullptr)
        return;
    QGraphicsItem* item = scene()->itemAt(mapToScene(env->pos()), transform());
    if (item != nullptr)
    {
        GraphicsPaletteColorItem* gItem = (GraphicsPaletteColorItem*) item;
        if (gItem != m_current_item)
        {
            m_current_item = gItem;
            emit itemClicked();
        }
    }
}
