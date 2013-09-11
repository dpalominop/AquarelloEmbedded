#ifndef LINPHONE_H
#define LINPHONE_H

#include <QWidget>
#include <QPushButton>
#include <QThread>
#include <QDebug>
#include <QLabel>
#include <QLayout>
#include <QVBoxLayout>
#include "softphone.h"
#include "softphonewidget.h"

class Linphone : public QWidget
{
    Q_OBJECT
    
public:
    explicit Linphone(QWidget *parent = 0);
    void resizeEvent(QResizeEvent *event);

signals:
    void signalCallHangup(QString);

private slots:
    void slotCallHangupButton();
    void slotEndCall();

private:
    bool EnableButton;
    QPushButton * callHangupButton;
    //Softphone *phone;
    SoftphoneWidget *phoneWidget;
    QVBoxLayout *lay;

};

#endif // LINPHONE_H
