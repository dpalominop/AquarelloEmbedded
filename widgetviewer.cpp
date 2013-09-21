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
    //AqVideo->setParent(this, Qt::Window);
    AqVideo->setWindowFlags(Qt::Window);
    WidgetItem = Scene.addWidget(AqVideo);
    //Scene.addItem(WidgetItem);

    QString PATH = QCoreApplication::applicationDirPath();
    AquarelloSettings screen(this, PATH + "/settings/aquarello.xml");

    if(QString::compare(screen.orientation, "landscape", Qt::CaseInsensitive)==0){
        // Orientation fixed to landscape
        //AqVideo->setFixedSize(screen.width, screen.height);
        Scene.setSceneRect(0, 0, screen.width, screen.height);
        WidgetItem->setRotation(0);
        WidgetItem->resize(QSizeF(screen.width, screen.height));
        //WidgetItem->setGeometry(QRectF(0, 0, 1000, 700));
        //WidgetItem->setPos(QPointF(0, 1000));
    }else{
        // Orientation fixed to portrait
        //AqVideo->setFixedSize(screen.height, screen.width);
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
