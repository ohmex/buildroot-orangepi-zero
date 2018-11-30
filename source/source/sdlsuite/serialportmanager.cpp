#include "serialportmanager.h"
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>

SerialPortManager::SerialPortManager(QObject *parent) :
    QObject(parent)
{
    serial = new QSerialPort(this);

    connect(serial, SIGNAL(error(QSerialPort::SerialPortError)), this, SLOT(handleError(QSerialPort::SerialPortError)));
    connect(serial, SIGNAL(readyRead()), this, SLOT(readData()));
}

SerialPortManager::~SerialPortManager()
{
}

void SerialPortManager::openSerialPort()
{
    serial->setPortName("ttyS1");
    serial->setBaudRate(QSerialPort::Baud9600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);
    if (serial->open(QIODevice::ReadWrite)) {
        qDebug() << "UART connected"<<serial;
    } else {
        qCritical() << "UART Port error:" << serial->errorString();
        qDebug() << tr("Open error");
    }
}

void SerialPortManager::closeSerialPort()
{
    serial->close();
    qDebug() << tr("UART Disconnected");
}

void SerialPortManager::writeData(const QByteArray &data)
{
    serial->write(data);
}

void SerialPortManager::readData()
{
    QByteArray data = serial->readAll();
    emit serialdata(data);
}

void SerialPortManager::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        qCritical() << "UART Port error:" << serial->errorString();
        closeSerialPort();
    }
}
