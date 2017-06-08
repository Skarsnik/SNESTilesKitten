#ifndef GRAPHICSTILESSCENE_H
#define GRAPHICSTILESSCENE_H

#include <QGraphicsScene>
#include <QObject>
#include "lowlevelstuff/src/tile.h"
#include "tilespattern.h"

class GraphicsTilesScene : public QGraphicsScene
{
public:
    GraphicsTilesScene(QObject *parent = 0);
    void    buildScene(const QList<tile8>& tiles, const QVector<QRgb> mPalette, const TilesPattern& tp);
    void    updateScene();
    void    setTilesZoom(unsigned int tileZoom);
    void    setTilesPattern(TilesPattern tp);

private:
    QList<tile8> allTiles;
    unsigned int tilesZoom;
    TilesPattern    tilesPattern;
    QMap<unsigned int, QPixmap>  imagesCache;

};

#endif // GRAPHICSTILESSCENE_H
