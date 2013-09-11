#ifndef FTPCLIENT_H
#define FTPCLIENT_H

#include <QObject>
#include <QDebug>
#include <QFtp>
#include <QUrlInfo>
#include <QEventLoop>
#include <QTimer>
#include <QFile>
#include <QProcess>
#include <QDir>

#include "configuration.h"

class FtpClient : public QObject
{
    Q_OBJECT
public:
    explicit FtpClient(QObject *parent = 0);
    void connectToftp(QString, int, int);
    void setCredentials(QString, QString);
    void setFileRef(QString);
    void setPrefix(QString);
    bool getUpdateStatus();
    QString getCurrentFileName();
signals:

public slots:

private slots:
    void ftpCommandFinished(int commandId, bool error);
    void addToList(const QUrlInfo &urlInfo);
    void testNetwork();
    bool removeDirectory(QDir);
    //void mySubProcessDone(int,QProcess::ExitStatus);
    void mySubProcessDone();
    void onProcessStarted();
private:
    QString IpAddr;
    int Port;
    QString UserName;
    QString Password;
    QString FileNameRef;
    QString Prefix;
    QFtp *ftp;
    QFile *file;
    QEventLoop *eventLoop;
    QStringList listfiles;
    QTimer *timer;
    bool status;
    bool updatestatus;
    QString newfile;
    QDateTime convertDate(QString);
    QString curfilename;

};

#endif // FTPCLIENT_H
