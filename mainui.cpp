#include "compressioninterface.h"
#include "graphicspalettecoloritem.h"
#include "mainui.h"
#include "ui_mainui.h"

#include <QFile>
#include <QFileDialog>
#include <QDebug>
#include <QtMath>
#include <QDir>
#include <QGraphicsPixmapItem>
#include <QPluginLoader>
#include <QInputDialog>
#include "lowlevelstuff/src/rommapping.h"
#include "lowlevelstuff/src/palette.h"
#include "pngexportimport.h"


MainUI::MainUI(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainUI)
{
    ui->setupUi(this);
    ui->presetOpenPushButton->setIcon(style()->standardPixmap(QStyle::SP_DialogOpenButton));
    ui->presetSavePushButton->setIcon(style()->standardPixmap(QStyle::SP_DialogSaveButton));
    QRegExpValidator* hexValid = new QRegExpValidator(QRegExp("[0-F|a-f]{0,8}"));
    romHasHeader = true;

    ui->snesAddrLineEdit->setValidator(hexValid);
    ui->pcAddrLineEdit->setValidator(hexValid);
    ui->palettePCLocationLineEdit->setValidator(hexValid);
    ui->paletteSNESLocationLineEdit->setValidator(hexValid);

    tilesZoom = 30;
    tilesPerRow = 16;

    tileScene = new QGraphicsScene();
    palScene = new QGraphicsScene();
    ui->graphicsView->setScene(tileScene);
    ui->paletteGraphicsView->setScene(palScene);
    setGrayscalePalette();
    buildPaletteScene();

    connect(ui->headerButtonGroup, SIGNAL(buttonClicked(int)), this, SLOT(on_headerButtonGroup_clicked(int)));
    if (loadCompressionPlugins())
    {
        foreach(const QString& key, dataEngine.availableCompressions().keys())
        {
            ui->compressionComboBox->addItem(key);
        }
    } else {
        qApp->exit(1);
    }
    if (1) {
        openRom("D:\\Emulation\\Zelda - A Link to the Past\\Zelda - A Link to the Past.smc");
        loadPreset("D:\\Project\\SNESTilesKitten\\Presets\\The legend of Zelda - Link Sprites.stk");
    }
    m_settings = new QSettings("skarsnik.nyo.fr", "SNESTilesKitten");
    if (m_settings->contains("windowGeometry"))
    {
        lastPresetDirectory = m_settings->value("lastPresetDirectory").toString();
        lastROMDirectory = m_settings->value("lastROMDirectory").toString();
        lastPNGDirectory = m_settings->value("lastPNGDirectory").toString();
        restoreGeometry(m_settings->value("windowGeometry").toByteArray());
        restoreState(m_settings->value("windowState").toByteArray());
    }
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
    dataEngine.overrideHeaderInfo = true;
    dataEngine.overridenHeaderInfo = romHasHeader;
    rawTiles = dataEngine.extractTiles(currentSet);
    return !rawTiles.isEmpty();
}

void MainUI::createImageList()
{
    images.clear();
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

void MainUI::buildTileScene()
{
    tileScene->clear();
    int i = 0;
    int j = 0;
    tileScene->setBackgroundBrush(Qt::blue);
    foreach(QImage image, images)
    {
        QGraphicsPixmapItem *newPixItem = new QGraphicsPixmapItem(QPixmap().fromImage(image).scaled(tilesZoom, tilesZoom));
        newPixItem->setPos(i * newPixItem->boundingRect().width() + i, j * newPixItem->boundingRect().width() + j);
        tileScene->addItem(newPixItem);
        i++;
        if (i % tilesPerRow == 0) {
            j++;
            i = 0;
        }
    }
    tileScene->setSceneRect(tileScene->itemsBoundingRect());
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

void MainUI::setGrayscalePalette(unsigned int paletteSize)
{
    mPalette.clear();
    unsigned int psize = paletteSize;
    for (unsigned int i = 0; i < psize; i++)
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
    return dataEngine.loadCompressionPlugins(pluginsDir);
}

bool MainUI::extractPalette()
{
    mPalette = dataEngine.extractPalette(currentSet);
    return !mPalette.isEmpty();
}


void MainUI::closeEvent(QCloseEvent *event)
{
    m_settings->setValue("lastROMDirectory", lastROMDirectory);
    m_settings->setValue("lastPresetDirectory", lastPresetDirectory);
    m_settings->setValue("lastPNGDirectory", lastPNGDirectory);
    m_settings->setValue("windowState", saveState());
    m_settings->setValue("windowGeometry", saveGeometry());
}

void MainUI::on_romOpenButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
          tr("Select ROM"), lastROMDirectory, tr("SNES ROM (*.smc *.sfc)"));
    if (!fileName.isNull())
        openRom(fileName);
}


