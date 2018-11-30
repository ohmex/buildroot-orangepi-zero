#include "serialserver.h"
#include <QtCore/QDebug>
#include <QNetworkInterface>

QT_USE_NAMESPACE

SerialServer::SerialServer(quint16 port, bool debug, QObject *parent) :
    QObject(parent),
    m_pWebSocketServer(new QWebSocketServer(QStringLiteral("SerialServer"), QWebSocketServer::NonSecureMode, this)),
    m_debug(debug)
{
    if (m_pWebSocketServer->listen(QHostAddress::Any, port)) {
        if (m_debug) qDebug() << "SerialServer listening on port" << port;
        connect(m_pWebSocketServer, &QWebSocketServer::newConnection, this, &SerialServer::onNewConnection);
        connect(m_pWebSocketServer, &QWebSocketServer::closed, this, &SerialServer::closed);
    }

    isPirated = false;

    spm = new SerialPortManager();
    connect(spm, SIGNAL(serialdata(QByteArray)), this, SLOT(onSerialData(QByteArray)));
    connect(this, SIGNAL(socketdata(QByteArray)), spm, SLOT(writeData(QByteArray)));

    foreach (const QHostAddress &address, QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress(QHostAddress::LocalHost)) {
             qDebug() << address.toString();
             ip = address.toString();
        }
    }

    spm->openSerialPort();

    connect(&mTimer, SIGNAL(timeout()), this, SLOT(onTimeout()));
    mTimer.setInterval(100);
    //mTimer.start();
}

void SerialServer::onTimeout()
{
    static unsigned int count = 0;
    Q_FOREACH (QWebSocket *pClient, m_clients) {
        if (pClient) {
            if (m_debug) {
                QString msg =  "Message counter = " + QString::number( count++ );
                pClient->sendTextMessage(msg);
            }
        }
    }
}

SerialServer::~SerialServer()
{
    m_pWebSocketServer->close();
    qDeleteAll(m_clients.begin(), m_clients.end());
}

void SerialServer::onNewConnection()
{
    QWebSocket *pSocket = m_pWebSocketServer->nextPendingConnection();

    connect(pSocket, &QWebSocket::textMessageReceived, this, &SerialServer::processTextMessage);
    connect(pSocket, &QWebSocket::binaryMessageReceived, this, &SerialServer::processBinaryMessage);
    connect(pSocket, &QWebSocket::disconnected, this, &SerialServer::socketDisconnected);

    m_clients << pSocket;
}

void SerialServer::onSerialData(QByteArray data)
{
    if (m_debug) qDebug() << "UART: " << data;

    Q_FOREACH (QWebSocket *pClient, m_clients) {
        if (pClient) {
            if(isPirated)
                pClient->sendTextMessage("PIRATED");
            else
                pClient->sendTextMessage(ip + "," + data);
        }
    }
}

void SerialServer::processTextMessage(QString message)
{
    if (m_debug) qDebug() << "TCP TXT: " << message;

    command = message;

    if(isPirated)
        emit socketdata("PIRATED");
    else
        emit socketdata(message.toUtf8());
}

void SerialServer::processBinaryMessage(QByteArray message)
{
    if (m_debug) qDebug() << "TCP BIN: " << message;

    command = message;

    if(isPirated)
        emit socketdata("PIRATED");
    else
        emit socketdata(message);
}

void SerialServer::update(bool connected)
{

}

void SerialServer::pirated(QString mac)
{
    isPirated = true;
}

void SerialServer::socketDisconnected()
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (m_debug) qDebug() << "Socket Disconnected:" << pClient;
    if (pClient) {
        m_clients.removeAll(pClient);
        pClient->deleteLater();
    }
}
