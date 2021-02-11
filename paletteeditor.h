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
    SNESPalette palette() const;

    void    setRomFile(QString fileName);


signals:
    void paletteChanged();

private slots:
    void on_lineEditSnes_editingFinished();

    void on_searchButton_clicked();

    void on_pushButton_clicked();

    void on_quitPushButton_clicked();

private:
    Ui::PaletteEditor*          ui;
    QGraphicsScene*             scene;
    GraphicsPaletteColorItem*   colorItem;
    SNESPalette                 m_palette;
    QString                     rom;
    void editColor();
};

#endif // PALETTEEDITOR_H
