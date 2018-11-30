#ifndef NETWORKWORKER_H
#define NETWORKWORKER_H

#include <QObject>
#include <QtNetwork/QNetworkInterface>
#include <QNetworkConfigurationManager>
#include <QNetworkSession>
#include <iostream>
#include "datatypes.h"

#define NET_POLL_DELAY 1250

using namespace std;

class NetworkWorker : public QObject {
    Q_OBJECT

public:
    NetworkWorker();

public slots:
    void process();

signals:
    void finished();
    void error(QString err);
    void update(bool network);
    void pirated(QString msg);

private:
    bool isConnectedToNetwork();
    QString getMacAddress();

private:
    bool license;
};

#endif // NETWORKWORKER_H
