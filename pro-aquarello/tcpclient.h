#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QDebug>
#include <QMutex>
#include <QWaitCondition>

class TcpClient : public QObject
{
    Q_OBJECT
public:
    explicit TcpClient(QObject *parent = 0);
    //void connect(const QObject *sender, const char *signal, const char *member, Qt::ConnectionType type) const;
    void Connect(QString serverip,int  port,int Timeout);
    void senData(QByteArray);
    QString receiveData(int Timeout);
    QString receiveDataIBM(int Timeout);
    void writeFile(QString filename);
    // public byte[] writeByte()
    bool isConnected();


signals:

    
public slots:

private:
    QTcpSocket *socket;
    bool constatus;
};

#endif // TCPCLIENT_H
