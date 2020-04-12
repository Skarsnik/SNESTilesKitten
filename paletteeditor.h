#ifndef PALETTEEDITOR_H
#define PALETTEEDITOR_H

#include <QGraphicsScene>
#include <QWidget>
#include "lowlevelstuff/src/palette.h"
#include "graphicspalettecoloritem.h"
#include "snespalette.h"

namespace Ui {
class PaletteEditor;
}

class PaletteEditor : public QWidget
{
    Q_OBJECT

public:
    explicit PaletteEditor(QWidget *parent = nullptr);
    ~PaletteEditor();
    void setPalette(SNESPalette pal);
    SNESPalette getCurrentPalette();
    void    setRomFile(QString fileName);

private slots:
    void on_lineEditSnes_editingFinished();

    void on_searchButton_clicked();

private:
    Ui::PaletteEditor*          ui;
    QGraphicsScene*             scene;
    GraphicsPaletteColorItem*   colorItem;
    SNESPalette                 palette;
    QString                     rom;
    void editColor();
};

#endif // PALETTEEDITOR_H