void MainUI::on_headerButtonGroup_clicked(int)
{
    romHasHeader = ui->headerRadioButton->isChecked();
}

void MainUI::updateUiWithPreset()
{
    ui->headerRadioButton->setChecked(romHasHeader);
    ui->loRomRadioButton->setChecked(currentSet.romType == "LoROM");
    if (currentSet.SNESTilesLocation != 0)
    {
        ui->tilesSNESRadioButton->toggle();
        ui->snesAddrLineEdit->setText(QString::number(currentSet.SNESTilesLocation, 16));       
    }
    else
    {
        ui->tilesPCRadioButton->toggle();
        ui->pcAddrLineEdit->setText(QString::number(currentSet.pcTilesLocation, 16));   
    }
    ui->sizeLineEdit->setText(QString::number(currentSet.length));
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
    ui->tilesPerRowSpinBox->setValue(currentSet.tilesPerRow);
    tilesPerRow = currentSet.tilesPerRow;
    if (currentSet.SNESPaletteLocation != 0)
    {
        ui->paletteSNESLocationRadioButton->toggle();
        ui->paletteSNESLocationLineEdit->setText(QString::number(currentSet.SNESPaletteLocation, 16));
    }
    if (currentSet.pcPaletteLocation != 0)
    {
        ui->palettePCLocationRadioButton->toggle();
        ui->palettePCLocationLineEdit->setText(QString::number(currentSet.pcPaletteLocation, 16));
    }
    ui->paletteNoZeroColorRadioButton->setChecked(currentSet.paletteNoZeroColor);
}

void MainUI::updateGTileView()
{
    if (currentSet.SNESPaletteLocation == 0 && currentSet.pcPaletteLocation == 0)
        setGrayscalePalette(qPow(2, currentSet.bpp));
    else
        extractPalette();
    if (!extractTiles()) {
        ui->statusBar->showMessage("Error extracting tiles");
        return;
    }
    createImageList();
    buildTileScene();
    buildPaletteScene();
}

void MainUI::on_paletteSNESLocationRadioButton_toggled(bool checked)
{
    ui->paletteSNESLocationLineEdit->setEnabled(checked);
}

void MainUI::on_palettePCLocationRadioButton_toggled(bool checked)
{
    ui->palettePCLocationLineEdit->setEnabled(checked);
}


void MainUI::on_refreshPushButton_clicked()
{
    updatePresetWithUi();
    updateGTileView();
}

void MainUI::on_paletteGrayRadioButton_toggled(bool checked)
{
    if (checked == true)
    {
        currentSet.SNESPaletteLocation = 0;
        currentSet.pcPaletteLocation = 0;
        ui->paletteNoZeroColorRadioButton->setEnabled(false);
    }
    else
        ui->paletteNoZeroColorRadioButton->setEnabled(true);
}

void MainUI::on_tilesSNESRadioButton_toggled(bool checked)
{
    ui->snesAddrLineEdit->setEnabled(checked);
}

void MainUI::on_tilesPCRadioButton_toggled(bool checked)
{
    ui->pcAddrLineEdit->setEnabled(checked);
}

void MainUI::openRom(QString rom)
{
    dataEngine.setRomFile(rom);
    ROMInfo& romInfo = dataEngine.romInfo;
    romHasHeader = romInfo.hasHeader;
    if (romHasHeader)
        ui->headerRadioButton->toggle();
    else
        ui->noHeaderRadioButton->toggle();
    if (romInfo.romType == "LoROM")
        ui->loRomRadioButton->toggle();
    else
        ui->hiRomRadioButton->toggle();
    ui->romFileLineEdit->setText(rom);
    ui->statusBar->showMessage(QFileInfo(rom).baseName() + " loaded - " + romInfo.romType + " - Rom title is : " + romInfo.romTitle);
    romFile = rom;
    lastROMDirectory = QFileInfo(rom).dir().absolutePath();

}

bool MainUI::loadPreset(const QString& presetFile)
{
    if (currentSet.load(presetFile)) {
        updateUiWithPreset();
        lastPresetDirectory = QFileInfo(presetFile).dir().absolutePath();
        ui->statusBar->showMessage("Preset file " + QFileInfo(presetFile).fileName() + " loaded - " + currentSet.name);
        if (!romFile.isEmpty())
            updateGTileView();
        return true;
    } else {
        qDebug() << "Error loading preset";
        return false;
    }
}

