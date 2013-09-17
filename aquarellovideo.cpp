#include "aquarellovideo.h"


AquarelloVideo *AquarelloVideo::theInstance_;

AquarelloVideo::AquarelloVideo(QWidget *parent)
: QWidget(parent),
  welcome_catalog(new QLabel(this,Qt::Widget)),
  timer(new QTimer(this)),
  aqState (AQUA_STATE::WELCOME)
  //widgetcall(new WidgetCall(this))
{
    QString PATH =QCoreApplication::applicationDirPath();
    settings = new AquarelloSettings(this, QString(PATH+"/settings/aquarello.xml"));

    theInstance_ = this;
    theInstance_->setWindowIcon(QIcon("icons/MainIcon.png"));

    if(settings->usingPictureCatalog)
        picture_catalog = new PictureCatalog(this);

    if(settings->usingBarcodeScanner)
    {
        barcode_catalog = new BarcodeCatalog(this);
        barcode_catalog->setSettings(settings);
        connect(barcode_catalog,SIGNAL(queryReceived()),this,SLOT(onBarCodeInterruption()));
        connect(barcode_catalog,SIGNAL(catalogTimeout()),this,SLOT(barcodeCatalogFinished()));
        connect(barcode_catalog,SIGNAL(quitRequest()),this,SLOT(quit()));
        barcode_catalog->start();

    }


    this->EnableButton = TRUE;
    this->callHangupButton = new MyButton(this);

    timerTableCaller = new QTimer(this);
    timerTableCaller->setSingleShot(true);

    if(settings->usingSoftPhone)
    {
            widgetcall = new WidgetCall(this);
            connect(timerTableCaller, SIGNAL(timeout()), this, SLOT(onTablereturn()));
            wait_response = new QLabel(this);
            contacts = new ContactSettings(this, PATH+"/settings/contacts.xml");

            thread = new QThread;
            phoneWidget = new Softphone;

            phoneWidget->moveToThread(thread);
            thread->start();

            connect( this->callHangupButton, SIGNAL(clicked()), this, SLOT(onTableCaller()) );
            if(contacts->contacts[0]["enabled"].toInt()) connect( this->widgetcall->dest_01, SIGNAL(clicked()), this, SLOT(onCallButton_01()) );
            if(contacts->contacts[1]["enabled"].toInt()) connect( this->widgetcall->dest_02, SIGNAL(clicked()), this, SLOT(onCallButton_02()) );
            if(contacts->contacts[2]["enabled"].toInt()) connect( this->widgetcall->dest_03, SIGNAL(clicked()), this, SLOT(onCallButton_03()) );
            if(contacts->contacts[3]["enabled"].toInt()) connect( this->widgetcall->dest_04, SIGNAL(clicked()), this, SLOT(onCallButton_04()) );
            if(contacts->contacts[4]["enabled"].toInt()) connect( this->widgetcall->dest_05, SIGNAL(clicked()), this, SLOT(onCallButton_05()) );
            if(contacts->contacts[5]["enabled"].toInt()) connect( this->widgetcall->dest_06, SIGNAL(clicked()), this, SLOT(onCallButton_06()) );
            if(contacts->contacts[6]["enabled"].toInt()) connect( this->widgetcall->dest_07, SIGNAL(clicked()), this, SLOT(onCallButton_07()) );
            if(contacts->contacts[7]["enabled"].toInt()) connect( this->widgetcall->dest_08, SIGNAL(clicked()), this, SLOT(onCallButton_08()) );

            QObject::connect( this->phoneWidget, SIGNAL(signalEndCall()), this, SLOT(onEndCall()), Qt::DirectConnection );
            QObject::connect( this->phoneWidget, SIGNAL(signalConnectedCall()), this, SLOT(onCall()), Qt::DirectConnection );
            QObject::connect( this->phoneWidget, SIGNAL(signalIncomingCall()), this, SLOT(inCall()), Qt::DirectConnection );

            QObject::connect(thread, SIGNAL(started()), phoneWidget, SLOT(process()));
            QObject::connect(phoneWidget, SIGNAL(finished()), thread, SLOT(quit()) );
            QObject::connect(phoneWidget, SIGNAL(finished()), phoneWidget, SLOT(deleteLater()));
            //connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
            QObject::connect(phoneWidget, SIGNAL(destroyed()), thread, SLOT(deleteLater()) );

    }
    if(settings->usingVideoCatalog)
    {
        video_catalog = new VideoCatalog(this);
        connect(video_catalog,SIGNAL(videoFinished(QMediaPlayer::MediaStatus)),this,SLOT(videoFinished(QMediaPlayer::MediaStatus)));
    }

    initLayout();
    connect(timer, SIGNAL(timeout()), this, SLOT(timeOutInterruption()));
    timer->start(settings->pictureCatalogDelay_ms);
    timeOutInterruption();
    resizeEvent(NULL);
}

