#ifndef SOFTPHONESETTINGS_H
#define SOFTPHONESETTINGS_H

#include <QObject>
#include <QDebug>
#include <QString>
#include <QMessageBox>
#include <QtCore/QPointer>
#include <QtCore/QFile>
#include <QtCore/QIODevice>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QXmlStreamReader>

class SoftPhoneSettings : public QObject
{
    Q_OBJECT

public:
    explicit SoftPhoneSettings(QObject *parent = 0);
    explicit SoftPhoneSettings(QObject *parent = 0, QString XmlFile="./settings/softphone.xml");

public:

    QString sipDomain;
    QString sipUserName;
    QString sipPassword;

    bool selfPreview;
    bool externalVideo;

    void parseXML(QString XmlFile);
    QMap<QString, QString> parseSoftphone(QXmlStreamReader &xml);
    void addElementDataToMap(QXmlStreamReader &xml, QMap<QString, QString> &map) const;
    void setSettings(QMap<QString, QString> &Softphone);

private:
    void writeConfigDefault(QString);
};

#endif // SOFTPHONESETTINGS_H
