#ifndef SOFTPHONEWIDGET_H
#define SOFTPHONEWIDGET_H

#include <QWidget>
#include "softphone.h"

class SoftphoneWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SoftphoneWidget(QWidget *parent = 0);
    
signals:
    
public slots:

public:
    Softphone *phone;
    
};

#endif // SOFTPHONEWIDGET_H
