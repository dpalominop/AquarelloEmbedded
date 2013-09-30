#ifndef UDPCLIENT_H
#define UDPCLIENT_H
#include <QObject>
#include <QTime>
#include <QThread>
#include <QUdpSocket>

class UdpClient : public QObject
{
    Q_OBJECT

public:
    explicit UdpClient(QObject *parent = 0);
    void Connect(QString,int,int);
    void senData(QByteArray);
    void closeAll();
    QString receiveData(int);
private:
    //QUdpSocket *server;
    //QUdpSocket *client;
    QString SrvIp;
    QString received;
    bool status;
    int SrvPort;
    int Timeout;

};
#endif // UDPCLIENT_H
