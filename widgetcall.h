#ifndef WIDGETCALL_H
#define WIDGETCALL_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include "mybutton.h"
#include <QLineEdit>

class WidgetCall : public QWidget
{
    Q_OBJECT
public:
    explicit WidgetCall(QWidget *parent = 0, QString icon_background="/icons7/call_button_vide.png");

    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout_4;
    MyButton *dest_01;
    QLabel *message;
    MyButton *dest_06;
    MyButton *dest_07;
    MyButton *dest_03;
    MyButton *dest_05;
    MyButton *dest_02;
    MyButton *dest_04;
    MyButton *dest_08;
    MyButton *dest_09;
    MyButton *dest_10;
    QSpacerItem *verticalSpacer;
    QGridLayout *gridLayout_2;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *verticalSpacer_5;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *verticalSpacer_4;
    QSpacerItem *horizontalSpacer_6;
    QSpacerItem *horizontalSpacer_5;
    QSpacerItem *horizontalSpacer_4;
    QSpacerItem *verticalSpacer_2;
    
signals:
    
public slots:
    
};

#endif // WIDGETCALL_H
