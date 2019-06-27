#include "compressioninterface.h"
#include "graphicspalettecoloritem.h"
#include "graphicstileitem.h"
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
#include "injectdialog.h"


MainUI::MainUI(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainUI)
{
    ui->setupUi(this);
    ui->presetOpenPushButton->setIcon(style()->standardPixmap(QStyle::SP_DialogOpenButton));
    ui->presetSavePushButton->setIcon(style()->standardPixmap(QStyle::SP_DialogSaveButton));
    QRegExpValidator* hexValid = new QRegExpValidator(QRegExp("[0-F|a-f]{0,8}"));
    QIntValidator* sizeValid = new QIntValidator(0, 1024 * 1024 * 1024);
    romHasHeader = true;

    ui->snesAddrLineEdit->setValidator(hexValid);
    ui->pcAddrLineEdit->setValidator(hexValid);
    ui->palettePCLocationLineEdit->setValidator(hexValid);
    ui->paletteSNESLocationLineEdit->setValidator(hexValid);
    ui->sizeLineEdit->setValidator(sizeValid);
    tilesPerRow = 8;

    tileScene = new GraphicsTilesScene();
    palScene = new QGraphicsScene();
    ui->graphicsView->setScene(tileScene);
    ui->paletteGraphicsView->setScene(palScene);
    setGrayscalePalette();
    buildPaletteScene();

    connect(ui->headerButtonGroup, SIGNAL(buttonClicked(int)), this, SLOT(on_headerButtonGroup_clicked(int)));
    connect(ui->graphicsView, SIGNAL(tileUnderCursorChanged()), this, SLOT(onTileUnderCursorChanged()));
    compressInfos = ROMDataEngine::compressionInfos();
    unsigned int cpt = 0;
    ui->compressionComboBox->insertItem(cpt, "None");
    ui->compressionComboBox->setItemData(cpt, "No compression", Qt::ToolTipRole);
    cpt++;
    foreach(const QString& key, ROMDataEngine::availableCompressions().keys())
    {
            ui->compressionComboBox->insertItem(cpt, key);
            ui->compressionComboBox->setItemData(cpt, compressInfos[key].description, Qt::ToolTipRole);
            cpt++;
    }
    QMap<QString, TilesPattern>  patterns = TilesPattern::Patterns();
    cpt = 0;
    foreach (const QString& key, patterns.keys()) {
        ui->tilePatternComboBox->insertItem(cpt, key);
        ui->tilePatternComboBox->setItemData(cpt, patterns[key].description, Qt::ToolTipRole);
        cpt++;
    }
    ui->tilePatternComboBox->setCurrentText("normal");
    /*if (1) {
        openRom("D:\\Emulation\\Zelda - A Link to the Past\\Zelda - A Link to the Past.smc");
        loadPreset("D:\\Project\\SNESTilesKitten\\Presets\\The Legend of Zelda - Link Sprites.stk");
    }*/
    m_settings = new QSettings("skarsnik.nyo.fr", "SNESTilesKitten");
    if (m_settings->contains("windowGeometry"))
    {
        lastPresetDirectory = m_settings->value("lastPresetDirectory").toString();
        lastROMDirectory = m_settings->value("lastROMDirectory").toString();
        lastPNGDirectory = m_settings->value("lastPNGDirectory").toString();
        restoreGeometry(m_settings->value("windowGeometry").toByteArray());
        restoreState(m_settings->value("windowState").toByteArray());
    }
    setWindowTitle(qApp->applicationName() + " " + qApp->applicationVersion());
    dataEngine.overrideHeaderInfo = true;
}

MainUI::~MainUI()
{
    delete ui;
}

void MainUI::onTileUnderCursorChanged()
{
    //qDebug() << "Tile under cursor : " << ui->graphicsView->tileUnderCursor();
    tile8 tile = ui->graphicsView->tileUnderCursor()->rawTile;
    int pcStart;
    if (currentSet.pcTilesLocation > 0)
        pcStart = currentSet.pcTilesLocation;
    else
        pcStart = ROMDataEngine::snesToPC(currentSet.SNESTilesLocation, currentSet.romType);
    int pcAddr = pcStart + tile.id * currentSet.bpp * 8;
    int romAddr = ROMDataEngine::pcToSnes(pcAddr, currentSet.romType);
    ui->tileInfos->setTile(ui->graphicsView->tileUnderCursor()->image, tile.id, pcAddr, romAddr, tile.id * currentSet.bpp * 8);
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
    dataEngine.overridenHeaderInfo = romHasHeader;
    rawTiles = dataEngine.extractTiles(currentSet);
    if (currentSet.compression != "None" && dataEngine.lastUnCompressSize != 0)
    {
        ui->sizeLineEdit->setText(QString::number(dataEngine.lastUnCompressSize));
        ui->statusBar->showMessage("Tiles presetset size get ajusted to the compressed string size");
    }
    return !rawTiles.isEmpty();
}

