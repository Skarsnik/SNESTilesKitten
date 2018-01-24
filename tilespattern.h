#ifndef TILESPATTERN_H
#define TILESPATTERN_H
#include "lowlevelstuff/src/tile.h"
#include <QDir>
#include <QVector>


class TilesPattern
{
public:
    TilesPattern();
    QString         name;
    QString         description;
    bool            custom;
    unsigned int    tilesPerRow;
    unsigned int    numberOfTiles;

    bool                                load(QString patternFile);

    static  bool                        loadPatterns(const QDir& patternDirectory);
    static  TilesPattern                pattern(QString name);
    static  QMap<QString, TilesPattern> Patterns();
    static  QVector<QVector<tile8> >    transform(const TilesPattern& pattern, const QList<tile8> &tiles);
    static  QVector<QVector<tile8> >    transform(const QString id, const QList<tile8> &tiles);
    static  QList<tile8>                reverse(const TilesPattern& pattern, const QList<tile8>& tiles);
protected:
    QVector<QVector<tile8> > transform(const QList<tile8> &tiles) const;
    QList<tile8>             reverse(const QList<tile8>& tiles) const;
    QVector<QVector<int> > transformVector;
private:
    static QMap<QString, TilesPattern> m_Patterns;
};


#endif // TILESPATTERN_H
