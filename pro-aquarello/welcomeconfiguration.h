#ifndef WELCOMECONFIGURATION_H
#define WELCOMECONFIGURATION_H

#include <QObject>
#include <QMap>
#include <QApplication>
#include <QXmlStreamReader>
class WelcomeConfiguration : public QObject
{
    Q_OBJECT
public:
    explicit WelcomeConfiguration(QObject *parent = 0);

signals:
    
public slots:
    void parseXML();

private:
    QList< QMap<QString,QString> > params;
    QMap<QString, QString> parseMessage(QXmlStreamReader& xml);
    void addElementDataToMap(QXmlStreamReader& xml,
                                 QMap<QString, QString>& map) const;
    void writeConfigDefault(QString);
    QString PATH;
    QString msgtxt;
    int msgx;
    int msgy;
    QString msgforecolor;
    QString msgcolor;
    int msgsize;
    QString msgeffect;
    QString msgstyle;
    QString msgweight;
    QString msgvisible;
public:
    QString getMsgTxt();
    int getMsgX();
    int getMsgY();
    QString getMsgForeColor();
    QString getMsgColor();
    int getMsgSize();
    QString getMsgEffect();
    QString getMsgStyle();
    QString getMsgWeight();
    QString getMsgVisible();
    void setMsgTxt(QString);

};

#endif // WELCOMECONFIGURATION_H
/*
<WelcomeMsgTxt>GEMPSY-TCC</WelcomeMsgTxt>
<WelcomeMsgX>250</WelcomeMsgX>
<WelcomeMsgY>200</WelcomeMsgY>
<WelcomeMsgForecolor>53E61E</WelcomeMsgForecolor>
<WelcomeMsgColor>F51E0F</WelcomeMsgColor>
<WelcomeMsgSize>40</WelcomeMsgSize>
<WelcomeMsgEffect>Fixed</WelcomeMsgEffect>
<WelcomeMsgStyle>Normal</WelcomeMsgStyle>
<WelcomeMsgWeight>Normal</WelcomeMsgWeight>
<WelcomeMsgVisible>true</WelcomeMsgVisible>
*/
