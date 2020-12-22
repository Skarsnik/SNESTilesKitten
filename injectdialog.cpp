#include <QGraphicsScene>
#include "graphicspalettecoloritem.h"
#include "injectdialog.h"
#include "pngexportimport.h"
#include "ui_injectdialog.h"
#include <QDebug>

void    createPaletteScene(QGraphicsScene* palScene, SNESPalette pal)
{
    unsigned int j = 0;
    unsigned int i = 0;
    unsigned colPos = 0;
    palScene->setBackgroundBrush(Qt::black);
    foreach(SNESColor col, pal.colors)
    {
        GraphicsPaletteColorItem *newItem = new GraphicsPaletteColorItem();
        newItem->color = col.rgb;
        newItem->iColor = colPos;
        newItem->setPos(i * newItem->boundingRect().width() + i, j * newItem->boundingRect().width() + j);
        palScene->addItem(newItem);
        i++;
        colPos++;
        if (i % 8 == 0) {
            j++;
            i = 0;
        }
    }
}

InjectDialog::InjectDialog(QWidget *parent, QString imgFile, const SNESPalette& originPal) :
    QDialog(parent),
    ui(new Ui::InjectDialog)
{
    ui->setupUi(this);
    ui->imageNameLabel->setText(imgFile);
    /*QImage img(imgFile);*/
    QPixmap pix;
    pix.load(imgFile);
    SNESPalette pal = paletteFromPNG(imgFile);

    QGraphicsScene* imgPaletteScene = new QGraphicsScene(this);
    QGraphicsScene* originalPaletteScene = new QGraphicsScene(this);
    createPaletteScene(imgPaletteScene, pal);
    createPaletteScene(originalPaletteScene, originPal);

    QGraphicsScene* imgScene = new QGraphicsScene(this);
    imgScene->addPixmap(pix);
    imgScene->setSceneRect(pix.rect());
    qDebug() << pix.rect();
    ui->imageGraphicsView->setScene(imgScene);
    ui->originalPaletteGraphicsView->setScene(originalPaletteScene);
    ui->imagePalettegraphicsView->setScene(imgPaletteScene);
    ui->imageGraphicsView->scale(3, 3);
    ui->imageGraphicsView->centerOn(0, 0);
}

InjectDialog::~InjectDialog()
{
    delete ui;
}

void InjectDialog::setCopyRomString(const QString &file)
{
    ui->copyCheckBox->setText(tr("Work on a copy of the rom file : ") + file);
}

void InjectDialog::on_buttonBox_accepted()
{
    useImagePalette = ui->replacePaletteCheckBox->isChecked();
    copyRom = ui->copyCheckBox->isChecked();
    accept();
}

void InjectDialog::on_buttonBox_rejected()
{
    reject();
}
