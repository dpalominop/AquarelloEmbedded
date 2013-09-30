#ifndef CONFIGFTP_H
#define CONFIGFTP_H

#include <QObject>
#include "configuration.h"

class ConfigFtp : public QObject
{
    Q_OBJECT
public:
    explicit ConfigFtp(QObject *parent = 0);

signals:

public slots:
    void parseXML();
private:
    QString PATH;
    QString root;
    QString ftpserver;
    int ftpport;
    QString ftpuser;
    QString ftppasswd;
    QString ftpprefname;
    QString ftpfileref;
    QMap<QString, QString> parseConfigFtp(QXmlStreamReader& xml);
    void addElementDataToMap(QXmlStreamReader& xml,
                                 QMap<QString, QString>& map) const;
    void writeConfigDefault(QString);
    void writeConfigFile(QString);

public:
    QList< QMap<QString,QString> > params;
    QString getFtpServer();
    int getFtpPort();
    QString getFtpUser();
    QString getFtpPasswd();
    QString getFtpPrefName();
    QString getFtpFileRef();
    void setFtpFileRef(QString);
    void updateConfigFile();
};

#endif // CONFIGFTP_H
