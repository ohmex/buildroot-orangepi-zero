#include <QtCore/QCoreApplication>
#include <QtCore/QCommandLineParser>
#include <QtCore/QCommandLineOption>
#include <QThread>
#include <QMutex>
#include <QMutexLocker>
#include "serialserver.h"
#include "networkworker.h"
#include "settings.h"
#include "logger.h"

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString & str)
{
    static QMutex mutex;
    QMutexLocker lock(&mutex);
    Logger::getInstance()->outputMessage( type, str );
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Settings *settings = Settings::getInstance();
    settings->readSettings();

    qInstallMessageHandler(myMessageOutput);

    QCommandLineParser parser;
    parser.setApplicationDescription("SerialServer");
    parser.addHelpOption();

    QCommandLineOption dbgOption(QStringList() << "d" << "debug",
            QCoreApplication::translate("main", "Debug output [default: off]."));
    parser.addOption(dbgOption);
    QCommandLineOption portOption(QStringList() << "p" << "port",
            QCoreApplication::translate("main", "Port for serialserver [default: 8086]."),
            QCoreApplication::translate("main", "port"), QLatin1Literal("8086"));
    parser.addOption(portOption);
    parser.process(a);
    bool debug = parser.isSet(dbgOption);
    int port = parser.value(portOption).toInt();

    QThread* nthread = new QThread;
    NetworkWorker* nworker = new NetworkWorker();
    nworker->moveToThread(nthread);
    QObject::connect(nthread, SIGNAL(started()), nworker, SLOT(process()));
    QObject::connect(nworker, SIGNAL(finished()), nthread, SLOT(quit()));
    QObject::connect(nworker, SIGNAL(finished()), nworker, SLOT(deleteLater()));
    QObject::connect(nthread, SIGNAL(finished()), nthread, SLOT(deleteLater()));

    //QObject::connect(nworker, SIGNAL(update(bool)), dbworker, SLOT(update(bool)));
    //QObject::connect(nworker, SIGNAL(pirated(QString)), mainwindow, SLOT(pirated(QString)));

    nthread->start();

    SerialServer *server = new SerialServer(port, debug);
    QObject::connect(server, &SerialServer::closed, &a, &QCoreApplication::quit);

    return a.exec();
}
