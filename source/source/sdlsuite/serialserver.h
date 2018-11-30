#ifndef SERIALSERVER_H
#define SERIALSERVER_H

#include "QtWebSockets/qwebsocketserver.h"
#include "QtWebSockets/qwebsocket.h"
#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QByteArray>
#include <QTimer>
#include <QTime>
#include "serialportmanager.h"

QT_FORWARD_DECLARE_CLASS(QWebSocketServer)
QT_FORWARD_DECLARE_CLASS(QWebSocket)

class SerialServer : public QObject
{
    Q_OBJECT
public:
    explicit SerialServer(quint16 port, bool debug = false, QObject *parent = Q_NULLPTR);
    ~SerialServer();

Q_SIGNALS:
    void closed();

private Q_SLOTS:
    void onNewConnection();
    void processTextMessage(QString message);
    void processBinaryMessage(QByteArray message);
    void socketDisconnected();

private:
    QWebSocketServer *m_pWebSocketServer;
    QList<QWebSocket *> m_clients;
    bool m_debug;
    QString ip;
    QString command;

signals:
    void socketdata(QByteArray);

private slots:
    void onTimeout();
    void onSerialData(QByteArray);
    void update(bool);
    void pirated(QString);

private:
    QTimer mTimer;
    SerialPortManager *spm;
    bool isPirated;
};

#endif //SERIALSERVER_H
