#include "linphone.h"


Linphone::Linphone(QWidget *parent) :
    QWidget(parent)
{
    this->EnableButton = TRUE;

    this->callHangupButton = new QPushButton(this);
    this->phoneWidget = new SoftphoneWidget(this);

    callHangupButton->setIcon(QIcon());
    callHangupButton->setIcon(QIcon("icons/CallButton.png"));
    callHangupButton->setIconSize(QSize(128, 128));
    callHangupButton->adjustSize();

    QPalette* palette = new QPalette();
    palette->setColor(QPalette::Button,Qt::transparent);
    callHangupButton->setPalette(*palette);

    connect( this->callHangupButton, SIGNAL(clicked()), this, SLOT(slotCallHangupButton()) );
    connect( this, SIGNAL(signalCallHangup(QString)), this->phoneWidget->phone, SLOT(callHangup(QString)) );
    connect( this->phoneWidget->phone, SIGNAL(signalEndCall()), this, SLOT(slotEndCall()) );

    resizeEvent(NULL);
}

void Linphone::resizeEvent(QResizeEvent *event)
{
    //TODO: FIX RESIZE ISSUE WHEN CALLING

    //QWidget::resizeEvent(event);

    this->phoneWidget->setMaximumSize(this->size());
    this->phoneWidget->resize(this->size());

    callHangupButton->raise();

    float spacingFactor=1;
    QPoint callButtonPosition;
    callButtonPosition.setX(spacingFactor*((this->width()-this->height())/2));
    callButtonPosition.setY(spacingFactor*(this->height()-callHangupButton->height()));
    callHangupButton->move(callButtonPosition);

    this->phoneWidget->stackUnder(callHangupButton);
    this->phoneWidget->lower();

    QWidget::resizeEvent(event);
}

void Linphone::slotEndCall()
{
    this->callHangupButton->setIcon(QIcon("icons/CallButton.png"));
    qDebug() << "Call Ended";
    this->phoneWidget->phone->IncomingEnabled = FALSE;
    this->EnableButton = TRUE;
}

void Linphone::slotCallHangupButton()
{
    if(this->EnableButton){

        if (this->phoneWidget->phone->IncomingEnabled){
            emit signalCallHangup("AcceptingIncomingCall");
        }
        else{
            emit signalCallHangup("Call");
        }

        qDebug() << "Call Started";
        this->callHangupButton->setIcon(QIcon("icons/EndCallButton.png"));
        this->EnableButton = FALSE;

    }else if(!EnableButton){

        if (this->phoneWidget->phone->IncomingEnabled){
            qDebug() << "emit signalCallHangup(HangupIncomingCall)" << endl;
            emit signalCallHangup("HangupIncomingCall");
        }
        else{
            emit signalCallHangup("Hangup");
            qDebug() << "emit signalCallHangup(Hangup)" << endl;
        }

        this->phoneWidget->phone->IncomingEnabled = FALSE;
        this->callHangupButton->setIcon(QIcon("icons/CallButton.png"));
        this->EnableButton = TRUE;
        qDebug() << "Call Ended";
    }
}