/* We use only thing that affect the set, rom header is not really important here for example */
/* Tile arrangement is not updated here */

void MainUI::updatePresetWithUi()
{
    currentSet.SNESTilesLocation = 0;
    currentSet.pcTilesLocation = 0;
    bool ok;
    if (ui->tilesSNESRadioButton->isChecked())
        currentSet.SNESTilesLocation = ui->snesAddrLineEdit->text().toUInt(&ok, 16);
    else
        currentSet.pcTilesLocation = ui->pcAddrLineEdit->text().toUInt(&ok, 16);
    currentSet.length = ui->sizeLineEdit->text().toUInt();
    if (ui->bpp2RadioButton->isChecked())
        currentSet.bpp = 2;
    if (ui->bpp3RadioButton->isChecked())
        currentSet.bpp = 3;
    if (ui->bpp4RadioButton->isChecked())
        currentSet.bpp = 4;

    currentSet.compression = ui->compressionComboBox->currentText();

    currentSet.SNESPaletteLocation = 0;
    currentSet.pcPaletteLocation = 0;
    if (ui->paletteSNESLocationRadioButton->isChecked())
        currentSet.SNESPaletteLocation = ui->paletteSNESLocationLineEdit->text().toUInt(&ok, 16);
    if (ui->palettePCLocationRadioButton->isChecked())
        currentSet.pcPaletteLocation = ui->palettePCLocationLineEdit->text().toUInt(&ok, 16);
    if (ui->paletteNoZeroColorRadioButton->isEnabled())
        currentSet.paletteNoZeroColor = ui->paletteNoZeroColorRadioButton->isChecked();
}

void MainUI::on_tilesPerRowSpinBox_valueChanged(int arg1)
{
    tilesPerRow = arg1;
}

void MainUI::on_tileZoomHorizontalSlider_valueChanged(int value)
{
    tilesZoom = value;
    buildTileScene();
}

void MainUI::on_tilesPerRowSpinBox_editingFinished()
{
    tilesPerRow = ui->tilesPerRowSpinBox->value();
    buildTileScene();
}

void MainUI::on_presetOpenPushButton_clicked()
{
    QString presetFile = QFileDialog::getOpenFileName(this,
                                tr("Select preset"), lastPresetDirectory, tr("stk (*.stk)"));
    if (!presetFile.isEmpty())
    {
        loadPreset(presetFile);
    }
}

void MainUI::on_presetSavePushButton_clicked()
{
    bool    ok;
    currentSet.tilesPerRow = tilesPerRow;
    QString presetName;
    if (!currentSet.name.isEmpty())
        presetName = currentSet.name;
    else
        presetName = dataEngine.romInfo.romTitle;
    presetName = QInputDialog::getText(this, tr("Preset name"), tr("Preset name"), QLineEdit::Normal, presetName, &ok);
    if (ok) {
        QString presetFile = QFileDialog::getSaveFileName(this, tr("Preset file"), lastPresetDirectory + "/" + presetName + ".stk", tr("stk (*.stk)"));
        if (!presetFile.isEmpty()) {
            currentSet.name = presetName;
            if (currentSet.save(presetFile)) {
                lastPresetDirectory = QFileInfo(presetFile).dir().absolutePath();
                ui->statusBar->showMessage("Preset file saved properly");
            } else {
                ui->statusBar->showMessage("Error saving preset");
                qDebug() << "Error saving preset";
            }
        }
    }
}


void MainUI::on_pngExportPushButton_clicked()
{
    unsigned int piko = currentSet.pcTilesLocation == 0 ? currentSet.SNESTilesLocation : currentSet.pcPaletteLocation;
    QString exportName = dataEngine.romInfo.romTitle + "_" + QString::number(piko, 16);
    if (!currentSet.name.isEmpty())
        exportName = currentSet.name;
    QString pngFile = QFileDialog::getSaveFileName(this, tr("Exported PNG File"), lastPNGDirectory + "/" + exportName + ".png", tr("PNG file, (*.png)"));
    if (!pngFile.isEmpty())
    {
        QImage img = mergeTilesToImage(rawTiles, mPalette, tilesPerRow);
        if (saveToPNG(img, pngFile))
            ui->statusBar->showMessage("Export to " + pngFile + " succesfull");
        else
            ui->statusBar->showMessage("Error while exporting to " + pngFile);
    }
}
