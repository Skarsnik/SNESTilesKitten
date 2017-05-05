#include "mainui.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    // Windows hack to have output in the console
    QApplication a(argc, argv);

    QApplication::setApplicationName("SNESTilesKitten");
    QApplication::setApplicationVersion(QString(__DATE__));

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

    MainUI w;
    w.show();
    return a.exec();
}
