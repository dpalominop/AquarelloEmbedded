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

    if(QString::compare(screen.orientation, "landscape", Qt::CaseInsensitive)==0){
        // Orientation fixed to landscape
        AqVideo->setFixedSize(screen.width, screen.height);
        Scene.setSceneRect(0, 0, screen.width, screen.height);
        WidgetItem->setRotation(0);
        WidgetItem->resize(QSizeF(screen.width, screen.height));
        //WidgetItem->setGeometry(QRectF(0, 0, 1000, 700));
        //WidgetItem->setPos(QPointF(0, 1000));
    }else{
        // Orientation fixed to portrait
        AqVideo->setFixedSize(screen.height, screen.width);
        Scene.setSceneRect(0, 0, screen.width, screen.height);
        WidgetItem->setRotation(270);
        WidgetItem->resize(QSizeF(screen.height,screen.width));
        WidgetItem->setGeometry(QRectF(0, 0, screen.height, screen.width));
        WidgetItem->setPos(QPointF(0, screen.height));
    }

    this->setFixedSize(screen.width, screen.height);

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
