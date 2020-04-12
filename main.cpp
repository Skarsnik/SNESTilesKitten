#include "mainui.h"
#include "paletteeditor.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    // Windows hack to have output in the console
    QApplication a(argc, argv);

    QApplication::setApplicationName("SNESTilesKitten");
    QApplication::setApplicationVersion(QString(__DATE__));

    QDir pluginsDir(qApp->applicationDirPath());
//TODO Linux handling
#if defined(Q_OS_WIN)
    if (pluginsDir.dirName().toLower() == "debug" || pluginsDir.dirName().toLower() == "release")
        pluginsDir.cdUp();
#elif defined(Q_OS_MAC)
    if (pluginsDir.dirName() == "MacOS") {
        pluginsDir.cdUp();
        pluginsDir.cdUp();
        pluginsDir.cdUp();
    }
#endif
    pluginsDir.cd("plugins");
    QDir patternDir = qApp->applicationDirPath();
    if (patternDir.dirName().toLower() == "debug" || patternDir.dirName().toLower() == "release")
    {
        patternDir.cdUp();
        patternDir.cdUp();
        patternDir.cd("SNESTilesKitten");
    }
    patternDir.cd("TilesPatterns");
    ROMDataEngine::loadCompressionPlugins(pluginsDir);
    TilesPattern::loadPatterns(patternDir);


    /*SNESPalette mPalette(16);
    unsigned int psize = 16;
    for (unsigned int i = 0; i < psize; i++)
    {
        QRgb color = qRgb(i * (255 / psize), i * (255 / psize), i * (255 / psize));
        SNESColor col;
        col.setRgb(color);
        mPalette.colors[i] = col;
    }
    PaletteEditor* p = new PaletteEditor();
    p->setPalette(mPalette);
    p->show();
    return a.exec();*/

    /*QList<tile8> someTiles;
    for (unsigned int i = 0; i < 64; i++)
    {
        tile8 t;
        t.id = i;
        someTiles.append(t);
    }
    /*qDebug() << TilesPattern::transform(TilesPattern::pattern("normal"), someTiles);
    qDebug() << TilesPattern::transform(TilesPattern::pattern("32x32 B (4x4)"), someTiles);
    qDebug() << TilesPattern::transform(TilesPattern::pattern("32x32 A (2x2*4)"), someTiles);
    QVector<QVector<tile8> > tt = TilesPattern::transform(TilesPattern::pattern("32x32 B (4x4)"), someTiles);
    QList<tile8> l;
    foreach (QVector<tile8> v, tt)
    {
        qDebug() << v;
        l.append(v.toList());
    }
    qDebug() << "Reverse" << l;
    qDebug() << TilesPattern::reverse(TilesPattern::pattern("32x32 B (4x4)"), l);
    return 0;*/
    MainUI w;
    w.show();
    return a.exec();
}