bool MainUI::extractPalette()
{
    dataEngine.overridenHeaderInfo = romHasHeader;
    mPalette = dataEngine.extractPalette(currentSet);
    return !mPalette.isEmpty();
}


void MainUI::buildTileScene()
{
   tileScene->buildScene(rawTiles, mPalette, currentSet.tilesPattern);
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

void MainUI::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event)
    m_settings->setValue("lastROMDirectory", lastROMDirectory);
    m_settings->setValue("lastPresetDirectory", lastPresetDirectory);
    m_settings->setValue("lastPNGDirectory", lastPNGDirectory);
    m_settings->setValue("windowState", saveState());
    m_settings->setValue("windowGeometry", saveGeometry());
}

void MainUI::on_romOpenButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
          tr("Select ROM"), lastROMDirectory, tr("SNES ROM or associated (*.smc *.sfc *.gfx *.bin);;All files (*)"));
    if (!fileName.isNull())
        openRom(fileName);
}


void MainUI::on_headerButtonGroup_clicked(int)
{
    romHasHeader = ui->headerRadioButton->isChecked();
}

void MainUI::rebuildGTileView()
{
    if (currentSet.SNESPaletteLocation == 0 && currentSet.pcPaletteLocation == 0)
        setGrayscalePalette(qPow(2, currentSet.bpp));
    else
        extractPalette();
    if (!extractTiles()) {
        ui->statusBar->showMessage("Error extracting tiles");
        return;
    }
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
    rebuildGTileView();
}

void MainUI::on_paletteGrayRadioButton_toggled(bool checked)
{
    if (checked == true)
    {
        currentSet.SNESPaletteLocation = 0;
        currentSet.pcPaletteLocation = 0;
        ui->paletteNoZeroColorCheckBox->setEnabled(false);
    }
    else
        ui->paletteNoZeroColorCheckBox->setEnabled(true);
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
    currentSet.romType = romInfo.romType;
    ui->romFileLineEdit->setText(rom);
    ui->statusBar->showMessage(QFileInfo(rom).baseName() + " loaded - " + romInfo.romType + " - Rom title is : " + romInfo.romTitle);
    romFile = rom;
    QIntValidator* intVal = (QIntValidator*) ui->sizeLineEdit->validator();
    intVal->setTop(romInfo.size);
    lastROMDirectory = QFileInfo(rom).dir().absolutePath();

}

bool MainUI::loadPreset(const QString& presetFile)
{
    if (currentSet.load(presetFile)) {
        updateUiWithPreset();
        lastPresetDirectory = QFileInfo(presetFile).dir().absolutePath();
        ui->statusBar->showMessage("Preset file " + QFileInfo(presetFile).fileName() + " loaded - " + currentSet.name);
        if (!romFile.isEmpty())
            rebuildGTileView();
        return true;
    } else {
        qDebug() << "Error loading preset";
        return false;
    }
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
        case 1:
        {
            ui->bpp1RadioButton->setChecked(true);
            break;
        }
        case 2:
        {
            ui->bpp2RadioButton->setChecked(true);
            break;
        }
        case 3:
        {
            ui->bpp3RadioButton->setChecked(true);
            break;
        }
        case 4:
            ui->bpp4RadioButton->setChecked(true);
    }
    ui->tilePatternComboBox->setCurrentText(currentSet.tilesPattern.name);
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
    ui->paletteNoZeroColorCheckBox->setChecked(currentSet.paletteNoZeroColor);
    if (currentSet.pcPaletteLocation == 0 && currentSet.SNESPaletteLocation == 0)
    {
        ui->palettePCLocationLineEdit->clear();
        ui->paletteSNESLocationLineEdit->clear();
        ui->paletteGrayRadioButton->toggle();
        ui->paletteNoZeroColorCheckBox->setEnabled(false);
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
    if (ui->bpp1RadioButton->isChecked())
        currentSet.bpp = 1;
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
    if (ui->paletteNoZeroColorCheckBox->isEnabled())
        currentSet.paletteNoZeroColor = ui->paletteNoZeroColorCheckBox->isChecked();
}

void MainUI::on_tilesPerRowSpinBox_valueChanged(int arg1)
{
    tilesPerRow = arg1;
}

