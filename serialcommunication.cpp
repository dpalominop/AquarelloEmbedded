#include "serialcommunication.h"

SerialCommunication::SerialCommunication(QObject *parent,
                                         QString portName,
                                         int baudRate) :
    QObject(parent),
    portName(portName),
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
    if(portIsOpened)    closeSerialPort();
}

void SerialCommunication::initSerial()
{
    serial = new QSerialPort(this);
    serial->setPortName(portName);
    serial->setBaudRate(baudRate);

    connect(serial, SIGNAL(error(QSerialPort::SerialPortError)), this,SLOT(handleError(QSerialPort::SerialPortError)));

    setPort();
    if(!portIsOpened){
        vtimer = new QTimer();
        vtimer->setInterval(5000);
        connect(vtimer,SIGNAL(timeout()),this,SLOT(verifyPort()));
        vtimer->start();
    }else{
            qDebug() << "READY SCAN";
            emit readyScan();
    }
    connect(timer,SIGNAL(timeout()),this,SLOT(receiveString()));
}

void SerialCommunication::sendString(QString *message)
{

    QByteArray ba = message->toLatin1();
    const char *c_string = ba.data();
    //cprintf(portNumber, c_string);
}


void SerialCommunication::sendChar(char character)
{
    //SendByte(portNumber, character);
}

void SerialCommunication::receiveString()
{
    QByteArray data = serial->readAll();

    if(data.size() > 0){
        receivedString = QString(data);
        printf("serialCom:    New String arrived\n");
        emit newStringArrived(receivedString);
    }
}

void SerialCommunication::setPort()
{
    if (serial->open(QIODevice::ReadWrite))
    {
        portIsOpened=true;
    }
    else
    {
        qDebug() << "WARNING: Can not open comport\n";
        portIsOpened=false;
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
        vtimer->stop();
        emit readyScan();
    }
}
void SerialCommunication::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        //QMessageBox::critical(this, tr("Critical Error"), serial->errorString());
        qDebug() << "Critical Error Serial: " << serial->errorString();
        stopScanContinuously();
        closeSerialPort();
        portIsOpened = false;
        if(!vtimer){
            vtimer = new QTimer();
            vtimer->setInterval(5000);
            connect(vtimer,SIGNAL(timeout()),this,SLOT(verifyPort()));
            vtimer->start();
        }else{
            if(vtimer->isActive()){
                vtimer->stop();
            }
            vtimer->start();
        }
    }

}
void SerialCommunication::closeSerialPort()
{
    serial->close();

}
