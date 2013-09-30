#ifndef SERIALCOMMUNICATION_H
#define SERIALCOMMUNICATION_H

#include <QObject>
#include <cstdio>
#include <QTimer>
#include <QDebug>
#include <QtSerialPort/QSerialPort>

class SerialCommunication : public QObject
{
    Q_OBJECT
public:

    explicit SerialCommunication(QObject *parent = 0, QString portName="/dev/ttyACM0", int baudRate=9600 );

    ~SerialCommunication();

    void sendString(QString* message);
    void sendChar(char character);


    void setPort();

    bool isActive();
    bool isRunningContiniously();

    void scanContinuously(int interval_ms=100);
    void stopScanContinuously();
    void initSerial();

    QString receivedString;

    void serialClear();

signals:
    void timeoutScan();
    void newStringArrived(QString receivedString);
    void readyScan();

public slots:
    void receiveString();
private slots:
    void verifyPort();
    void closeSerialPort();
    void handleError(QSerialPort::SerialPortError error);

private:
    QSerialPort *serial;
    QString portName;
    int baudRate;

    unsigned char buffer[4096];
    int bufferSize;
    bool portIsOpened;
    bool runningContiniously;
    int socknum=0;

    QTimer *timer;
    QTimer *vtimer;

};

#endif // SERIALCOMMUNICATION_H
