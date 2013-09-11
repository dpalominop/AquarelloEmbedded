#ifndef WIDGETCALL_H
#define WIDGETCALL_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>

class WidgetCall : public QWidget
{
    Q_OBJECT
public:
    explicit WidgetCall(QWidget *parent = 0);

    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout_4;
    QPushButton *dest_01;
    QLabel *message;
    QPushButton *dest_06;
    QPushButton *dest_07;
    QPushButton *dest_03;
    QPushButton *dest_05;
    QPushButton *dest_02;
    QPushButton *dest_04;
    QPushButton *dest_08;
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
