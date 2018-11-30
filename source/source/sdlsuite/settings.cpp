#include "settings.h"
#include <QFileInfo>
#include <QDebug>
#include <QTextStream>
#include <QProcess>
#include <sys/reboot.h>
#include <sys/utsname.h>

Settings *Settings::instance = 0;

Settings::Settings()
{
}

Settings *Settings::getInstance()
{
    if (!instance)
      instance = new Settings();
    return instance;
}

bool Settings::fileExists(QString path) {
    QFileInfo checkFile(path);
    if (checkFile.exists() && checkFile.isFile()) {
        return true;
    } else {
        return false;
    }
}

void Settings::readSettings() {
    QString read_file_path = USB_SETTINGS_FILE;
    if(fileExists(read_file_path)) {
        QSettings settings(read_file_path, QSettings::NativeFormat);

        writeNetworkSettings(settings);

        if(fileExists(SRC_PROGRAM_FILE)) {
            if (QFile::exists(DST_PROGRAM_FILE))
            {
                QFile::remove(DST_PROGRAM_FILE);
            }

            QFile::copy(SRC_PROGRAM_FILE, DST_PROGRAM_FILE);
        }

        system("sync");
        QFile::remove(USB_SETTINGS_FILE);
        QFile::remove(SRC_PROGRAM_FILE);
        system("sync");
        system("sync");
        reboot(RB_AUTOBOOT);
        exit(0);
    } else {
        struct utsname my_uname;
        uname(&my_uname);
        QString str = QString(my_uname.machine);
        if(!str.contains("arm", Qt::CaseInsensitive)) {

        }
    }
}

void Settings::writeNetworkSettings(QSettings &settings)
{
    qDebug()<<"Settings::writeNetworkSettings";
    QString outputFilename = NETWORK_INTERFACES_FILE;
    QFile outputFile(outputFilename);
    outputFile.open(QIODevice::WriteOnly);

    if(!outputFile.isOpen()){
        return;
    }

    QTextStream outStream(&outputFile);

    bool dhcp = QString::compare(settings.value("NETWORK_DHCP", QString()).toString(), "yes", Qt::CaseInsensitive) ? false : true ;

    if(dhcp) {
        outStream << "auto lo\niface lo inet loopback\n\nauto eth0\niface eth0 inet dhcp\n  pre-up /etc/network/nfs_check\n	wait-delay 15";
    } else {
        outStream << "auto lo\niface lo inet loopback\n\nauto eth0\niface eth0 inet static\n";
        outStream << "address " << settings.value("NETWORK_IP", QString()).toString() << "\n";
        outStream << "netmask " << settings.value("NETWORK_NETMASK", QString()).toString() << "\n";
        outStream << "broadcast " <<  settings.value("NETWORK_BROADCAST", QString()).toString() << "\n";
        outStream << "gateway " << settings.value("NETWORK_GATEWAY", QString()).toString() << "\n";
    }

    outputFile.flush();
    outputFile.close();
}
