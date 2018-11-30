#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QString>
#include <QSettings>

#define USB_SETTINGS_FILE "/media/usbhd-sda1/serialserver.txt"
#define SRC_PROGRAM_FILE "/media/usbhd-sda1/serialserver"
#define DST_PROGRAM_FILE "/usr/bin/serialserver"
#define NETWORK_INTERFACES_FILE "/etc/network/interfaces"

class Settings
{
private:
    static Settings *instance;

public:
    static Settings *getInstance();
    void readSettings();

private:
    Settings();
    bool fileExists(QString path);
    void writeNetworkSettings(QSettings &settings);

signals:

public slots:
};

#endif // SETTINGS_H
