#include "tilespattern.h"

#include <QSettings>
#include <QDebug>

QMap<QString, TilesPattern> TilesPattern::m_Patterns;

TilesPattern::TilesPattern()
{
    tilesPerRow = 16;
    numberOfTiles = 16;
}

bool TilesPattern::load(QString patternFile)
{
    QSettings   pFile(patternFile, QSettings::IniFormat);
    name = pFile.value("pattern/name").toString();
    description = pFile.value("pattern/description").toString();
    numberOfTiles = pFile.value("pattern/number_of_tile").toInt();
    qDebug() << name;
    //unsigned int nbOfTile = pFile.value("_/number_of_tile").toUInt();
    QString patternString = pFile.value("pattern/pattern").toString();
    qDebug() << patternString;

    // Pattern String is a array description

    transformVector.clear();
    QRegExp arrayRegExp("(\\[[\\s|0-F|a-f|,]+\\])");
    int pos = 0;
    while (arrayRegExp.indexIn(patternString, pos) != -1)
    {
        QString arrayString = arrayRegExp.cap(1);
        QVector<int> tmpVect;
        //qDebug() << arrayString;
        unsigned int stringPos = 1;

        while(arrayString[stringPos] != ']')
        {
            while (arrayString[stringPos].isSpace())
                stringPos++;
            QRegExp hex("([0-F|a-f]+)");
            bool    ok;
            if (hex.indexIn(arrayString, stringPos) == stringPos)
            {
                tmpVect.append(hex.cap(1).toInt(&ok, 16));
            }
            while (arrayString[stringPos].isSpace())
                stringPos++;
            stringPos++; // should be the comma
        }
        pos += arrayRegExp.matchedLength();
        transformVector.append(tmpVect);
    }
    qDebug() << transformVector.size() << transformVector;
    return true;
}

bool TilesPattern::loadPatterns(const QDir& patternDirectory)
{
    foreach (QString fileName, patternDirectory.entryList(QDir::Files))
    {
        TilesPattern tp;
        qDebug() << "Loading " << fileName;
        if (!tp.load(patternDirectory.absoluteFilePath(fileName)))
            return false;
        m_Patterns[tp.name] = tp;
    }
    return true;
}

TilesPattern TilesPattern::pattern(QString name)
{
    return m_Patterns[name];
}

QMap<QString, TilesPattern> TilesPattern::Patterns()
{
    return m_Patterns;
}

QVector<QVector<tile8> > TilesPattern::transform(const QList<tile8>& tiles) const
{
    unsigned int repeatOffsetY = 0;
    unsigned int repeatOffsetX = 0;
    unsigned int tVectHeight = transformVector.size();
    unsigned int tVectWidth = transformVector[0].size();
    unsigned int repeat = 0;
    QVector<QVector<tile8> > toret;
    unsigned int transPerRow = tilesPerRow / tVectWidth;
    unsigned int nbTransform = tiles.size() / numberOfTiles;
    if (transPerRow > nbTransform)
        toret.resize(tVectHeight);
    else
        toret.resize((unsigned int((nbTransform / transPerRow) + 0.5)) * tVectHeight);
    QMutableVectorIterator<QVector<tile8> > it(toret);
    while (it.hasNext())
    {
        it.next().resize(tilesPerRow);
    }
    //qDebug() << toret[0].size() << "x" << toret.size();
    while (repeat != nbTransform)
    {
        //qDebug() << "repeat" << repeat;
        for (unsigned int j = 0; j < tVectHeight; j++)
        {
            for (unsigned int i = 0; i < tVectWidth; i++)
            {
                unsigned int posTile = transformVector[j][i] + numberOfTiles * repeat;
                unsigned int posX = i + repeatOffsetX;
                unsigned int posY = j + repeatOffsetY;
                toret[posY][posX] = tiles[posTile];
            }
        }
        if (repeatOffsetX + tVectWidth == tilesPerRow)
        {
            repeatOffsetX = 0;
            repeatOffsetY += tVectHeight;
        }
        else
            repeatOffsetX += tVectWidth;
        repeat++;
    }
    return toret;
}

QVector<QVector<tile8> > TilesPattern::transform(const TilesPattern &pattern, const QList<tile8>& tiles)
{
    return pattern.transform(tiles);
}

QVector<QVector<tile8> > TilesPattern::transform(const QString id, const QList<tile8>& tiles)
{
    return m_Patterns[id].transform(tiles);
}

