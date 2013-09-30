#ifndef BARCODESCANNERSETTINGS_H
#define BARCODESCANNERSETTINGS_H

#include <QWidget>
#include <QString>
#include <QDebug>
#include <QMessageBox>
#include <QtCore/QPointer>
#include <QtCore/QFile>
#include <QtCore/QIODevice>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QXmlStreamReader>
#include <QTextStream>
#include <QMap>
#include <QList>
#include <iostream>

class BarcodeScannerSettings : public QWidget
{
    Q_OBJECT
    friend class BarcodeCatalog;
public:
    explicit BarcodeScannerSettings(QWidget *parent = 0);
    explicit BarcodeScannerSettings(QWidget *parent = 0, QString configFile="./settings/scanner.xml");

    void setSettings(QMap<QString, QString> &Scanner);
    void addElementDataToMap(QXmlStreamReader &xml, QMap<QString, QString> &map) const;
    QMap<QString, QString> parseScanner(QXmlStreamReader &xml);

    void fillSerialPortMap();
    void fillAllowedBaudRates();
    QMap<QString , int> serialPortMap;
    QList<int> allowedBaudRates;

    int baudRate;
    int serialPort;

    void parseXML(QString XmlFile);
};

#endif // BARCODESCANNERSETTINGS_H
