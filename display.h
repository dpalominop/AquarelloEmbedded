#ifndef DISPLAY_H
#define DISPLAY_H
#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QStringList>
//#include "welcomeconfiguration.h"
#include "linemessages.h"

class Display : public QObject
{
    Q_OBJECT
public:
    explicit Display(QObject *parent = 0);
    void createDisplayPage(QString);
    void createDisplayPageAqua(QString);
    void createDisplayPageGenio(QString);
    void createDisplayPageIBM(QString);
    void createDisplayPageWEB(QString);
    void createWelcomePage(QString);
    void setBackImgPath(QString);
    void setForeImgPath(QString);
    void strData(QString);
    void setTerminal(QString);
    void setDataAccessFault(QString);
    void byteData(QByteArray);
    void parseXML(QString,QString);
    void clearFields();

private:
    void parseMessageIBM(QXmlStreamReader& xml);
    QString headScriptHTML();
    QString strdata;
    //WelcomeConfiguration *msg;
    LineMessages *lm;
    QString backgroundColor(QString);
    QString foregroundColor(QString);
    QString styleFont(QString);
    QString termDisplay;
    QString backImgPath;
    QString foreImgPath;
    QString errMessage;
    QByteArray bData;
    QString ItemCode;
    QString SalePrice;
    QString ItemName;
    QString RestrictedSaleType;
    QString Discount;
};
#endif // DISPLAY_H
