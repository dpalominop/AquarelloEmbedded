#include "serialcommunication.h"

SerialCommunication::SerialCommunication(QObject *parent,
                                         int portNumber,
                                         int baudRate) :
    QObject(parent),
    portNumber(portNumber),
    baudRate(baudRate),
    bufferSize(0),
    portIsOpened(false),
    runningContiniously(false),
    receivedString(QString()),
    timer(new QTimer())
{

}

SerialCommunication::~SerialCommunication()
{
    if(portIsOpened)    CloseComport(portNumber);
}

void SerialCommunication::initSerial()
{
    setPort();

    vtimer = new QTimer();
    vtimer->setInterval(1000);
    connect(vtimer,SIGNAL(timeout()),this,SLOT(verifyPort()));
    vtimer->start();

    if(portIsOpened){
        qDebug() << "READY SCAN";
        emit readyScan();
    }
    connect(timer,SIGNAL(timeout()),this,SLOT(receiveString()));
}

void SerialCommunication::sendString(QString *message)
{

    QByteArray ba = message->toLatin1();
    const char *c_string = ba.data();
    cprintf(portNumber, c_string);
}


void SerialCommunication::sendChar(char character)
{
    SendByte(portNumber, character);
}

void SerialCommunication::receiveString()
{
    if(portIsOpened)
    {
        bufferSize = PollComport(portNumber, buffer, 4095);
        //printf("Serial Com  : Scanning\n");

        if(bufferSize > 0)
        {
          buffer[bufferSize] = 0;   // always put a "null" at the end of a string!

/*          for(int i=0; i < bufferSize; i++)
          {
            if(buffer[i] < 32)  // replace unreadable control-codes by dots
            {
              buffer[i] = '.';
            }
          }
*/
          //printf("received %i bytes: %s\n", bufferSize, (char *)buffer);
          receivedString=QString((char *)buffer);
          printf("serialCom:    New String arrived\n");
          emit newStringArrived(receivedString);
        }
        else
        {
            //printf("Could NOT Poll Serial Port\n");
        }

        //receivedString=QString((char *)buffer);
    }

}


void SerialCommunication::setPort()
{


    if(OpenComport(portNumber, baudRate))
    {
      qDebug() << "WARNING: Can not open comport\n";

      portIsOpened=false;
    }
    else
    {

      portIsOpened=true;
    }

}


bool SerialCommunication::isActive()
{
    return portIsOpened;

}
bool SerialCommunication::isRunningContiniously()
{
    return runningContiniously;
}

void SerialCommunication::scanContinuously(int interval_ms)
{
    if(!timer->isActive())
        timer->start(interval_ms);
}
void SerialCommunication::stopScanContinuously()
{
    timer->stop();
}
void SerialCommunication::verifyPort(){

    setPort();
    if(portIsOpened){
        if (!timer->isActive())
            emit readyScan();
    }else{
        timer->stop();
    }
}
