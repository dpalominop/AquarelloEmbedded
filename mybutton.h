#ifndef MYBUTTON_H
#define MYBUTTON_H

#include <QWidget>
#include <QDebug>
#include <QPushButton>
#include <QTouchEvent>
#include <QGestureEvent>
#include <QGraphicsSceneMouseEvent>
#include <QGuiApplication>

class MyButton : public QPushButton
{
    Q_OBJECT
public:
    explicit MyButton(QWidget *parent = 0);

    bool event(QEvent *qevent);
signals:

public slots:

};

#endif // MYBUTTON_H
