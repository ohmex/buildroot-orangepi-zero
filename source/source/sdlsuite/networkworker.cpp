#include <QThread>
#include <QCoreApplication>
#include "networkworker.h"
#include "mac.h"

/*
NetworkWorker::NetworkWorker()
{
    license = true;

    if( MAC_CHECK_ENABLE )
    if( QString::compare(getMacAddress(), new QString(MAC0), Qt::CaseInsensitive) &&
        QString::compare(getMacAddress(), new QString(MAC1), Qt::CaseInsensitive) &&
        QString::compare(getMacAddress(), new QString(MAC2), Qt::CaseInsensitive) ) {
        license = false;
    }
}
*/
NetworkWorker::NetworkWorker()
{
    license = false;

    std::vector<std::string> macs = {MACS};

    if( MAC_CHECK_ENABLE )
    for (std::vector<std::string>::iterator it = macs.begin() ; it != macs.end(); ++it) {

        if( !QString::compare(getMacAddress(), QString::fromStdString(*it), Qt::CaseInsensitive) ) {
            license = true;
            //qDebug() << "FOUND"<<getMacAddress();
        }
    }
}

QString NetworkWorker::getMacAddress()
{
    foreach(QNetworkInterface netInterface, QNetworkInterface::allInterfaces())
    {
        // Return only the first non-loopback MAC Address
        if (!(netInterface.flags() & QNetworkInterface::IsLoopBack) && !QString::compare(netInterface.humanReadableName(), "eth0", Qt::CaseInsensitive))
            return netInterface.hardwareAddress();
    }
    return QString();
}

bool NetworkWorker::isConnectedToNetwork(){
    bool result = false;

    const QString channelName ("eth0");
    QNetworkConfigurationManager *nwManager = new QNetworkConfigurationManager (this);
    QList<QNetworkConfiguration> configurations = nwManager->allConfigurations ();
    while (!configurations.isEmpty()) {
        QNetworkConfiguration configuration = configurations.takeFirst();

        if (configuration.name () != channelName) {
            continue;
        }

        QNetworkSession *session = new QNetworkSession (configuration, this);

        session->open ();
        session->waitForOpened ();
        result = session->isOpen ();
        delete session;

        break;
    }

    return result;
}

void NetworkWorker::process() {

    bool network;

    while(true) {

        QCoreApplication::processEvents();

        if(license) {
            network = isConnectedToNetwork();
            emit update(network);
        } else {
            qDebug() << "Pirated:" << getMacAddress();
            emit pirated(getMacAddress());
        }

        QThread::msleep(NET_POLL_DELAY);
    }

    emit finished();
}
