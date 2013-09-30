#ifndef CONFIGURATION_H
#define CONFIGURATION_H
#include <QMap>
#include <QObject>
#include <QApplication>
#include <QXmlStreamReader>
//#include <QMessageBox>
#include <QFile>
#include <QTextStream>

//#define HTTP_PORT 8088
#define CONFIG_FILE "config.xml"
#define CONFIG_FTPFILE "configftp.xml"
#define WELCOME_FILE "welcome.xml"
#define LINEMSG_FILE "linemsg.xml"
//#define WORK_DIRECTORY "/sdcard/Aquarello/"
#define SLIDE_DIRECTORY "slideshow"
//#define BACKFOREIMG_DIRECTORY "backforeimg"
#define BACKFOREIMG_DIRECTORY "slideshow"
//#define ADVERTISE_DIRECTORY "advertise"
//#define ADVERTISE_CONFIG "advertise.xml"
//#define ADVERTISE_POSX "0"
//#define ADVERTISE_POSY "400"
#define IMAGES_DIRECTORY "slideshow"
#define WELCOME_HTML "/tmp/welcome.html"
#define VIEW_FILE "/tmp/view.html"
#define DEVICETYPEID "040000003406"
#define SERIALNMBR "000000000010"
#define APP_VERSION "1.0.0_20130725"
//#define AQUARELLO_RD_CMD "mount -r -o remount /media/mmcblk0p4"
//#define AQUARELLO_RW_CMD "mount -rw -o remount /media/mmcblk0p4"
//#define CONFIG_RD_CMD "mount -r -o remount /media/mmcblk0p3"
//#define CONFIG_RW_CMD "mount -rw -o remount /media/mmcblk0p3"
//#define WPA_FILE "/media/mmcblk0p3/etc/wpa_supplicant.conf"
#define WPA_FILE "/etc/wpa_supplicant.conf"
//#define INTERFACES_FILE "/media/mmcblk0p3/etc/network/interfaces"
#define INTERFACES_FILE "/etc/network/interfaces"
#define TMP_CONFIG_FILE "/tmp/config.xml"
#define TMP_WPA_FILE "/tmp/wpa_supplicant.conf"
#define TMP_INTERFACES_FILE "/tmp/interfaces"
#define TMP_CONFIG_FTPFILE "/tmp/configftp.xml"
#define TMP_DIR "/tmp"
#define TMP_FTP "/tmp/ftp"
#define FTP_DIR "ftpdir"

class Configuration : public QObject
{
    Q_OBJECT
public:
    explicit Configuration(QObject *parent = 0);

signals:
    
public slots:
    void parseXML();

private:
    QString PATH;
    QString root;
    QString terminal;
    QString serverip;
    int serverport;
    int timecode;
    int timeslide;
    QString portcom;
    int baudrate;
    //Qint headcode = 1;
    QString localipaddr;
    QString serialnumber;
    QString devicetypeid;
    QString password;
    QString appversion;
    QString baseurl;
    QMap<QString, QString> parseAquarello(QXmlStreamReader& xml);
    void addElementDataToMap(QXmlStreamReader& xml,
                                 QMap<QString, QString>& map) const;
    void writeConfigDefault(QString);
    void writeConfigFile(QString);

public:
    QList< QMap<QString,QString> > params;
    QString getPortCom();
    int getBaudrate();
    QString getTerminal();
    QString getServerip();
    int getServerport();
    int getTimeCode();
    int getTimeSlide();
    QString getPassword();
    QString getBaseURL();
    QString getAppVersion();
    void checkAppVersion();

//    QMap<QString,QString> MainView;
//    QMap<QString,QString> FirstView;
//    QMap<QString,QString> SecondView;
//    QMap<QString,QString> ProductView;
};

#endif // CONFIGURATION_H
