#ifndef MAINUI_H
#define MAINUI_H

#include <QMainWindow>
#include "c_stuff/tile.h"
#include "compressioninterface.h"
#include "tilepreset.h"
#include <QGraphicsScene>

namespace Ui {
class MainUI;
}

class MainUI : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainUI(QWidget *parent = 0);
    ~MainUI();

private slots:
    void on_romOpenButton_clicked();

    void on_pushButton_3_clicked();
    void on_headerButtonGroup_clicked(int);

private:
    Ui::MainUI      *ui;
    TilePreset      currentSet;

    QString         romFile;
    QGraphicsScene  *tileScene;
    QGraphicsScene  *palScene;
    QList<tile8>    rawTiles;
    QList<QImage>   images;
    char*           rawData;
    QVector<QRgb>   mPalette;

    QMap<QString, CompressionInterface*>    availableCompressions;

    void    updateUiWithPreset();
    bool    extractTiles();
    void    createImageList();
    void    buildScene();
    void    buildPaletteScene();
    void    setGrayscalePalette();
    bool    loadCompressionPlugins();
    bool    extractPalette();
};

#endif // MAINUI_H