void MainUI::on_tileZoomHorizontalSlider_valueChanged(int value)
{
    tileScene->setTilesZoom(value);
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
    QString presetName;
    if (!currentSet.name.isEmpty())
        presetName = currentSet.name;
    else
        presetName = dataEngine.romInfo.romTitle;
    presetName = QInputDialog::getText(this, tr("Preset name"), tr("Preset name"), QLineEdit::Normal, presetName, &ok);
    if (ok) {
        QString presetFile = QFileDialog::getSaveFileName(this, tr("Preset file"), lastPresetDirectory + "/" + presetName + ".stk", tr("stk (*.stk)"));
        if (!presetFile.isEmpty()) {
            updatePresetWithUi();
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
    unsigned int piko = currentSet.pcTilesLocation == 0 ? currentSet.SNESTilesLocation : currentSet.pcTilesLocation;
    QString exportName = dataEngine.romInfo.romTitle + "_" + QString::number(piko, 16);
    if (!currentSet.name.isEmpty())
        exportName = currentSet.name;
    QString pngFile = QFileDialog::getSaveFileName(this, tr("Exported PNG File"), lastPNGDirectory + "/" + exportName + ".png", tr("PNG file, (*.png)"));
    if (!pngFile.isEmpty())
    {
        QImage img = mergeTilesToImage(rawTiles, mPalette, currentSet.tilesPattern);
        if (saveToPNG(img, pngFile))
        {
            lastPNGDirectory = QFileInfo(pngFile).dir().absolutePath();
            ui->statusBar->showMessage("Export to " + pngFile + " succesfull");
        }
        else
            ui->statusBar->showMessage("Error while exporting to " + pngFile);
    }
}

void MainUI::on_pngImportpushButton_clicked()
{
    QString pngFile = QFileDialog::getOpenFileName(this,
                                tr("Select PNG image to inject"), lastPNGDirectory, tr("png (*.png)"));
    if (!pngFile.isEmpty())
    {
        InjectDialog iDiag(this, pngFile, mPalette);
        QFileInfo fi(romFile);
        QString copy = fi.absolutePath() + "/" + fi.baseName() + "_copy." + fi.completeSuffix();
        iDiag.setCopyRomString(fi.baseName() + "_copy." + fi.completeSuffix());
        if (iDiag.exec())
        {
            if (iDiag.copyRom)
            {
                if (QFileInfo::exists(copy))
                {
                    if (!QFile::remove(copy))
                    {
                        qCritical() << "Can't remove rom copy " << copy;
                        return ;
                    }
                    if (QFile::copy(romFile, copy) == false)
                    {
                        qCritical() << "Can't create a copy of the rom file\n";
                        return ;
                    }
                }
                qDebug() << "Set rom copy";
                dataEngine.setRomFile(copy);
            }
            qDebug() << "Importing stuff";
            updatePresetWithUi();
            QList<tile8>    importedRawTiles = tilesFromPNG(pngFile);
            qDebug("Imported %d tiles", importedRawTiles.size());
            importedRawTiles = TilesPattern::reverse(currentSet.tilesPattern, importedRawTiles);
            qDebug("Transformed to %d tiles", importedRawTiles.size());
            dataEngine.injectTiles(importedRawTiles, currentSet);
            if ((currentSet.pcPaletteLocation != 0 || currentSet.SNESPaletteLocation != 0) && iDiag.useImagePalette)
            {
                QVector<QRgb> ImportedPalette = paletteFromPNG(pngFile);
                dataEngine.injectPalette(ImportedPalette, currentSet);
                mPalette = ImportedPalette;
            }
            if (iDiag.copyRom)
            {
                dataEngine.setRomFile(romFile);
                ui->statusBar->showMessage("Import successfull - Displaying tiles of the copy");
            } else {
                ui->statusBar->showMessage("Import successfull");
            }
            rawTiles = importedRawTiles;
            buildTileScene();
        }
    }
}

void MainUI::on_compressionComboBox_currentIndexChanged(const QString &arg1)
{
    if (arg1 == "None" || compressInfos[arg1].canCompress)
        ui->pngImportpushButton->setEnabled(true);
    else
        ui->pngImportpushButton->setEnabled(false);

}

void MainUI::on_tilePatternComboBox_currentIndexChanged(const QString &arg1)
{
    currentSet.tilesPattern = TilesPattern::pattern(arg1);
    if (!rawTiles.isEmpty())
        tileScene->buildScene(rawTiles, mPalette, currentSet.tilesPattern);
}
