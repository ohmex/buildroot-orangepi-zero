#ifndef SERIALPORTMANAGER_H
#define SERIALPORTMANAGER_H

#include <QtCore/QtGlobal>
#include <QObject>
#include <QtSerialPort/QSerialPort>

class SerialPortManager : public QObject
{
    Q_OBJECT
public:
    explicit SerialPortManager(QObject *parent = 0);
    ~SerialPortManager();

public slots:
    void openSerialPort();
    void closeSerialPort();

private slots:
    void writeData(const QByteArray &data);
    void readData();
    void handleError(QSerialPort::SerialPortError error);

signals:
    void serialdata(QByteArray);

private:
    QSerialPort *serial;
};

#endif // SERIALPORTMANAGER_H