AquarelloVideo::~AquarelloVideo()
{
    if(timer->isActive()){
        timer->stop();
    }
    if(settings->usingVideoCatalog){
        video_catalog->stopPlaylist();
        video_catalog->deleteLater();
    }
    barcode_catalog->stopScanning();
    barcode_catalog->deleteLater();

    if(settings->usingSoftPhone)
        phoneWidget->exitSoftphone = TRUE;
    delay(3);
    qApp->quit();
    this->deleteLater();

}

void AquarelloVideo::initLayout()
{
    Qt::WindowFlags flags = this->windowFlags();
    this->setWindowFlags(flags | Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint);

    welcome_catalog->setMaximumSize(this->size());
    welcome_catalog->resize(this->size());
    welcome_catalog->setAlignment(Qt::AlignCenter);
    welcome_catalog->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    welcome_catalog->setLineWidth(3);
    welcome_catalog->setStyleSheet("QLabel { background-color : white; color : black }");
    welcome_catalog->setTextFormat(Qt::RichText);
    welcome_catalog->setBackgroundRole(QPalette::Base);
    welcome_catalog->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    welcome_catalog->setScaledContents(true);

    QFont fontWelcome = welcome_catalog->font();
    fontWelcome.setPointSize(fontWelcome.pointSize() + 8);
    welcome_catalog->setFont(fontWelcome);

    welcome_catalog->setHidden(false);

    welcomeImagePath=getImageFilePath("welcome_picture","default_pictures","foreground");
    callImagePath=getImageFilePath("softphone_pictures","default_pictures","call");
    waitImagePath=getImageFilePath("softphone_pictures","default_pictures","please-wait");
    contactsBackImagePath=getImageFilePath("softphone_pictures","default_pictures","background-contacts");
    sellerButtonImagePath=getImageFilePath("icons","default_pictures","bouton_appel_vendeur");
    callButtonImagePath=getImageFilePath("icons","default_pictures","CallButton");
    endCallButtonImagePath=getImageFilePath("icons","default_pictures","EndCallButton");

    //qDebug() << "WELCOME: " << welcomeImagePath;

    if(settings->usingPictureCatalog)
    {
        picture_catalog->setMaximumSize(this->size());
        picture_catalog->resize(this->size());
        picture_catalog->setAlignment(Qt::AlignCenter);
        picture_catalog->setFrameStyle(QFrame::Panel | QFrame::Sunken);
        picture_catalog->setLineWidth(3);
        picture_catalog->setStyleSheet("QLabel { background-color : white; color : black }");
        picture_catalog->setTextFormat(Qt::RichText);
        picture_catalog->setBackgroundRole(QPalette::Base);
        picture_catalog->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
        picture_catalog->setScaledContents(true);

        QFont font = picture_catalog->font();
        font.setPointSize(font.pointSize() + 8);
        picture_catalog->setFont(font);

        picture_catalog->setHidden(true);
    }

    if(settings->usingVideoCatalog)
    {
        video_catalog->setMaximumSize(this->size());
        video_catalog->resize(this->size());
        video_catalog->setHidden(true);
    }

    if(settings->usingSoftPhone)
    {
        wait_response->setMaximumSize(this->size());
        wait_response->resize(this->size());
        wait_response->setAlignment(Qt::AlignCenter);
        wait_response->setFrameStyle(QFrame::Panel | QFrame::Sunken);
        wait_response->setLineWidth(3);
        wait_response->setStyleSheet("QLabel { background-color : white; color : black }");
        wait_response->setTextFormat(Qt::RichText);
        wait_response->setBackgroundRole(QPalette::Base);
        wait_response->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
        wait_response->setScaledContents(true);
        QFont fontWR = wait_response->font();
        fontWR.setPointSize(fontWR.pointSize() + 8);
        wait_response->setFont(fontWR);
        wait_response->setHidden(true);

        widgetcall->setMaximumSize(this->size());
        widgetcall->resize(this->size());
        widgetcall->dest_01->setIcon(QIcon(callButtonImagePath));
        widgetcall->dest_01->setIconSize(widgetcall->dest_01->size());
        widgetcall->dest_01->setText(contacts->contacts[0]["firstname"]+QString(" ")+contacts->contacts[0]["surname"]);
        if(!contacts->contacts[0]["enabled"].toInt()) widgetcall->dest_01->setHidden(true);
        widgetcall->dest_02->setIcon(QIcon(callButtonImagePath));
        widgetcall->dest_02->setIconSize(widgetcall->dest_02->size());
        widgetcall->dest_02->setText(contacts->contacts[1]["firstname"]+QString(" ")+contacts->contacts[1]["surname"]);
        if(!contacts->contacts[1]["enabled"].toInt()) widgetcall->dest_02->setHidden(true);
        widgetcall->dest_03->setIcon(QIcon(callButtonImagePath));
        widgetcall->dest_03->setIconSize(widgetcall->dest_03->size());
        widgetcall->dest_03->setText(contacts->contacts[2]["firstname"]+QString(" ")+contacts->contacts[2]["surname"]);
        if(!contacts->contacts[2]["enabled"].toInt()) widgetcall->dest_03->setHidden(true);
        widgetcall->dest_04->setIcon(QIcon(callButtonImagePath));
        widgetcall->dest_04->setIconSize(widgetcall->dest_04->size());
        widgetcall->dest_04->setText(contacts->contacts[3]["firstname"]+QString(" ")+contacts->contacts[3]["surname"]);
        if(!contacts->contacts[3]["enabled"].toInt()) widgetcall->dest_04->setHidden(true);
        widgetcall->dest_05->setIcon(QIcon(callButtonImagePath));
        widgetcall->dest_05->setIconSize(widgetcall->dest_05->size());
        widgetcall->dest_05->setText(contacts->contacts[4]["firstname"]+QString(" ")+contacts->contacts[4]["surname"]);
        if(!contacts->contacts[4]["enabled"].toInt()) widgetcall->dest_05->setHidden(true);
        widgetcall->dest_06->setIcon(QIcon(callButtonImagePath));
        widgetcall->dest_06->setIconSize(widgetcall->dest_06->size());
        widgetcall->dest_06->setText(contacts->contacts[5]["firstname"]+QString(" ")+contacts->contacts[5]["surname"]);
        if(!contacts->contacts[5]["enabled"].toInt()) widgetcall->dest_06->setHidden(true);
        widgetcall->dest_07->setIcon(QIcon(callButtonImagePath));
        widgetcall->dest_07->setIconSize(widgetcall->dest_07->size());
        widgetcall->dest_07->setText(contacts->contacts[6]["firstname"]+QString(" ")+contacts->contacts[6]["surname"]);
        if(!contacts->contacts[6]["enabled"].toInt()) widgetcall->dest_07->setHidden(true);
        widgetcall->dest_08->setIcon(QIcon(callButtonImagePath));
        widgetcall->dest_08->setIconSize(widgetcall->dest_08->size());
        widgetcall->dest_08->setText(contacts->contacts[7]["firstname"]+QString(" ")+contacts->contacts[7]["surname"]);
        if(!contacts->contacts[7]["enabled"].toInt()) widgetcall->dest_08->setHidden(true);
        widgetcall->setHidden(true);

        callHangupButton->setIcon(QIcon());
        //callHangupButton->setIcon(QIcon(callButtonImagePath));
        callHangupButton->setIcon(QIcon(sellerButtonImagePath));
        callHangupButton->setIconSize(QImage(sellerButtonImagePath).size());
        callHangupButton->resize(QImage(sellerButtonImagePath).size());
        //callHangupButton->adjustSize();
        callHangupButton->setHidden(true);
        //this->setStyleSheet("QPushButton {border-radius: 60px;}");
    }
    else
    {
        callHangupButton->setHidden(true);
    }

}

