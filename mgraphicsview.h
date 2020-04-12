#ifndef MGRAPHICSVIEW_H
#define MGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QMouseEvent>
#include <QObject>
#include <QWidget>
#include "lowlevelstuff/src/tile.h"
#include "graphicstileitem.h"

class MGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    MGraphicsView(QWidget* parent);
    GraphicsTileItem*  tileUnderCursor();

signals:
    void    tileUnderCursorChanged();

protected:
    void    mouseMoveEvent(QMouseEvent* event);

private:
    GraphicsTileItem*   m_tileUnderCursor;
};

#endif // MGRAPHICSVIEW_H

