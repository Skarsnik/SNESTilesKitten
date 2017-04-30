#include "mainui.h"
#include "pngexportimport.h"
#include <QApplication>
#include <QCommandLineParser>
#include <stdio.h>
#ifdef Q_OS_WIN
#include <windows.h>
#endif

#define tr(...) QCoreApplication::translate(__VA_ARGS__)


bool    workingModeExport = true;
bool    pngFile = true;
QString externalFile;
QString presetFile;
QString romFile;
bool    romCopy = false;
#include <QDebug>

bool    processArgument(QApplication&, QCommandLineParser& parser);
void    doStuff();
void    showRomInfo(const ROMInfo& romInfo);

int main(int argc, char *argv[])
{
    // Windows hack to have output in the console
    QApplication a(argc, argv);

    QApplication::setApplicationName("SNESTilesKitten");
    QApplication::setApplicationVersion(QString(__DATE__));

#ifdef Q_OS_WIN
    if (a.arguments().size() > 1)
    {
        /*AllocConsole();*/

        AttachConsole(GetCurrentProcessId());
        freopen("CON", "w", stdout);
        freopen("CON", "w", stderr);
        freopen("CON", "r", stdin);
    }
#endif

    QDir pluginsDir(qApp->applicationDirPath());
//TODO Linux handling
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
    ROMDataEngine::loadCompressionPlugins(pluginsDir);
    if (a.arguments().size() > 1)
    {
          QCommandLineParser parser;
          bool ok = processArgument(a, parser);
          if (ok)
          {
                doStuff();
          }
    } else {
        MainUI w;
        w.show();
        return a.exec();
    }
    return 0;
}

bool    processArgument(QApplication &app, QCommandLineParser& parser)
{
    parser.setApplicationDescription("A SNES tiles extractor/injector");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("preset", tr("option", "The preset to use"));

    QCommandLineOption pngOption("png", tr("option", "Work with PNG file (export/import), default"));
    QCommandLineOption rawOption("raw", tr("option", "Work with RAW file (export/import)"));
    QCommandLineOption copyOption("rawcopy", tr("option", "Do a copy of the data as raw snes tile"), "filename");
    QCommandLineOption romCopyOption("copy", tr("option", "Work on a copy of the ROM when applying change"));
    parser.addOption(pngOption);
    parser.addOption(rawOption);
    parser.addOption(copyOption);
    parser.addOption(romCopyOption);

    QCommandLineOption outputOption(QStringList() << "o" << "output", tr("option", "The file to output the tiles"), tr("option", "filename"));
    QCommandLineOption inputOption(QStringList() << "i" << "input", tr("option", "The file used as input for injection"), tr("option", "filename"));
    parser.addOption(outputOption);
    parser.addOption(inputOption);
    QCommandLineOption exportOption("extract", tr("option", "Extract mode"));
    QCommandLineOption importOption("inject", tr("option", "Inject mode"));
    parser.addOption(exportOption);
    parser.addOption(importOption);

    QCommandLineOption romfileOption("rom", tr("option", "SNES ROM file to use"), "nope");
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
        QImage ImgExport = mergeTilesToImage(rawTiles, mPalette, preset.tilesPerRow);
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
        fprintf(stdout, "Number of tile extracted from the PNG : %d\n", rawTiles.size());
        QVector<QRgb> mPalette = paletteFromPNG(externalFile);
        /*
        QImage pikoNg = mergeTilesToImage(rawTiles, mPalette, preset.tilesPerRow);
        saveToPNG(pikoNg, "piko.png");*/
        fprintf(stdout, "Preset file says lenght is : %d\n", preset.length);
        if (!dataEngine.injectTiles(rawTiles, preset))
        {
            fprintf(stderr, "Error while injecting tile\n");
            return ;
        }
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