void AquarelloVideo::resizeCallHangupButton()
{
    //callHangupButton->adjustSize();

    float spacingFactor=1;
    QPoint callButtonPosition;
    callButtonPosition.setX(spacingFactor*(this->width()-callHangupButton->width()-10));
    callButtonPosition.setY(spacingFactor*(this->height()-callHangupButton->height()-10));
    callHangupButton->move(callButtonPosition);
}

void AquarelloVideo::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    welcome_catalog->setMaximumSize(this->size());
    welcome_catalog->resize(this->size());

    if(settings->usingPictureCatalog)
    {
        picture_catalog->setMaximumSize(this->size());
        picture_catalog->resize(this->size());
    }

    if(settings->usingVideoCatalog)
    {
        video_catalog->setMaximumSize(this->size());
        video_catalog->resize(this->size());
    }

    if(settings->usingSoftPhone)
    {
        wait_response->setMaximumSize(this->size());
        wait_response->resize(this->size());

        widgetcall->setMaximumSize(this->size());
        widgetcall->resize(this->size());

        resizeCallHangupButton();
    }
}

void AquarelloVideo::quit()
{
    qApp->quit();

}


void AquarelloVideo::delay(int delay_sec)
{
    QTime deathTime= QTime::currentTime().addSecs(delay_sec);
    while( QTime::currentTime() < deathTime )
    QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void AquarelloVideo::onBarCodeInterruption()
{
    if(timer->isActive()){
        timer->stop();
    }
    aqState = AQUA_STATE::CATALOG_BARCODE;
    setStateEnviroment(aqState);
    launchStateEnviroment(aqState);
    //timer->start();
}

void AquarelloVideo::videoFinished(QMediaPlayer::MediaStatus mediaStatus)
{
    if(mediaStatus == QMediaPlayer::EndOfMedia)
    {
        if(timer->isActive()){
            timer->stop();
        }
        timer->start();
        timeOutInterruption();
        qDebug() << "QMediaPlayer::EndOfMedia" << endl;
    }
    if(mediaStatus == QMediaPlayer::StoppedState)
    {
        qDebug() << "QMediaPlayer::StoppedState" << endl;
        if(timer->isActive()){
            timer->stop();
        }
    }

}

void AquarelloVideo::barcodeCatalogFinished()
{
    qDebug() << "barcodeCatalogFinished";
    if(EnableButton){
        if(timer->isActive()){
            timer->stop();
        }
        timer->start();
        aqState = AQUA_STATE::WELCOME;
        timeOutInterruption();
    }else{
        aqState = AQUA_STATE::CALL;
        setStateEnviroment(aqState);
        launchStateEnviroment(aqState);
    }
}

void AquarelloVideo::setStateEnviroment(AQUA_STATE::AquarelloState aqState)
{
    switch(aqState)    {
        case AQUA_STATE::WELCOME :
            welcome_catalog->setPixmap(QPixmap::fromImage(QImage(welcomeImagePath)));
            welcome_catalog->setHidden(false);
            if(settings->usingPictureCatalog)   picture_catalog->setHidden(true);
            if(settings->usingVideoCatalog)     video_catalog->setHidden(true);
            if(settings->usingVideoCatalog)     video_catalog->stopPlaylist();
            if(settings->usingSoftPhone)        callHangupButton->setHidden(false);
            if(settings->usingSoftPhone)        wait_response->setHidden(true);
            if(settings->usingSoftPhone)        widgetcall->setHidden(true);
            if(settings->usingBarcodeScanner)   barcode_catalog->setHidden(true);

            break;

        case AQUA_STATE::CATALOG_PICTURE :
            welcome_catalog->setHidden(true);
            if(settings->usingPictureCatalog)   picture_catalog->setHidden(false);
            if(settings->usingVideoCatalog)     video_catalog->setHidden(true);
            if(settings->usingVideoCatalog)     video_catalog->stopPlaylist();
            if(settings->usingSoftPhone)        callHangupButton->setHidden(false);
            if(settings->usingSoftPhone)        wait_response->setHidden(true);
            if(settings->usingSoftPhone)        widgetcall->setHidden(true);
            if(settings->usingBarcodeScanner)   barcode_catalog->setHidden(true);

            break;

        case AQUA_STATE::CATALOG_VIDEO :
            welcome_catalog->setHidden(true);
            if(settings->usingPictureCatalog)   picture_catalog->setHidden(true);
            if(settings->usingVideoCatalog)     video_catalog->setHidden(false);
            if(settings->usingSoftPhone)        callHangupButton->setHidden(false);
            if(settings->usingSoftPhone)        wait_response->setHidden(true);
            if(settings->usingSoftPhone)        widgetcall->setHidden(true);
            if(settings->usingBarcodeScanner)   barcode_catalog->setHidden(true);

            break;

        case AQUA_STATE::CATALOG_BARCODE :
            welcome_catalog->setHidden(true);
            if(settings->usingPictureCatalog)   picture_catalog->setHidden(true);
            if(settings->usingVideoCatalog)     video_catalog->setHidden(true);
            if(settings->usingVideoCatalog)     video_catalog->stopPlaylist();
            if(settings->usingSoftPhone)        callHangupButton->setHidden(true);
            if(settings->usingSoftPhone)        wait_response->setHidden(true);
            if(settings->usingSoftPhone)        widgetcall->setHidden(true);
            if(settings->usingBarcodeScanner)   barcode_catalog->setHidden(false);

            break;

        case AQUA_STATE::TABLE_CALLER :
            welcome_catalog->setHidden(false);
            if(settings->usingPictureCatalog)   picture_catalog->setHidden(true);
            if(settings->usingVideoCatalog)     video_catalog->setHidden(true);
            if(settings->usingVideoCatalog)     video_catalog->stopPlaylist();
            if(settings->usingSoftPhone)        callHangupButton->setHidden(false);
            if(settings->usingSoftPhone)        wait_response->setHidden(true);
            if(settings->usingSoftPhone)        widgetcall->setHidden(false);
            if(settings->usingBarcodeScanner)   barcode_catalog->setHidden(true);

            break;

        case AQUA_STATE::WAITING_RESPONSE :
            welcome_catalog->setHidden(true);
            if(settings->usingPictureCatalog)   picture_catalog->setHidden(true);
            if(settings->usingVideoCatalog)     video_catalog->setHidden(true);
            if(settings->usingVideoCatalog)     video_catalog->stopPlaylist();
            if(settings->usingSoftPhone)        callHangupButton->setHidden(false);
            if(settings->usingSoftPhone)        wait_response->setHidden(false);
            if(settings->usingSoftPhone)        widgetcall->setHidden(true);
            if(settings->usingBarcodeScanner)   barcode_catalog->setHidden(true);

            break;

        case AQUA_STATE::CALL :
            //delay(2);
            welcome_catalog->setHidden(true);
            if(settings->usingPictureCatalog)   picture_catalog->setHidden(true);
            if(settings->usingVideoCatalog)     video_catalog->setHidden(true);
            if(settings->usingVideoCatalog)     video_catalog->stopPlaylist();
            if(settings->usingSoftPhone)        callHangupButton->setHidden(false);
            if(settings->usingSoftPhone)        wait_response->setHidden(false);
            if(settings->usingSoftPhone)        widgetcall->setHidden(true);
            if(settings->usingBarcodeScanner)   barcode_catalog->setHidden(true);

            break;

        case AQUA_STATE::INCALL :
            //delay(2);
            welcome_catalog->setHidden(true);
            if(settings->usingPictureCatalog)   picture_catalog->setHidden(true);
            if(settings->usingVideoCatalog)     video_catalog->setHidden(true);
            if(settings->usingVideoCatalog)     video_catalog->stopPlaylist();
            if(settings->usingSoftPhone)        callHangupButton->setHidden(false);
            if(settings->usingSoftPhone)        wait_response->setHidden(false);
            if(settings->usingSoftPhone)        widgetcall->setHidden(true);
            if(settings->usingBarcodeScanner)   barcode_catalog->setHidden(true);

            break;

        default :
            ;   //EMPTY
    }
}

void AquarelloVideo::launchStateEnviroment(AQUA_STATE::AquarelloState state)
{
    switch(state)    {
        case AQUA_STATE::WELCOME :
            {
                timerTableCaller->stop();
                welcome_catalog->setPixmap(QPixmap::fromImage(QImage(welcomeImagePath)));

                if(timer->isActive()){
                    timer->stop();
                }
                timer->start();
                //if(!timer->isActive())  timer->start();
                //Swiching State
                if(settings->usingPictureCatalog)
                {
                    aqState = AQUA_STATE::CATALOG_PICTURE ;
                }   else if(settings->usingVideoCatalog)
                {
                    aqState = AQUA_STATE::CATALOG_VIDEO;
                }
                if(settings->usingSoftPhone)
                {
                    callHangupButton->setIcon(QIcon());
                    //callHangupButton->setIcon(QIcon(callButtonImagePath));
                    callHangupButton->setIcon(QIcon(sellerButtonImagePath));
                    //callHangupButton->setIconSize(QSize(280, 50));
                    callHangupButton->setIconSize(QImage(sellerButtonImagePath).size());
                    callHangupButton->resize(QImage(sellerButtonImagePath).size());
                    resizeCallHangupButton();
                    //callHangupButton->setIconSize(QSize(callHangupButton->width(), callHangupButton->height()));
                    //callHangupButton->resize(QImage(sellerButtonImagePath).size());

                    callHangupButton->raise();
                    welcome_catalog->stackUnder(callHangupButton);
                    welcome_catalog->lower();
                }
                break;
            }

        case AQUA_STATE::CATALOG_PICTURE :
            {
                timerTableCaller->stop();

                if(settings->usingSoftPhone)
                {
                    callHangupButton->setIcon(QIcon());
                    //callHangupButton->setIcon(QIcon(callButtonImagePath));
                    callHangupButton->setIcon(QIcon(sellerButtonImagePath));
                    //callHangupButton->setIconSize(QSize(280, 50));
                    callHangupButton->setIconSize(QImage(sellerButtonImagePath).size());
                    callHangupButton->resize(QImage(sellerButtonImagePath).size());
                    resizeCallHangupButton();
                    //callHangupButton->setIconSize(QSize(callHangupButton->width(), callHangupButton->height()));
                    //callHangupButton->resize(QImage(sellerButtonImagePath).size());

                    callHangupButton->raise();
                    picture_catalog->stackUnder(callHangupButton);
                    picture_catalog->lower();
                }


                picture_catalog->showNextPicture();
                if(picture_catalog->catalogIsFinished)
                {
                    if(settings->usingVideoCatalog)
                    {
                        aqState = AQUA_STATE::CATALOG_VIDEO;
                    }   else {
                        aqState = AQUA_STATE::WELCOME;
                    }
                }

                break;
            }

        case AQUA_STATE::CATALOG_VIDEO :
            {
                timerTableCaller->stop();

                if(settings->usingSoftPhone)
                {
                    callHangupButton->setIcon(QIcon());
                    //callHangupButton->setIcon(QIcon(callButtonImagePath));
                    callHangupButton->setIcon(QIcon(sellerButtonImagePath));
                    //callHangupButton->setIconSize(QSize(280, 50));
                    callHangupButton->setIconSize(QImage(sellerButtonImagePath).size());
                    callHangupButton->resize(QImage(sellerButtonImagePath).size());
                    resizeCallHangupButton();
                    //callHangupButton->setIconSize(QSize(callHangupButton->width(), callHangupButton->height()));
                    //callHangupButton->resize(QImage(sellerButtonImagePath).size());

                    callHangupButton->raise();
                    video_catalog->stackUnder(callHangupButton);
                    video_catalog->lower();
                }

                video_catalog->showNextVideo();

                if(video_catalog->catalogIsFinished)
                {
                    if(settings->usingPictureCatalog)
                    {
                    aqState = AQUA_STATE::CATALOG_PICTURE;
                    }   else
                    {
                    aqState = AQUA_STATE::WELCOME;
                    }

                }
                if(timer->isActive()){
                    timer->stop();//Don't worry! Timer'll be restarted when video's finished (through SLOT videoFinished)
                }
                break;
            }

            case AQUA_STATE::CATALOG_BARCODE :
            {
                timerTableCaller->stop();

                resizeCallHangupButton();
                if(settings->usingBarcodeScanner)
                {
                   if(timer->isActive()){
                       timer->stop();
                   }
                   //Swiching State
                   aqState = AQUA_STATE::CATALOG_PICTURE; //Timer is going to restart when usingBarcode is over.
                }
                break;
            }

            case AQUA_STATE::TABLE_CALLER  :
                {
                    if(timer->isActive()){
                        timer->stop();
                    }
//                    aqState = AQUA_STATE::CATALOG_PICTURE;
                    callHangupButton->setIcon(QIcon());
                    callHangupButton->setIcon(QIcon(endCallButtonImagePath));
                    //callHangupButton->setIconSize(QSize(100, 50));
                    callHangupButton->setIconSize(QImage(endCallButtonImagePath).size());
                    callHangupButton->resize(QImage(endCallButtonImagePath).size());
                    //callHangupButton->adjustSize();
                    resizeCallHangupButton();
                    //callHangupButton->setIconSize(QSize(callHangupButton->width(), callHangupButton->height()));
                    //callHangupButton->resize(QImage(endCallButtonImagePath).size());

                    //welcome_catalog->setPixmap(QPixmap::fromImage(QImage(callImagePath)));
                    welcome_catalog->setPixmap(QPixmap::fromImage(QImage(contactsBackImagePath)));

                    callHangupButton->raise();
                    welcome_catalog->stackUnder(callHangupButton);
                    welcome_catalog->lower();

                    timerTableCaller->stop();
                    timerTableCaller->start( settings->barcodeCatalogDelay_ms );
                    break;
                }

            case AQUA_STATE::WAITING_RESPONSE  :
                {
                    timerTableCaller->stop();
                    wait_response->setPixmap(QPixmap::fromImage(QImage(waitImagePath)));
                    if(timer->isActive()){
                        timer->stop();
                    }

                    callHangupButton->setIcon(QIcon());
                    callHangupButton->setIcon(QIcon(endCallButtonImagePath));
                    //callHangupButton->setIconSize(QSize(100, 50));
                    callHangupButton->setIconSize(QImage(endCallButtonImagePath).size());
                    callHangupButton->resize(QImage(endCallButtonImagePath).size());
                    resizeCallHangupButton();
                    //callHangupButton->setIconSize(QSize(callHangupButton->width(), callHangupButton->height()));
                    //callHangupButton->resize(QImage(endCallButtonImagePath).size());

                    callHangupButton->raise();
                    wait_response->stackUnder(callHangupButton);
                    wait_response->lower();

                    break;
                }
            case AQUA_STATE::CALL :
            {
                    timerTableCaller->stop();

                    if(timer->isActive()){
                        timer->stop();
                    }

                    wait_response->setPixmap(QPixmap::fromImage(QImage(callImagePath)));

                    callHangupButton->raise();
                    wait_response->stackUnder(callHangupButton);
                    wait_response->lower();

                    resizeCallHangupButton();

                    break;
            }
            case AQUA_STATE::INCALL :
            {
                timerTableCaller->stop();

                if(timer->isActive()){
                    timer->stop();
                }
                wait_response->setPixmap(QPixmap::fromImage(QImage(callImagePath)));

                callHangupButton->raise();
                wait_response->stackUnder(callHangupButton);
                wait_response->lower();

                callHangupButton->setIcon(QIcon(callButtonImagePath));
                callHangupButton->setIconSize(QImage(callButtonImagePath).size());
                callHangupButton->resize(QImage(callButtonImagePath).size());
                resizeCallHangupButton();

                break;
            }

        default :
                    ;   //EMPTY CASE

    }
}

void AquarelloVideo::timeOutInterruption()
{
    qDebug() << "timeOutInterruption: "<< aqState;
    if(aqState == AQUA_STATE::TABLE_CALLER){
        //aqState=AQUA_STATE::CATALOG_PICTURE;
        aqState=AQUA_STATE::WELCOME;
    }
    setStateEnviroment(aqState);
    launchStateEnviroment(aqState);
}

void AquarelloVideo::onCall()
{
    aqState=AQUA_STATE::CALL;
    setStateEnviroment(aqState);
    launchStateEnviroment(aqState);
}
void AquarelloVideo::inCall()
{
    qDebug() << "INCAll";
    aqState=AQUA_STATE::INCALL;
    setStateEnviroment(aqState);
    launchStateEnviroment(aqState);
}

void AquarelloVideo::onWaitResponse()
{
    aqState=AQUA_STATE::WAITING_RESPONSE;
    setStateEnviroment(aqState);
    launchStateEnviroment(aqState);
}

void AquarelloVideo::onCallReleased()
{
    qDebug() <<"CALL RELEASED FROM AQUARELLO"<<endl;
    if(timer->isActive()){
        timer->stop();
    }
    timer->start();

    //aqState=AQUA_STATE::CATALOG_PICTURE;
    aqState=AQUA_STATE::WELCOME;
    setStateEnviroment(aqState);
    launchStateEnviroment(aqState);
}

bool AquarelloVideo::usingSoftphone()
{
    return settings->usingSoftPhone;
}

void AquarelloVideo::onEndCall()
{
    qDebug() << "Call Ended FROM AQUARELLO";
    //this->callHangupButton->setIcon(QIcon(callButtonImagePath));
    this->callHangupButton->setIcon(QIcon(sellerButtonImagePath));
    callHangupButton->setIconSize(QImage(sellerButtonImagePath).size());
    callHangupButton->resize(QImage(sellerButtonImagePath).size());
    resizeCallHangupButton();
    this->EnableButton = TRUE;
    onCallReleased();
}

void AquarelloVideo::onTableCaller()
{
    qDebug() << "onTableCaller: " << aqState;
    switch (aqState) {
    case AQUA_STATE::WELCOME:
    case AQUA_STATE::CATALOG_PICTURE:
    case AQUA_STATE::CATALOG_VIDEO:
        if(timer->isActive()){
            timer->stop();
        }
        aqState=AQUA_STATE::TABLE_CALLER;
        setStateEnviroment(aqState);
        launchStateEnviroment(aqState);
        //timer->start(); //add
        break;

    case AQUA_STATE::TABLE_CALLER:
        if(timer->isActive()){
            timer->stop();
        }
        timer->start();

        //aqState=AQUA_STATE::CATALOG_PICTURE;
        aqState=AQUA_STATE::WELCOME;
        setStateEnviroment(aqState);
        launchStateEnviroment(aqState);
        break;

    case AQUA_STATE::WAITING_RESPONSE:
    case AQUA_STATE::CALL:
        onHangupButton();
        break;
    case AQUA_STATE::INCALL:
        onAnswerButton();
        break;
    default:
        onCallReleased();
        break;
    }
}

void AquarelloVideo::onTablereturn()
{
    timerTableCaller->stop();

    if(aqState==AQUA_STATE::TABLE_CALLER){

        if(settings->usingPictureCatalog)
            aqState=AQUA_STATE::CATALOG_PICTURE;
        else if (settings->usingVideoCatalog)
            aqState=AQUA_STATE::CATALOG_VIDEO;
        else aqState=AQUA_STATE::WELCOME;

        timeOutInterruption();
        timer->stop();
        if(!timer->isActive())  timer->start();
    }
}

void AquarelloVideo::onCallButton_01()
{
    this->callHangupButton->setIcon(QIcon(endCallButtonImagePath));
    this->EnableButton = FALSE;

    if(timer->isActive()){
        timer->stop();
    }

    if (phoneWidget->IncomingEnabled){
        phoneWidget->evento="AcceptingIncomingCall";
        this->onWaitResponse();
    }
    else{
        phoneWidget->evento=QString("Call,")+contacts->contacts[0]["phone"];
        this->onWaitResponse();
    }

    qDebug() << "Call Started FROM AQUARELLO";
}

void AquarelloVideo::onCallButton_02()
{
    this->callHangupButton->setIcon(QIcon(endCallButtonImagePath));
    this->EnableButton = FALSE;

    if(timer->isActive()){
        timer->stop();
    }

    if (phoneWidget->IncomingEnabled){
        phoneWidget->evento="AcceptingIncomingCall";
        this->onWaitResponse();
    }
    else{
        phoneWidget->evento=QString("Call,")+contacts->contacts[1]["phone"];
        this->onWaitResponse();
    }

    qDebug() << "Call Started FROM AQUARELLO";
}

void AquarelloVideo::onCallButton_03()
{
    this->callHangupButton->setIcon(QIcon(endCallButtonImagePath));
    this->EnableButton = FALSE;

    if(timer->isActive()){
        timer->stop();
    }

    if (phoneWidget->IncomingEnabled){
        phoneWidget->evento="AcceptingIncomingCall";
        this->onWaitResponse();
    }
    else{
        phoneWidget->evento=QString("Call,")+contacts->contacts[2]["phone"];
        this->onWaitResponse();
    }

    qDebug() << "Call Started FROM AQUARELLO";
}

void AquarelloVideo::onCallButton_04()
{
    this->callHangupButton->setIcon(QIcon(endCallButtonImagePath));
    this->EnableButton = FALSE;

    if(timer->isActive()){
        timer->stop();
    }

    if (phoneWidget->IncomingEnabled){
        phoneWidget->evento="AcceptingIncomingCall";
        this->onWaitResponse();
    }
    else{
        phoneWidget->evento=QString("Call,")+contacts->contacts[3]["phone"];
        this->onWaitResponse();
    }

    qDebug() << "Call Started FROM AQUARELLO";
}

void AquarelloVideo::onCallButton_05()
{
    this->callHangupButton->setIcon(QIcon(endCallButtonImagePath));
    this->EnableButton = FALSE;

    if(timer->isActive()){
        timer->stop();
    }

    if (phoneWidget->IncomingEnabled){
        phoneWidget->evento="AcceptingIncomingCall";
        this->onWaitResponse();
    }
    else{
        phoneWidget->evento=QString("Call,")+contacts->contacts[4]["phone"];
        this->onWaitResponse();
    }

    qDebug() << "Call Started FROM AQUARELLO";
}

void AquarelloVideo::onCallButton_06()
{
    this->callHangupButton->setIcon(QIcon(endCallButtonImagePath));
    this->EnableButton = FALSE;

    if(timer->isActive()){
        timer->stop();
    }

    if (phoneWidget->IncomingEnabled){
        phoneWidget->evento="AcceptingIncomingCall";
        this->onWaitResponse();
    }
    else{
        phoneWidget->evento=QString("Call,")+contacts->contacts[5]["phone"];
        this->onWaitResponse();
    }

    qDebug() << "Call Started FROM AQUARELLO";
}

void AquarelloVideo::onCallButton_07()
{
    this->callHangupButton->setIcon(QIcon(endCallButtonImagePath));
    this->EnableButton = FALSE;

    if(timer->isActive()){
        timer->stop();
    }

    if (phoneWidget->IncomingEnabled){
        phoneWidget->evento="AcceptingIncomingCall";
        this->onWaitResponse();
    }
    else{
        phoneWidget->evento=QString("Call,")+contacts->contacts[6]["phone"];
        this->onWaitResponse();
    }

    qDebug() << "Call Started FROM AQUARELLO";
}

void AquarelloVideo::onCallButton_08()
{
    this->callHangupButton->setIcon(QIcon(endCallButtonImagePath));
    this->EnableButton = FALSE;

    if(timer->isActive()){
        timer->stop();
    }

    if (phoneWidget->IncomingEnabled){
        phoneWidget->evento="AcceptingIncomingCall";
        this->onWaitResponse();
    }
    else{
        phoneWidget->evento=QString("Call,")+contacts->contacts[7]["phone"];
        this->onWaitResponse();
    }

    qDebug() << "Call Started FROM AQUARELLO";
}

void AquarelloVideo::onHangupButton()
{
/*    this->callHangupButton->setIcon(QIcon(sellerButtonImagePath));
    callHangupButton->setIconSize(QImage(sellerButtonImagePath).size());
    callHangupButton->resize(QImage(sellerButtonImagePath).size());
    resizeCallHangupButton();
*/    //
    this->EnableButton = TRUE;

    if (phoneWidget->IncomingEnabled){
        phoneWidget->evento="HangupIncomingCall";
    }
    else{
        phoneWidget->evento="Hangup";
    }
}
void AquarelloVideo::onAnswerButton()
{
    //this->phoneWidget->phone->IncomingEnabled = FALSE;
    this->callHangupButton->setIcon(QIcon(endCallButtonImagePath));
    callHangupButton->setIconSize(QImage(endCallButtonImagePath).size());
    callHangupButton->resize(QImage(endCallButtonImagePath).size());
    resizeCallHangupButton();
    this->EnableButton = FALSE;

    if (phoneWidget->IncomingEnabled){
        //emit signalCallHangup("HangupIncomingCall");
        phoneWidget->evento="AcceptingIncomingCall";
    }
    else{
        //emit signalCallHangup("Hangup");
        phoneWidget->evento="Hangup";
    }
}

QString AquarelloVideo::getImageFilePath(QString dirname,QString dirdefault, QString filename)
{
    QString PATH =QCoreApplication::applicationDirPath()+"/"+dirname;
    QString bname = "";
    bool existfile = false;

    QDir dir(PATH);
         dir.setFilter(QDir::Files | QDir::NoSymLinks);
    QFileInfoList list = dir.entryInfoList();

    QString pattern = QString("^%1.[png|jpeg|jpg|bmp|gif]+$").arg(filename);
    QRegExp r(pattern,Qt::CaseInsensitive);

    for (int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);

        if(r.exactMatch(fileInfo.fileName())){
            bname=fileInfo.absoluteFilePath();
            existfile = true;
            break;
        }
    }
    if(!existfile){
        PATH =QCoreApplication::applicationDirPath()+"/"+dirdefault;
        QDir dirdef(PATH);
             dirdef.setFilter(QDir::Files | QDir::NoSymLinks);
        QFileInfoList listdef = dirdef.entryInfoList();
        for (int p = 0; p < listdef.size(); ++p) {
            QFileInfo fileInfoDef = listdef.at(p);

            if(r.exactMatch(fileInfoDef.fileName())){
                bname=fileInfoDef.absoluteFilePath();
                existfile = true;
                break;
            }
        }
    }
    return bname;
}
