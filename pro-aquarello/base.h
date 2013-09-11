#ifndef BASE_H
#define BASE_H
#include <QApplication>
#include <QMainWindow>
//#include <QTextEdit>
//#include <QPushButton>
#include <QLineEdit>
#include <QWidget>
#include <QBoxLayout>
#include <QTimer>
#include <QWebView>
#include <QFile>
//#include <QMessageBox>
#include <QWebElement>
#include <QWebElementCollection>
#include <QWebFrame>
//#include <QXmlStreamReader>
#include <QDir>
#include <QProcess>
//#include <QtMultimedia/QSound>
#include <QNetworkInterface>
//#include <QTextCodec>

#include "configuration.h"
#include "configftp.h"
#include "welcomeconfiguration.h"
#include "display.h"
#include "displaygenio.h"
#include "displayibm.h"
#include "tcpclient.h"
#include "udpclient.h"
#include "tcpserver.h"
#include "ftpclient.h"

class Base : public QMainWindow
{
    Q_OBJECT
public:
    explicit Base(QWidget *parent = 0);
    QWebView* web;
    QLineEdit* editbarcode;
    QTimer *slidetimer;
    QTimer *codetimer;

signals:
    
public slots:
    void barcodeSlot();
    //void on_pageLoad_finished(bool);
    void normal_status();
    void welcome_status();
    void show_product_detail(QString);
    QString getForeground();
    QString getBackground();
    void doPut(QString);

private slots:
    void adjustLocation();
    void finishLoading(bool);
    void linkClicked(QUrl url);
    void quit();
    
private:
    //QPushButton *quitButton;
    //bool loadedFlag;
    bool codeFlag;
    //bool initportFlag;
    Configuration *manager;
    ConfigFtp *cfgftp;
    WelcomeConfiguration *welcome;
    TcpClient *tcpclient;
    UdpClient *udpclient;
    TcpServer *tcpserver;
    FtpClient *ftpclient;
    QStringList slidelist;
    int imageNumberToShow;
    int lengthSlideShow;
    Display *display;
    QString welcomepage;
    void welcomePage();
    void getSlideShow();
    QString localipaddr;
    QString terminal;
    QString getIpAddr();
    QString foregroundPath;
    QString backgroundPath;
    QByteArray intToByteArray(int);
    void ReadCodeStatus(QString);
    void qrcodeSettings(QString);
    QList<QString> headerHtml();
    QList<QString> footerHtml();
    QList<QString> bodyHtml(int, int, QString);
    void writeHtmlView(QList<QString>);
    bool remote;
    void processFtp();
    bool processZipFile(QString);
    bool removeDirectory( QDir );
    QDateTime convertDate(QString);
    QString qrcode;
};

#endif // BASE_H
