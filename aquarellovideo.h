#ifndef AQUARELL_VIDEO_H_
#define AQUARELL_VIDEO_H_

//#include <QApplication>
#include <QCheckBox>
#include <QFont>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QObject>
#include <QStatusBar>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QWidget>
#include <QTimer>
#include <QFileSystemModel>
#include <QDebug>
#include <QtMultimedia/QMediaPlayer>
//#include <QtMultimediaWidgets/QVideoWidget>
#include <QtMultimedia/QMediaPlaylist>
//#include <QtWidgets/QGraphicsScene>
//#include <QtWidgets/QGraphicsView>

#include <QCoreApplication>
#include <assert.h>
#include <QMessageBox>
#include <QString>

#include "picturecatalog.h"
#include "videocatalog.h"
#include "aquarellosettings.h"
#include "widgetcall.h"
#include "softphone.h"
#include "contactsettings.h"
#include "barcodecatalog.h"
#include "mybutton.h"
#include <iostream>


namespace AQUA_STATE {
    typedef enum {
        WELCOME         = 0,
        CATALOG_PICTURE = 1,
        CATALOG_VIDEO   = 2,
        CATALOG_BARCODE = 3,
        TABLE_CALLER    = 4,
        WAITING_RESPONSE = 5,
        CALL            = 6,
        INCALL            = 7
    } AquarelloState;
}


class AquarelloSettings;

class AquarelloVideo : public QWidget
{
    Q_OBJECT

public:
    AquarelloVideo(QWidget *parent = 0);
    virtual ~AquarelloVideo();
    static AquarelloVideo *instance();
    void resizeEvent(QResizeEvent *event);
    bool usingSoftphone();
    //void Initialize(AquarelloSettings *);

signals:
    void signalVideoCatalogHasFinished();
    //SoftphoneWidget
    void signalCallHangup(QString);
    void stopThread();
    void startThread();

public slots:
    void onBarCodeInterruption();
    void barcodeCatalogFinished();
    void timeOutInterruption();
    void videoFinished(QMediaPlayer::MediaStatus mediaStatus);
    void onCallReleased();
    void onCall();
    void inCall();
    void onWaitResponse();
    void quit();
    //SoftphoneWidget
    void onCallButton_01();
    void onCallButton_02();
    void onCallButton_03();
    void onCallButton_04();
    void onCallButton_05();
    void onCallButton_06();
    void onCallButton_07();
    void onCallButton_08();
    void onCallButton_09();
    void onCallButton_10();
    void onHangupButton();
    void onAnswerButton();
    void onEndCall();
    void onTableCaller();
    void onTablereturn();
    void onRegistrationFailed();
    void onRegistrationOk();

private:
    static AquarelloVideo *theInstance_;
    bool EnableButton;

    QLabel          * welcome_catalog;
    PictureCatalog  * picture_catalog;
    VideoCatalog    * video_catalog;
    BarcodeCatalog  * barcode_catalog;
    QLabel          * wait_response;
    Softphone       * phoneWidget;
    MyButton     * callHangupButton;
    QThread         * thread;
    ContactSettings * contacts;

    WidgetCall      * widgetcall;

    QTimer *timer;
    QTimer *timerTableCaller;

    QString PATH;

    void delay(int delay_sec);
    void initLayout();
    AQUA_STATE::AquarelloState aqState;
    void setStateEnviroment(AQUA_STATE::AquarelloState aqState);
    void launchStateEnviroment(AQUA_STATE::AquarelloState aqState);
    void resizeCallHangupButton();
    AquarelloSettings * settings;
    QString getImageFilePath(QString,QString,QString);
    QString welcomeImagePath;
    QString callImagePath;
    QString waitImagePath;
    QString callButtonImagePath;
    QString endCallButtonImagePath;
    QString sellerButtonImagePath;
    QString contactsBackImagePath;
    QString returnBackImagePath;
    QString callButtonVideImagePath;
    QString mainIconImagePath;

    bool RegSoft=false;
};


#endif


