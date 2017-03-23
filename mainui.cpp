#include "compressioninterface.h"
#include "graphicspalettecoloritem.h"
#include "mainui.h"
#include "ui_mainui.h"

#include <QFile>
#include <QFileDialog>
#include <QDebug>
#include <QtMath>
#include <QGraphicsPixmapItem>
#include <QPluginLoader>
#include "c_stuff/rommapping.h"
#include "c_stuff/palette.h"

MainUI::MainUI(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainUI)
{
    ui->setupUi(this);
    currentSet.header = true;
    currentSet.bpp = 4;
    currentSet.romType = "LoROM";
    currentSet.size = 0x86FFF - 0x80000;
    currentSet.tilesLocation = 0; // 0x10F000;//0x108000; $80000-$86FFF
    currentSet.pcTilesLocation = 0x80000;
    currentSet.compression = "None";
    currentSet.paletteLocation = 0;
    currentSet.pcPaletteLocation = 0xDD308;
    tileScene = new QGraphicsScene();
    palScene = new QGraphicsScene();
    romFile = "D:\\Emulation\\Zelda - A Link to the Past\\Zelda - A Link to the Past.smc";

    ui->graphicsView->setScene(tileScene);
    ui->paletteGraphicsView->setScene(palScene);
    setGrayscalePalette();
    buildPaletteScene();
    //ui->graphicsView->scale(5, 5);
    connect(ui->headerButtonGroup, SIGNAL(buttonClicked(int)), this, SLOT(on_headerButtonGroup_clicked(int)));
    if (loadCompressionPlugins())
    {
        foreach(const QString& key, availableCompressions.keys())
        {
            ui->compressionComboBox->addItem(key);
        }
    } else {
        qApp->exit(1);
    }
    updateUiWithPreset();
}

MainUI::~MainUI()
{
    delete ui;
}

void    show_tile8(tile8 t)
{
    printf("{\n");
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
          printf("%d ", t.data[i + j * 8]);
        printf("\n");
    }
    printf("}\n");
}

bool MainUI::extractTiles()
{
    QFile romFile(romFile);
    uint    filePos = 0;
    enum rom_type rType = HiROM;
    unsigned int size = currentSet.size;

    if (currentSet.romType == "LoROM")
        rType = LoROM;
    romFile.open(QIODevice::ReadOnly);
    if (currentSet.tilesLocation != 0) {
        filePos = rommapping_snes_to_pc(currentSet.tilesLocation, rType, currentSet.header);
        if (filePos == -1)
        {
          qDebug() << rommapping_error_text;
          return false;
        }
    } else {
        qDebug() << "direct file location";
        filePos = currentSet.pcTilesLocation;
        if (currentSet.header)
            filePos += 0x200;
    }

    qDebug() << "Location" << QString::number(filePos, 16);
    romFile.seek(filePos);
    QByteArray qdata = romFile.read(currentSet.size);
    char*   data = qdata.data();
    QString compressionSelected = currentSet.compression;
    if (compressionSelected != "None") {
        qDebug() << "Using " << compressionSelected << " compression";
        data = availableCompressions[compressionSelected]->unCompress(compressionSelected, data, 0, &size);
        if (data == NULL)
            return false;
    }
    qDebug() << "Size : " << size;
    for (int tilePos = 0; tilePos < size; tilePos += currentSet.bpp * 8) {
        tile8   newTile = unpack_bpp_tile(data, tilePos, currentSet.bpp);
        rawTiles.append(newTile);
    }
    return true;
}

void MainUI::createImageList()
{
    foreach (const tile8 tile, rawTiles)
    {
        QImage newImage(8, 8, QImage::Format_Indexed8);
        newImage.setColorCount(mPalette.size());
        for (int i = 0; i < mPalette.size(); i++)
        {
            newImage.setColor(i, mPalette[i]);
        }
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
               newImage.setPixel(i, j, tile.data[i + j * 8]);
        }
        /*show_tile8(tile);
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
                printf("%u, ", (unsigned int) newImage.pixelIndex(i, j));
            printf("\n");
        }*/
        images.append(newImage);
    }
}

