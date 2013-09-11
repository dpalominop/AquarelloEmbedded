#include "tcpclient.h"

TcpClient::TcpClient(QObject *parent) :
    QObject(parent)
{

}


void TcpClient::Connect(QString serverip,int  port,int Timeout)
{

    socket=new QTcpSocket(this);
    socket->connectToHost(serverip, port);
    qDebug()<< "try to connect server: "<< serverip << "by port: "<<port;
    if (!socket->waitForConnected(Timeout)) {
        qDebug()<< "Not Connected";
	constatus=0;
        return;
    }
    constatus=1;
    qDebug()<< "Connected!";
}

void TcpClient::senData(QByteArray data)
{
    socket->write(data);
    qDebug()<< "Request to the server :"<<data;
}

/*QString TcpClient::receiveData(int Timeout)
{
    while (socket->bytesAvailable() < (int)sizeof(quint16)) {
        if (!socket->waitForReadyRead(Timeout)) {
            //return "Ready time exceeded";
	    return "";
        }
         qDebug()<< socket->bytesAvailable()<<"min: "<<sizeof(quint16);
    }

    return socket->readAll();
}*/
/*
QString TcpClient::receiveData(int Timeout)
{
    QString recv = "";

    while (socket->state() == QAbstractSocket::ConnectedState){

        if (!socket->waitForReadyRead(Timeout)) {
            //return "Ready time exceeded";
            //qDebug()<< socket->errorString();
            break;
        }else{
            recv += socket->readAll();
        }
            //qDebug()<< socket->bytesAvailable()<<"min: "<<sizeof(quint16);

    }

    return recv;
}
*/
QString TcpClient::receiveData(int Timeout)
{
    QString recv = "";
    int bytesAvail;
    int bytesRead=0;
    int totalBytesRead=0;
    char *data = new char[2048];

    memset(data,0,sizeof(data));
    while (socket->state() == QAbstractSocket::ConnectedState){

        if (!socket->waitForReadyRead(Timeout)) {
            //return "Ready time exceeded";
            break;
        }else{
            bytesAvail = socket->bytesAvailable();
            if(bytesAvail > 0){
                //memset(ch,0,sizeof(ch));
                bytesRead = socket->read(&data[totalBytesRead],2045-totalBytesRead);
                totalBytesRead += bytesRead;
                //qDebug() << "RECV="+QString(data)+"LEN="+QString("%1").arg(bytesRead);
		//qDebug() << QString("BYTES READ = %1").arg(bytesRead);
		if(totalBytesRead >= 2045){
			totalBytesRead = 2045;
			socket->close();
			break;
		}
            }
            //qDebug()<< socket->bytesAvailable()<<"min: "<<sizeof(quint16);
        }

    }
    //if(bytesRead > 0){
    if(totalBytesRead > 0){
        recv = QString::fromLatin1(&data[0],totalBytesRead);
        //recv = QString::fromUtf8(&data[0],totalBytesRead);
    }

    return recv;
}
QString TcpClient::receiveDataIBM(int Timeout)
{
    QString recv = "";
    int bytesAvail;
    int bytesRead=0;
    int totalBytesRead=0;
    char *data = new char[2048];

    memset(data,0,sizeof(data));
    while (socket->state() == QAbstractSocket::ConnectedState){

        if (!socket->waitForReadyRead(Timeout)) {
            //return "Ready time exceeded";
            break;
        }else{
            bytesAvail = socket->bytesAvailable();
            if(bytesAvail > 0){
                //memset(ch,0,sizeof(ch));
                bytesRead = socket->read(&data[totalBytesRead],2045-totalBytesRead);
                totalBytesRead += bytesRead;
                //qDebug() << "RECV="+QString(data)+"LEN="+QString("%1").arg(bytesRead);
		if(totalBytesRead >= 2045){
			socket->close();
                        break;
                }

            }
            //qDebug()<< socket->bytesAvailable()<<"min: "<<sizeof(quint16);
        }

    }

    //if(bytesRead > 0){
    if(totalBytesRead > 4){
        recv = QString::fromLatin1(&data[4],totalBytesRead-4);
        //recv = QString::fromUtf8(&data[4],totalBytesRead-4);
    }

    return recv;
}
bool TcpClient::isConnected()
{
	return constatus;
}
