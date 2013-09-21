#ifndef WIDGETVIEWER_H
#define WIDGETVIEWER_H

#include <QWidget>
#include <QPointF>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsWebView>
#include <QGraphicsWidget>
#include <QWebFrame>
#include <QGuiApplication>
#include <QGraphicsProxyWidget>
#include "aquarellovideo.h"
#include "widgetcall.h"

class WidgetViewer : public QGraphicsView
{
    Q_OBJECT
public:
    explicit WidgetViewer(QWidget *parent = 0);

signals:
    void clicked();
public slots:

private:
    QGraphicsScene Scene;
    QGraphicsWidget *WidgetItem;
    AquarelloVideo *AqVideo;
    WidgetCall *w;
};

#endif // WIDGETVIEWER_H