void MainUI::buildScene()
{
    tileScene->clear();
    int i = 0;
    int j = 0;
    //scene->addRect(0, 0, 120, (images.size() / 8) * 30, QPen(Qt::blue), Qt::blue);
    tileScene->setBackgroundBrush(Qt::blue);
    foreach(QImage image, images)
    {
        QGraphicsPixmapItem *newPixItem = new QGraphicsPixmapItem(QPixmap().fromImage(image).scaled(30, 30));
        newPixItem->setPos(i * newPixItem->boundingRect().width() + i, j * newPixItem->boundingRect().width() + j);
        tileScene->addItem(newPixItem);
        i++;
        if (i % 16 == 0) {
            j++;
            i = 0;
        }
    }
}

void MainUI::buildPaletteScene()
{
    palScene->clear();
    unsigned int j = 0;
    unsigned int i = 0;
    unsigned colPos = 0;
    palScene->setBackgroundBrush(Qt::black);
    foreach(QRgb col, mPalette)
    {
        GraphicsPaletteColorItem *newItem = new GraphicsPaletteColorItem();
        newItem->color = col;
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

void MainUI::setGrayscalePalette()
{
    mPalette.clear();
    unsigned int psize = qPow(2, currentSet.bpp);
    for (int i = 0; i < psize; i++)
    {
        QRgb color = qRgb(i * (255 / psize), i * (255 / psize), i * (255 / psize));
        mPalette.append(color);
    }
}

bool MainUI::loadCompressionPlugins()
{
    QDir pluginsDir(qApp->applicationDirPath());
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
    foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
        if (!fileName.contains(QRegExp(".*\\.(dll|so)$")))
           continue;
        QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(fileName));
        QObject *plugin = pluginLoader.instance();
        if (plugin) {
            CompressionInterface*   ci = qobject_cast<CompressionInterface *>(plugin);
            if (ci) {
                qDebug() << "Loaded " << fileName;
                foreach(CompressionInfo info, ci->compressionList()) {
                    qDebug() << "  " << info.name << " : " << info.shortDescription;
                    availableCompressions[info.name] = ci;
                }
            }
            else
            {
                qCritical() << "Loaded pluging(" << fileName << ")is not a compression plugin";
                return false;
            }
        }
        else {
            qCritical() << "Can't load " << fileName;
            return false;
        }
    }
    return true;
}

bool MainUI::extractPalette()
{
    mPalette.clear();
    QFile plop(romFile);
    unsigned filePos = currentSet.pcPaletteLocation;
    if (currentSet.header)
        filePos += 0x200;

    plop.open(QIODevice::ReadOnly);
    plop.seek(filePos);
    unsigned int palette_size = qPow(2, currentSet.bpp);
    QByteArray ab = plop.read(palette_size * 2);
    char* data = ab.data();
    s_palette* raw_pal = extract_palette(data, 0, palette_size);
    for (int i = 0; i < palette_size; i++)
    {
        m_color col = raw_pal->colors[i];
        mPalette.append(qRgb(col.red, col.green, col.blue));
    }
    return true;
}

void MainUI::on_romOpenButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
          tr("Select ROM"), QString(), tr("SNES ROM (*.smc *.sfc)"));
    if (!fileName.isNull())
        romFile = fileName;
}

void MainUI::on_pushButton_3_clicked()
{
    extractPalette();
    if (!extractTiles())
        return;
    createImageList();
    buildScene();
    buildPaletteScene();
}

void MainUI::on_headerButtonGroup_clicked(int)
{
    currentSet.header = ui->headerRadioButton->isChecked();
}

void MainUI::updateUiWithPreset()
{
    ui->headerRadioButton->setChecked(currentSet.header);
    ui->loRomRadioButton->setChecked(currentSet.romType == "LoROM");
    if (currentSet.tilesLocation != 0)
        ui->snesAddrLineEdit->setText(QString::number(currentSet.tilesLocation, 16));
    else
        ui->pcAddrLineEdit->setText(QString::number(currentSet.pcTilesLocation, 16));
    ui->sizeLineEdit->setText(QString::number(currentSet.size));
    ui->compressionComboBox->setCurrentText(currentSet.compression);
    switch (currentSet.bpp)
    {
        case 2:
            ui->bpp2RadioButton->setChecked(true);
        case 3:
            ui->bpp3RadioButton->setChecked(true);
        case 4:
            ui->bpp4RadioButton->setChecked(true);
    }
}
