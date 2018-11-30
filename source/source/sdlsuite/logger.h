#ifndef LOGGER_H
#define LOGGER_H

#include <QString>
#include <QFile>
#include <QTextStream>

#define USB_LOG_FILE_RPI "/media/usbhd-sda1/logs.txt"
#define USB_LOG_FILE_PC "/home/sachin/Desktop/logs.txt"

enum MACHINE
{
    MACHINE_PC,
    MACHINE_RPI
};


class Logger
{
    static Logger *s_instance;
    bool filelog;
    QFile *file;
    MACHINE machine;

private:
    Logger();
    bool fileExists(QString path);

    QTextStream& qFileOut()
    {
        static QTextStream ts2( file );
        return ts2;
    }

public:
    static Logger *getInstance()
    {
        if (!s_instance)
          s_instance = new Logger();
        return s_instance;
    }

    void outputMessage( QtMsgType type, const QString &msg );
};

#endif // LOGGER_H
