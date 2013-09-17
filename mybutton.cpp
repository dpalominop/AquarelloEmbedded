#include "mybutton.h"

MyButton::MyButton(QWidget *parent) :
    QPushButton(parent)
{
}

bool MyButton::event(QEvent * qevent)
{
    //qDebug() << "qevent->type():" << qevent->type() << endl;

    /*if (qevent->type() == QEvent::Gesture) {
       //return gestureEvent(static_cast<QGestureEvent*>(event));
    }
    else */
    if (qevent->type() == QEvent::TouchBegin) {

       emit clicked(true);
       //emit this->click();
       return touchBeginEvent(static_cast<QTouchEvent*>(qevent));

    }else if (qevent->type() == QEvent::TouchUpdate) {

       return touchUpdateEvent(static_cast<QTouchEvent*>(qevent));

    }else if (qevent->type() == QEvent::TouchEnd) {

       return touchEndEvent(static_cast<QTouchEvent*>(qevent));

    }else if (qevent->type() == QEvent::MouseButtonPress) {

        emit clicked(true);
        qDebug() << "MP in QPushButton" << this->objectName() << endl;

    }else if (qevent->type() == QEvent::MouseMove ) {

        qDebug() << "MM in QPushButton" << endl;
    }


    return QPushButton::event(qevent);
}

bool MyButton::touchBeginEvent(QTouchEvent *qevent)
{
    qDebug() << "TB in QPushButton" << qevent;
    qevent->accept();
    return true;
}

bool MyButton::touchUpdateEvent(QTouchEvent *qevent)
{
    qDebug() << "TU in QPushButton" << qevent;
    return true;
}

bool MyButton::touchEndEvent(QTouchEvent *qevent)
{
    //emit this->click();
    qevent->accept();
    qDebug() << "TE in QPushButton" << qevent;
    return true;
}

void MyButton::mousePressEvent(QGraphicsSceneMouseEvent *qevent)
{
    qDebug() << "MP in QPushButton" << qevent;
}

void MyButton::mouseMoveEvent(QGraphicsSceneMouseEvent *qevent)
{
    qDebug() << "MM in QPushButton" << qevent;
}
