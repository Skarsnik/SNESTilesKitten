#ifndef GRAPHICSTILESSCENE_H
#define GRAPHICSTILESSCENE_H

#include <QGraphicsScene>
#include <QObject>
#include "lowlevelstuff/src/tile.h"

class GraphicsTilesScene : public QGraphicsScene
{
public:
    GraphicsTilesScene(QObject *parent = 0);
    void    buildScene(const QList<QImage>& images, const QList<tile8>& tiles);
    void    updateScene();
    void    setTilesZoom(unsigned int tileZoom);
    void    setTilesPerRow(unsigned int value);

private:
    unsigned int tilesZoom;
    unsigned int tilesPerRow;

};

#endif // GRAPHICSTILESSCENE_H
