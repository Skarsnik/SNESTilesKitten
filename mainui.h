#ifndef MAINUI_H
#define MAINUI_H

#include <QMainWindow>
#include "lowlevelstuff/src/tile.h"
#include "compressioninterface.h"
#include "romdataengine.h"
#include "tilepreset.h"
#include <QGraphicsScene>
#include <QSettings>

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

    void on_headerButtonGroup_clicked(int);

    void on_paletteSNESLocationRadioButton_toggled(bool checked);

    void on_palettePCLocationRadioButton_toggled(bool checked);

    void on_refreshPushButton_clicked();

    void on_paletteGrayRadioButton_toggled(bool checked);

    void on_tilesSNESRadioButton_toggled(bool checked);

    void on_tilesPCRadioButton_toggled(bool checked);

    void on_tilesPerRowSpinBox_valueChanged(int arg1);

    void on_tileZoomHorizontalSlider_valueChanged(int value);

    void on_tilesPerRowSpinBox_editingFinished();

    void on_presetOpenPushButton_clicked();

    void on_presetSavePushButton_clicked();

    void on_pngExportPushButton_clicked();

private:
    Ui::MainUI      *ui;
    TilePreset      currentSet;
    QSettings*      m_settings;
    QString         lastPresetDirectory;
    QString         lastROMDirectory;
    QString         lastPNGDirectory;

    QString         romFile;
    unsigned int    tilesZoom;
    unsigned int    tilesPerRow;
    bool            romHasHeader;
    QGraphicsScene  *tileScene;
    QGraphicsScene  *palScene;
    ROMDataEngine   dataEngine;

    QList<tile8>    rawTiles;
    QList<QImage>   images;
    char*           rawData;
    QVector<QRgb>   mPalette;

    QMap<QString, CompressionInterface*>    availableCompressions;

    void    openRom(QString rom);
    bool    loadPreset(const QString &presetFile);
    void    updatePresetWithUi();
    void    updateUiWithPreset();

    void    updateGTileView();
    bool    extractTiles();
    void    createImageList();
    void    buildTileScene();
    void    buildPaletteScene();
    void    setGrayscalePalette(unsigned int paletteSize = 16);
    bool    loadCompressionPlugins();
    bool    extractPalette();

    void    closeEvent(QCloseEvent* event);
};

#endif // MAINUI_H
