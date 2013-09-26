#ifndef BARCODECATALOG_H
#define BARCODECATALOG_H
#include <QWebView>
#include <QDesktopWidget>
//#include <QWebView>
#include <QWebElement>
#include <QWebElementCollection>
#include <QWebFrame>
#include <QString>
#include <QTimer>
#include <QDir>
#include <QDebug>
#include <QNetworkInterface>
#include <QtMultimedia/QSound>
#include <QProcess>
#include "serialcommunication.h"
//#include "barcodescannersettings.h"
#include "aquarellosettings.h"
#include "tcpclient.h"
#include "udpclient.h"
#include "display.h"
#include <iostream>

#define BACKFOREIMG_DIRECTORY "scanner_pictures"
#define VIEW_FILE "/tmp/view.html"
#define DEVICETYPEID "040000003406"
#define SERIALNMBR "000000000010"

class BarcodeCatalog : public QWebView
{
    Q_OBJECT
public:
    explicit BarcodeCatalog(QWidget *parent = 0);
    //explicit BarcodeCatalog(QWidget *parent = 0, int catalogDuration_ms = 5000);
    //explicit BarcodeCatalog(QWidget *parent = 0, AquarelloSettings *settings = NULL);
    void setSettings(AquarelloSettings *);
    void start();

signals:
    void catalogTimeout();
    void queryReceived();
    void quitRequest();
    void startSound();
    void startServerConsulting(QString);

public slots:
    void queryCatalog(QString barcodeNumber);
    void stopScanning();
    void startScanning();
    void serverConsulting(QString codigo);

private slots:
    void adjustLocation();
    void finishLoading(bool);
    void linkClicked(QUrl url);

private:
    int catalogDuration_ms ;
    QTimer * timer;
    SerialCommunication *serialCom;
    //BarcodeScannerSettings *settings;
    AquarelloSettings *settings;
    void init();
    bool remote;
    //void fillSerialPortMap();
    //void fillAllowedBaudRates();
    QMap<QString , int> serialPortMap;
    QString backgroundImagePath;
    //QString getBackground();
    QString getImageFilePath(QString,QString,QString);
    QByteArray intToByteArray(int);
    void ReadCodeStatus(QString);
    QString PATH;
    QThread *myThread;
    QSound *mySound;
};

#endif // BARCODECATALOG_H
