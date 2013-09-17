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
    bool touchBeginEvent(QTouchEvent *qevent);
    bool touchUpdateEvent(QTouchEvent *qevent);
    void mousePressEvent(QGraphicsSceneMouseEvent *qevent);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *qevent);
    bool touchEndEvent(QTouchEvent *qevent);
signals:

public slots:

};

#endif // MYBUTTON_H
