#include <QFileInfo>
#include <QDataStream>
#include <QTime>
#include "logger.h"
#include <iostream>
#include <sys/utsname.h>

Logger *Logger::s_instance = 0;

Logger::Logger() {
    QString logfile;
    filelog = false;
    struct utsname my_uname;
    uname(&my_uname);
    QString str = QString(my_uname.machine);
    if(str.contains("arm", Qt::CaseInsensitive)) {
        machine = MACHINE_RPI;
        logfile = QString(USB_LOG_FILE_RPI);
    } else {
        machine = MACHINE_PC;
        logfile = QString(USB_LOG_FILE_PC);
    }

    std::cout << "Logger: logfile = " << logfile.toStdString().c_str() << "\n";

    if(fileExists(logfile)) {
        std::cout << "Logger: messages will be written on file now\n";
        filelog = true;
        file = new QFile(logfile);
        file->open(QIODevice::WriteOnly);
    } else {
        std::cout << "Logger: messages will be written on console now\n";
    }
}

bool Logger::fileExists(QString path) {
    QFileInfo checkFile(path);
    if (checkFile.exists() && checkFile.isFile()) {
        return true;
    } else {
        return false;
    }
}

void Logger::outputMessage(QtMsgType type, const QString &msg)
{
    if(filelog) {
        qFileOut() << QTime::currentTime().toString() << ": " << msg << "\n";
        qFileOut().flush();
    } else if (machine == MACHINE_PC){
        std::cout << QTime::currentTime().toString().toStdString().c_str() << ": " << msg.toStdString().c_str() << std::endl;
    } else {  //  Delete this else part for device
        std::cout << QTime::currentTime().toString().toStdString().c_str() << ": " << msg.toStdString().c_str() << std::endl;
    }
}
