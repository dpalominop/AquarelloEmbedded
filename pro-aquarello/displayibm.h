#ifndef DISPLAYIBM_H
#define DISPLAYIBM_H
#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QStringList>
#include "welcomeconfiguration.h"
#include "linemessages.h"

class DisplayIBM : public QObject
{
    Q_OBJECT
public slots:
    void parseXML(QString);

public:
    explicit DisplayIBM(QObject *parent = 0);
    void createDisplayPage(QString);
    void createWelcomePage(QString);
    void setBackImgPath(QString);
    void setForeImgPath(QString);
    void strData(QString);
    void setDataAccessFault(QString);
    //QList< QMap<QString,QString> > params;

private:
    QList< QMap<QString,QString> > params;
    void parseMessage(QXmlStreamReader& xml);
    QString headScriptHTML();
    LineMessages *lm;
    QString errMessage;
    WelcomeConfiguration *msg;
    QString backgroundColor(QString);
    QString foregroundColor(QString);
    QString styleFont(QString);
    QString backImgPath;
    QString foreImgPath;
    QString ItemCode;
    QString SalePrice;
    QString ItemName;
    QString RestrictedSaleType;
    QString Discount;
};
#endif // DISPLAY_H
