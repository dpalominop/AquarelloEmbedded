
#include "udpclient.h"
#include "udp_client.h"

UdpClient::UdpClient(QObject *parent) :
    QObject(parent)
{

}
void UdpClient::Connect(QString serverip,int  port,int timeout)
{
    SrvIp=serverip;
    SrvPort=port;
    Timeout=timeout;
}
void UdpClient::senData(QByteArray datagram)
{
    qDebug()<< "Request to the server :"<<datagram.data();
    received = ReadUDPSocket(SrvIp.toLatin1().data(),SrvPort,Timeout,datagram.data());

}
QString UdpClient::receiveData(int timeout)
{
    return received;

}

int isReadable(int sdr,int * error,int timeOut)
{
        struct timeval tv;

        fd_set socketReadSet;
        FD_ZERO(&socketReadSet);
        FD_SET(sdr,&socketReadSet);

        if (timeOut) {
                tv.tv_sec  = timeOut / 1000;
                tv.tv_usec = (timeOut % 1000) * 1000;
        } else {
                tv.tv_sec  = 0;
                tv.tv_usec = 0;
        } // if
        //SOCKET_ERROR = -1
        if (select(sdr+1,&socketReadSet,0,0,&tv) == -1) {
                *error = 1;
                return 0;
        } // if
        *error = 0;
        //printf("FD_ISSET=%d\n",FD_ISSET(sd,&socketReadSet));
        return FD_ISSET(sdr,&socketReadSet) != 0;
} /* isReadable */

QString ReadUDPSocket(char *serverip, int serverport, int timeout, char *data)
{
        char msg[1024];
        char receive[1024];
        struct sockaddr_in cliAddr,remoteServAddr;
        struct hostent *h;
        int n,timeOut,error;
        int echoLen;
        int sd,rc;
        int len=0;
        //recepcion primera trama

        h = gethostbyname(serverip);
        remoteServAddr.sin_family = h->h_addrtype; memcpy((char *) &remoteServAddr.sin_addr.s_addr,h->h_addr_list[0], h->h_length);
        remoteServAddr.sin_port = htons(serverport);
        sd = socket(AF_INET,SOCK_DGRAM,0);
        if(sd<0) {
            return QString("");
        }
        cliAddr.sin_family = AF_INET;
        cliAddr.sin_addr.s_addr = htonl(INADDR_ANY);
        cliAddr.sin_port = htons(0);

        rc = bind(sd, (struct sockaddr *) &cliAddr, sizeof(cliAddr));
        if(rc<0) {
            //printf("%s: cannot bind port\n", argv[0]);
            close(sd);
            return QString("");
        }
        rc = sendto(sd, data, strlen(data)+1, 0,(struct sockaddr *) &remoteServAddr,sizeof(remoteServAddr));

        //timeOut=10000;
        timeOut=timeout;
        if(!isReadable(sd,&error,timeOut)){
                //timeout recepcion respuesta
                qDebug() << "TIMEOUT";
                close(sd);
                return QString("");
        }else{
                memset(msg,0,sizeof(msg));
                echoLen=sizeof(remoteServAddr);
                n = recv(sd, msg, sizeof(msg), 0);
                if(n<0) {
                        //printf("cannot receive data\n");
                        close(sd);
                        return QString("");
                }
                if(n>0){
                    strcpy(&receive[len],msg);
                    len+=n;
                }
                //printf("msg = %s\n",msg);
                //ip_decodepacket(msg,fp);
        }
        timeOut=600;
        while(isReadable(sd,&error,timeOut)){
                memset(msg,0,sizeof(msg));
                echoLen=sizeof(remoteServAddr);
                n = recv(sd, msg, sizeof(msg), 0);
                if(n<0) {
                        //printf("cannot receive data1\n");
                        break;
                }
                if(n>0){
                    strcpy(&receive[len],msg);
                    len+=n;
                }
                timeOut=350;
        }
        close(sd);
	//return QString::fromLatin1(receive);
	QString qstr = QString::fromUtf8(&receive[0],len);
        //qDebug() << qstr;
        return qstr;
}
