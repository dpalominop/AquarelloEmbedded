#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
//#include <QTcpServer>
//#include <QTcpSocket>
#include <QDebug>

class TcpServer : public QObject
{
    Q_OBJECT
public:
    explicit TcpServer(QObject *parent = 0);
signals:
    void workFinished();
    void putFile(QString);
public slots:
    void doWork();
    //void newConnection();
private:
    //TcpServer *server;
};

#endif // TCPSERVER_H
