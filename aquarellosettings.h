#ifndef AQUARELLOSETTINGS_H
#define AQUARELLOSETTINGS_H

#include <QWidget>
#include <QString>
#include <QDebug>
#include <QMessageBox>
#include <QCoreApplication>
#include <QtCore/QPointer>
#include <QtCore/QFile>
#include <QtCore/QIODevice>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QXmlStreamReader>

#define APP_VERSION "1.0.0_20130901"

class AquarelloSettings : public QWidget
{
    Q_OBJECT

public:
    explicit AquarelloSettings (QWidget *parent = 0);
    explicit AquarelloSettings (QWidget *parent = 0, QString XmlFile="./settings/aquarello.xml");

public:

    bool usingPictureCatalog;
    bool usingVideoCatalog;
    bool usingSoftPhone;
    bool usingBarcodeScanner;

    int pictureCatalogDelay_ms;
    int barcodeCatalogDelay_ms;

    QString terminal;
    QString serverip;
    int serverport;
    QString portcom;
    int baudrate;
    QString localipaddr;
    QString serialnumber;
    QString devicetypeid;
    QString password;
    QString appversion;
    QString baseurl;
    QString orientation;
    int width;
    int height;

    void parseXML(QString XmlFile);
    QMap<QString, QString> parseAquarello(QXmlStreamReader &xml);
    void addElementDataToMap(QXmlStreamReader &xml, QMap<QString, QString> &map) const;
    void setSettings(QMap<QString, QString> &Aquarello);

private:
    void writeConfigDefault(QString);
};

#endif // AQUARELLOSETTINGS_H
