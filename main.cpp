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
#include <QDebug>

bool    processArgument(QApplication&, QCommandLineParser& parser);
void    doStuff();

int main(int argc, char *argv[])
{
#ifdef Q_OS_WIN
        /*AllocConsole();*/

        AttachConsole(GetCurrentProcessId());
        freopen("CON", "w", stdout);
        freopen("CON", "w", stderr);
        freopen("CON", "r", stdin);
#endif
    QApplication a(argc, argv);

    QApplication::setApplicationName("SNESTilesKitten");
    QApplication::setApplicationVersion(QString(__DATE__));

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
    QCommandLineOption copyOption("copy", tr("option", "Do a copy of the data as raw snes tile"), "filename");
    parser.addOption(pngOption);
    parser.addOption(rawOption);
    parser.addOption(copyOption);

    QCommandLineOption outputOption(QStringList() << "o" << "output", tr("option", "The file to output the tiles"), tr("option", "filename"));
    QCommandLineOption inputOption(QStringList() << "i" << "input", tr("option", "The file used as input for injection"), tr("option", "filename"));
    parser.addOption(outputOption);
    parser.addOption(inputOption);
    QCommandLineOption exportOption("extract", tr("option", "Export mode"));
    QCommandLineOption importOption("inject", tr("option", "import mode"));
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
        fprintf(stderr, "Can't use the output option in import mode\n");
        return false;
    }
    if (parser.isSet(inputOption) && workingModeExport == true)
    {
        fprintf(stderr, "Can't use the input option when working in export mode\n");
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
    if (workingModeExport)
        extract();
    else
        inject();
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
        ImgExport.save(externalFile, "PNG");
    }
}

void    inject()
{
    fprintf(stdout, "Injecting data\n");
}
