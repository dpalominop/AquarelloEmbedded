#ifndef SERIALCOMMUNICATION_H
#define SERIALCOMMUNICATION_H

#include <QObject>
#include <cstdio>
#include <QTimer>
#include <QDebug>

#include "rs232.h"

class SerialCommunication : public QObject
{
    Q_OBJECT
public:

    explicit SerialCommunication(QObject *parent = 0, int portNumber=0, int baudRate=9600 );

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

signals:
    void timeoutScan();
    void newStringArrived(QString receivedString);
    void readyScan();

public slots:
    void receiveString();
private slots:
    void verifyPort();

private:
    int portNumber;
    int baudRate;

    unsigned char buffer[4096];
    int bufferSize;
    bool portIsOpened;
    bool runningContiniously;

    QTimer *timer;
    QTimer *vtimer;

};

#endif // SERIALCOMMUNICATION_H
