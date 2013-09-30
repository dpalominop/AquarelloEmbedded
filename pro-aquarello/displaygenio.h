#ifndef DISPLAYGENIO_H
#define DISPLAYGENIO_H
#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include "welcomeconfiguration.h"
#include "linemessages.h"

class DisplayGenio : public QObject
{
    Q_OBJECT
public:
    explicit DisplayGenio(QObject *parent = 0);
    void createWelcomePage(QString);
    void createDisplayPage(QString);
    void setBackImgPath(QString);
    void setForeImgPath(QString);
    void byteData(QByteArray);
private:
    QByteArray Data;
    QString headScriptHTML();
    LineMessages *lm;
    WelcomeConfiguration *msg;
    QString backImgPath;
    QString foreImgPath;
};
#endif // DISPLAYGENIO_H
