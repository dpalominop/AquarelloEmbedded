#include "mybutton.h"

MyButton::MyButton(QWidget *parent) :
    QPushButton(parent)
{
}

bool MyButton::event(QEvent * qevent)
{
    if (qevent->type() == QEvent::MouseButtonPress) {
        emit clicked(true);
        qDebug() << "MP in QPushButton" << this->objectName() << endl;
    }

    return QPushButton::event(qevent);
}
