#ifndef GRAPHICSTILESSCENE_H
#define GRAPHICSTILESSCENE_H

#include <QGraphicsScene>
#include <QObject>
#include "lowlevelstuff/src/tile.h"
#include "snespalette.h"
#include "tilespattern.h"

class GraphicsTilesScene : public QGraphicsScene
{
public:
    GraphicsTilesScene(QObject *parent = 0);
    void    buildScene(const QList<tile8>& tiles, const SNESPalette mPalette, const TilesPattern& tp);
    void    updateScene();
    void    setTilesZoom(unsigned int tileZoom);
    void    setTilesPattern(TilesPattern tp);

private:
    QList<tile8> allTiles;
    QVector<QVector<tile8> > arrangedTiles;
    unsigned int tilesZoom;
    TilesPattern    tilesPattern;
    QMap<unsigned int, QPixmap>  imagesCache;

};

#endif // GRAPHICSTILESSCENE_H
