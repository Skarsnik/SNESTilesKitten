#include <QCoreApplication>
#include <QCommandLineParser>
#include <stdio.h>
#include "pngexportimport.h"
#include "myrominfo.h"
#include "romdataengine.h"

bool    workingModeExport = true;
bool    pngFile = true;
QString externalFile;
QString presetFile;
QString romFile;
bool    romCopy = false;
#include <QDebug>

bool    processArgument(QCoreApplication &);
void    doStuff();
void    showRomInfo(const ROMInfo& romInfo);



int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QCoreApplication::setApplicationName("SNESTinyKitten");
    QCoreApplication::setApplicationVersion(QString(__DATE__));

    if (processArgument(a))
    {
        doStuff();
    } else {
        return 1;
    }
}

bool    processArgument(QCoreApplication &app)
{
    QCommandLineParser parser;
    parser.setApplicationDescription("A SNES tiles extractor/injector");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("preset", QCoreApplication::translate("option", "The preset to use"));

    QCommandLineOption pngOption("png", QCoreApplication::translate("option", "Work with PNG file (export/import), default"));
    QCommandLineOption rawOption("raw", QCoreApplication::translate("option", "Work with RAW file (export/import)"));
    QCommandLineOption copyOption("rawcopy", QCoreApplication::translate("option", "Do a copy of the data as raw snes tile"), "filename");
    QCommandLineOption romCopyOption("copy", QCoreApplication::translate("option", "Work on a copy of the ROM when applying change"));
    parser.addOption(pngOption);
    parser.addOption(rawOption);
    parser.addOption(copyOption);
    parser.addOption(romCopyOption);

    QCommandLineOption outputOption(QStringList() << "o" << "output", QCoreApplication::translate("option", "The file to output the tiles"), QCoreApplication::translate("option", "filename"));
    QCommandLineOption inputOption(QStringList() << "i" << "input", QCoreApplication::translate("option", "The file used as input for injection"), QCoreApplication::translate("option", "filename"));
    parser.addOption(outputOption);
    parser.addOption(inputOption);
    QCommandLineOption exportOption("extract", QCoreApplication::translate("option", "Extract mode"));
    QCommandLineOption importOption("inject", QCoreApplication::translate("option", "Inject mode"));
    parser.addOption(exportOption);
    parser.addOption(importOption);

    QCommandLineOption romfileOption("rom", QCoreApplication::translate("option", "SNES ROM file to use"), "romfile");
    parser.addOption(romfileOption);
    parser.process(app);

    // Check for invalid option
    if (parser.isSet(exportOption) && parser.isSet(importOption))
    {
        fprintf(stderr, "Can't set both export and import mode\n");
        return false;
    }
    if (parser.isSet(importOption))
        workingModeExport = false;
    if (parser.isSet(outputOption) && workingModeExport == false)
    {
        fprintf(stderr, "Can't use the output option in inject mode\n");
        return false;
    }
    if (parser.isSet(inputOption) && workingModeExport == true)
    {
        fprintf(stderr, "Can't use the input option when working in extract mode\n");
        return false;
    }
    if (parser.positionalArguments().empty())
    {
        fprintf(stderr, "You need to set a preset\n");
        return false;
    }
    if (!parser.isSet(romfileOption))
    {
        fprintf(stderr, "You need to set the ROM file\n");
        return false;
    }
    presetFile = parser.positionalArguments().first();
    romFile = parser.value(romfileOption);
    if (!QFileInfo::exists(romFile))
    {
        fprintf(stderr, "Can't find romfile %s\n", qPrintable(QFileInfo(romFile).absoluteFilePath()));
        return false;
    }
    romFile = QFileInfo(romFile).absoluteFilePath();
    romCopy = parser.isSet(romCopyOption);
    if (workingModeExport && parser.isSet(outputOption))
        externalFile = parser.value(outputOption);
    if (!workingModeExport && parser.isSet(inputOption))
        externalFile = parser.value(inputOption);
    if (!QFileInfo::exists(externalFile) && !workingModeExport)
    {
        fprintf(stderr, "Can't find the file used to inject data%s\n", qPrintable(QFileInfo(externalFile).absoluteFilePath()));
        return false;
    }
    return true;
}

void    extract();
void    inject();

void    doStuff()
{
    showRomInfo(ROMInfo(romFile));
    if (workingModeExport)
        extract();
    else
        inject();
}

void    showRomInfo(const ROMInfo& romInfo)
{
    if (romInfo.hasHeader)
        fprintf(stdout, "ROM has SMC header\n");
    else
        fprintf(stdout, "ROM does not have SMC header\n");
    fprintf(stdout, "ROM type is %s\n", qPrintable(romInfo.romType));
    fprintf(stdout, "ROM Tile : %s\n", qPrintable(romInfo.romTitle));
}

void    extract()
{
    fprintf(stdout, "=Extracting tiles\n=");
    TilePreset preset;
    ROMDataEngine dataEngine;
    dataEngine.setRomFile(romFile);
    preset.load(presetFile);
    QList<tile8> rawTiles = dataEngine.extractTiles(preset);
    fprintf(stdout, "%d tiles extracted\n", rawTiles.size());
    QVector<QRgb> mPalette = dataEngine.extractPalette(preset);
    if (pngFile)
    {
        QImage ImgExport = mergeTilesToImage(rawTiles, mPalette, preset.tilesPattern);
        fprintf(stdout, "Writing to %s\n", qPrintable(externalFile));
        saveToPNG(ImgExport, externalFile);
    }
}

void    inject()
{
    if (romCopy)
    {
        QFileInfo fi(romFile);
        QString copy = fi.absolutePath() + "/" + fi.baseName() + "_copy." + fi.completeSuffix();
        fprintf(stdout, "Doing a copy of %s to %s\n", qPrintable(romFile), qPrintable(copy));
        if (QFileInfo::exists(copy))
        {
            if (!QFile::remove(copy))
            {
                fprintf(stderr, "Can't remove rom copy (%s)\n", qPrintable(copy));
                return ;
            }
        }
        if (QFile::copy(romFile, copy) == false)
        {
            fprintf(stderr, "Can't create a copy of the rom file\n");
            return ;
        }
        romFile = copy;
    }
    fprintf(stdout, "Injecting data\n");
    TilePreset preset;
    ROMDataEngine dataEngine;
    dataEngine.setRomFile(romFile);
    preset.load(presetFile);
    QList<tile8> rawTiles;
    if (pngFile)
    {
        rawTiles = tilesFromPNG(externalFile);
        rawTiles = TilesPattern::reverse(preset.tilesPattern, rawTiles);
        fprintf(stdout, "Number of tile extracted from the PNG : %d\n", rawTiles.size());
        QVector<QRgb> mPalette = paletteFromPNG(externalFile);
        fprintf(stdout, "Preset file says lenght is : %d\n", preset.length);
        unsigned int injectedSize = dataEngine.injectTiles(rawTiles, preset);
        if (injectedSize == 0)
        {
            fprintf(stderr, "Error while injecting tile\n");
            return ;
        }
        fprintf(stdout, "%d bytes of data writen to the ROM.\n", injectedSize);
        if (preset.compression != "None")
        {
            fprintf(stdout, "Compressed data size is %d\n", dataEngine.lastCompressSize);
            if (preset.length < dataEngine.lastCompressSize)
                fprintf(stderr, "Error: The compressed lenght is superior to the lenght in the preset file\n");
        }
        if (preset.pcPaletteLocation != 0 || preset.SNESPaletteLocation != 0)
            dataEngine.injectPalette(mPalette, preset);
    }

}
