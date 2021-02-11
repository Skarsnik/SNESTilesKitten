#include "graphicspalettecoloritem.h"
#include "paletteeditor.h"
#include "ui_paletteeditor.h"

#include <QDebug>
#include <QMessageBox>

PaletteEditor::PaletteEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PaletteEditor),
    m_palette(16)
{
    ui->setupUi(this);
    scene = new QGraphicsScene();
    ui->paletteGView->setScene(scene);
    connect(ui->paletteGView, &GraphicsPaletteView::itemClicked, [=] {
        colorItem = ui->paletteGView->getClickedItem();
        ui->lineEditIndex->setText(QString::number(colorItem->iColor));
        ui->lineEditB->setText(QString::number(qBlue(colorItem->color), 16));
        ui->lineEditR->setText(QString::number(qRed(colorItem->color), 16));
        ui->lineEditG->setText(QString::number(qGreen(colorItem->color), 16));
        ui->lineEditSnes->setText(QString::number(m_palette.colors[colorItem->iColor].snes, 16));
    });
    connect(ui->lineEditR, &QLineEdit::editingFinished, this, &PaletteEditor::editColor);
    connect(ui->lineEditG, &QLineEdit::editingFinished, this, &PaletteEditor::editColor);
    connect(ui->lineEditB, &QLineEdit::editingFinished, this, &PaletteEditor::editColor);
}


void    PaletteEditor::setPalette(SNESPalette pal)
{
    unsigned int j = 0;
    unsigned int i = 0;
    unsigned colPos = 0;
    scene->clear();
    scene->setBackgroundBrush(Qt::black);
    m_palette = pal;
    foreach(SNESColor col, pal.colors)
    {
        GraphicsPaletteColorItem *newItem = new GraphicsPaletteColorItem();
        newItem->color = col.rgb;
        newItem->iColor = colPos;
        newItem->setPos(i * newItem->boundingRect().width() + i, j * newItem->boundingRect().width() + j);
        scene->addItem(newItem);
        i++;
        colPos++;
        if (i % 8 == 0) {
            j++;
            i = 0;
        }
    }
    if (pal.size == 16)
        ui->paletteSize16Radio->setChecked(true);
    if (pal.size == 8)
        ui->paletteSize8Radio->setChecked(true);
    if (pal.size == 4)
        ui->paletteSize4Radio->setChecked(true);
    if (pal.size == 2)
        ui->paletteSize2Radio->setChecked(true);
}

SNESPalette PaletteEditor::palette() const
{
    return m_palette;
}

void PaletteEditor::setRomFile(QString fileName)
{
    rom = fileName;
}

PaletteEditor::~PaletteEditor()
{
    delete ui;
}

void    PaletteEditor::editColor()
{
    bool    ok;
    colorItem->color = qRgb(ui->lineEditR->text().toUInt(&ok, 16), ui->lineEditG->text().toUInt(&ok, 16), ui->lineEditB->text().toUInt(&ok, 16));
    m_palette.colors[colorItem->iColor].setRgb(colorItem->color);
    ui->lineEditSnes->setText(QString::number(m_palette.colors[colorItem->iColor].snes, 16));
    scene->update();
}


void PaletteEditor::on_lineEditSnes_editingFinished()
{
    bool    ok;
    SNESColor& col = m_palette.colors[colorItem->iColor];
    col.setSNES(ui->lineEditSnes->text().toUInt(&ok, 16));
    ui->lineEditR->setText(QString::number(qRed(col.rgb)));
    ui->lineEditB->setText(QString::number(qBlue(col.rgb)));
    ui->lineEditG->setText(QString::number(qGreen(col.rgb)));
    colorItem->color = col.rgb;
    scene->update();
}


//318C 0000 004A 0012 0018 001E 010C 0170 02BE 3086 614C 5A90 7356 214E 4256 635E

static QByteArray   sbData()
{
    uchar rawData[32] = {0x8C, 0x31, 0, 0, 0x4A, 0, 0x12, 0, 0x18, 0, 0x1E, 0, 0x0C, 0x01,
                        0x70, 0x01, 0xBE, 0x02, 0x86, 0x30, 0x4C, 0x61, 0x90, 0x54, 0x56,
                         0x73, 0x4E, 0x21, 0x56, 0x42, 0x5E, 0x63};
    return QByteArray((char*)rawData, 16);
}


void PaletteEditor::on_searchButton_clicked()
{
    QFile romQFile(rom);
    romQFile.open(QIODevice::ReadOnly);
    QByteArray romData = romQFile.readAll();
    QByteArray paletteData = m_palette.encode();
    //QByteArray paletteData = sbData();
    qDebug() << paletteData;
    int pos = romData.indexOf(paletteData);
    qDebug() << pos;
    QString text;
    if (pos == -1)
        text = tr("No corresponding data found in the rom");
    else {
        text = QString(tr("Data for palette found at : %1")).arg(pos, 0, 16);
    }
    QMessageBox::information(this, tr("Palette Data "),  text);
}

void PaletteEditor::on_pushButton_clicked()
{
    emit paletteChanged();
}

void PaletteEditor::on_quitPushButton_clicked()
{
    emit paletteChanged();
    this->hide();
}
