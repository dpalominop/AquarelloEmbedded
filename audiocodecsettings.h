#ifndef AUDIOCODECSETTINGS_H
#define AUDIOCODECSETTINGS_H

#include <QObject>
#include <QDebug>
#include <QString>
#include <QMessageBox>
#include <QCoreApplication>
#include <QtCore/QPointer>
#include <QtCore/QFile>
#include <QtCore/QIODevice>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QXmlStreamReader>

class AudioCodecSettings : public QObject
{
    Q_OBJECT
public:
    explicit AudioCodecSettings(QObject *parent = 0);
    explicit AudioCodecSettings(QObject *parent = 0, QString XmlFile="./settings/audiocodecs.xml");

    QList< QMap<QString,QString> > codecs;

signals:

public slots:
    void parseXML(QString XmlFile);

private:
    QMap<QString, QString> parseCodec(QXmlStreamReader& xml);
    void addElementDataToMap(QXmlStreamReader& xml, QMap<QString, QString>& map) const;
    
};

#endif // AUDIOCODECSETTINGS_H
