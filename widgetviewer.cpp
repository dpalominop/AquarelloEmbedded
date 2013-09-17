#include "widgetviewer.h"

WidgetViewer::WidgetViewer(QWidget *parent) :
    QGraphicsView(parent)
{
    Scene.setItemIndexMethod(QGraphicsScene::NoIndex);
    setScene(&Scene);
    setCacheMode(CacheBackground);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);
    setResizeAnchor(AnchorViewCenter);

    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    AqVideo = new AquarelloVideo();
    WidgetItem = Scene.addWidget(AqVideo);
    //Scene.addItem(WidgetItem);

    QString PATH = QCoreApplication::applicationDirPath();
    AquarelloSettings screen(this, PATH + "/settings/aquarello.xml");

    if(screen.orientation == QString("landscape")){
        // Orientation fixed to landscape
        AqVideo->setFixedSize(1024, 768);
        Scene.setSceneRect(0, 0, 1024, 768);
        WidgetItem->setRotation(0);
        WidgetItem->resize(QSizeF(1024, 768));
        //WidgetItem->setGeometry(QRectF(0, 0, 1000, 700));
        //WidgetItem->setPos(QPointF(0, 1000));
    }else{
        // Orientation fixed to portrait
        AqVideo->setFixedSize(768, 1024);
        Scene.setSceneRect(0, 0, 1024, 768);
        WidgetItem->setRotation(270);
        WidgetItem->resize(QSizeF(768,1024));
        WidgetItem->setGeometry(QRectF(0, 0, 768, 1024));
        WidgetItem->setPos(QPointF(0, 768));
    }

    this->setFixedSize(1024, 768);

    WidgetItem->setAcceptTouchEvents(true);
    qApp->setAttribute(Qt::AA_SynthesizeMouseForUnhandledTouchEvents, true);
    AqVideo->setAttribute(Qt::WA_AcceptTouchEvents, true);
}

bool WidgetViewer::event(QEvent * qevent)
{
    /*if (qevent->type() == QEvent::Gesture) {
       //return gestureEvent(static_cast<QGestureEvent*>(event));
    }
    else */if (qevent->type() == QEvent::TouchBegin) {
       return touchBeginEvent(static_cast<QTouchEvent*>(qevent));
    }
    else if (qevent->type() == QEvent::TouchUpdate) {
       return touchUpdateEvent(static_cast<QTouchEvent*>(qevent));
    }
    else if (qevent->type() == QEvent::TouchEnd) {
       //return touchEndEvent(static_cast<QTouchEvent*>(event));
    }else if (qevent->type() == QEvent::MouseButtonPress) {
        qDebug() << "MP in QGRapchisView" << endl;
     }

    return QGraphicsView::event(qevent);
}

bool WidgetViewer::touchBeginEvent(QTouchEvent *qevent)
{
    qDebug() << "TB in QGraphicsView" << qevent;
    qevent->accept();
    return true;
}

bool WidgetViewer::touchUpdateEvent(QTouchEvent *qevent)
{
    qDebug() << "TU in QGraphicsView" << qevent;
    return true;
}

void WidgetViewer::mousePressEvent(QGraphicsSceneMouseEvent *qevent)
{
    qDebug() << "MP in QGraphicsView" << qevent;
}

void WidgetViewer::mouseMoveEvent(QGraphicsSceneMouseEvent *qevent)
{
    qDebug() << "MM in QGraphicsView" << qevent;
}
