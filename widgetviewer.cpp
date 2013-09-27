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

    AqVideo = new AquarelloVideo;
    AqVideo->setWindowFlags(Qt::Window);
    WidgetItem = Scene.addWidget(AqVideo);

    QSize displaySize = QApplication::desktop()->screenGeometry(this).size();
    qDebug() << "displaySize: " << displaySize;
    this->setFixedSize(displaySize);
    QString PATH = QCoreApplication::applicationDirPath();
    AquarelloSettings screen(this, PATH + "/settings/aquarello.xml");
    //Scene.setSceneRect(0, 0, this->width(), this->height());

    if(QString::compare(screen.orientation, "landscape", Qt::CaseInsensitive)==0){
        // Orientation fixed to landscape
        WidgetItem->setRotation(0);
        WidgetItem->resize(QSizeF(this->width(), this->height()));
    }else{
        // Orientation fixed to portrait
        WidgetItem->setRotation(270);
        WidgetItem->resize(QSizeF(this->height(),this->width()));
        //WidgetItem->setGeometry(QRectF(0, 0, this->height(), this->width()));
        WidgetItem->setPos(QPointF(0, this->height()));
    }

    WidgetItem->setAcceptTouchEvents(true);
    qApp->setAttribute(Qt::AA_SynthesizeMouseForUnhandledTouchEvents, true);
    AqVideo->setAttribute(Qt::WA_AcceptTouchEvents, true);
}
