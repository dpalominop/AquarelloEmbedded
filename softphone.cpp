#include "softphone.h"

Softphone::Softphone(QObject *parent) :
    QObject(parent)
{

}

Softphone::~Softphone()
{
    linphone_core_get_default_proxy(lc,&proxy_cfg); /* get default proxy config*/
    linphone_proxy_config_edit(proxy_cfg); /*start editing proxy configuration*/
    linphone_proxy_config_enable_register(proxy_cfg,FALSE); /*de-activate registration for this proxy config*/
    linphone_proxy_config_done(proxy_cfg); /*initiate REGISTER with expire = 0*/

    while(linphone_proxy_config_get_state(proxy_cfg) !=  LinphoneRegistrationCleared){
       linphone_core_iterate(lc); /*to make sure we receive call backs before shutting down*/
       ms_usleep(50000);
    }

    qDebug() << "Shutting down...\n";
    linphone_core_destroy(lc);
    qDebug() << "Exited\n";

    emit finished();
}

void Softphone::process(void)
{
    QString PATH =QCoreApplication::applicationDirPath();
    SoftPhoneSettings * settings =  new SoftPhoneSettings(this, QString(PATH+"/settings/softphone.xml"));
    AudioCodecSettings* audiocodecs = new AudioCodecSettings(this, QString(PATH + "/settings/audiocodecs.xml"));

    QString Identity            = QString("sip:") + settings->sipUserName + QString("@") + settings->sipDomain;
    QString Password            = settings->sipPassword;
    char dest[50]                  = "sip:157@190.187.84.221";
    qDebug() << "Identity: " << Identity;
    IncomingEnabled = FALSE;
    //int winId;

    //IncomingEnabled = FALSE;
    signal(SIGINT,stop);

    linphone_core_enable_logs(NULL); /*enable liblinphone logs.*/

    vtable.registration_state_changed=registration_state_changed;
    vtable.call_state_changed=call_state_changed;

    /*
    Instanciate a LinphoneCore object given the LinphoneCoreVTable
    */
    lc=linphone_core_new(&vtable,NULL,NULL,NULL);
    linphone_core_enable_video_preview(lc,false);
    linphone_core_enable_video(lc, false, false);
    //linphone_core_set_native_video_window_id(lc,winId);
    //linphone_core_set_native_preview_window_id(lc,winId);

    /*create proxy config*/
    proxy_cfg = linphone_proxy_config_new();
    /*parse identity*/
    LinphoneAddress *from = linphone_address_new(Identity.toLatin1().data());
    if (from==NULL){
    //printf("%s not a valid sip uri, must be like sip:toto@sip.linphone.org \n",identity);
    return;
    }
    LinphoneAuthInfo *info;
    if (Password.size() > 0){
    info=linphone_auth_info_new(linphone_address_get_username(from),NULL,Password.toLatin1().data(),NULL,NULL); /*create authentication structure from identity*/
    linphone_core_add_auth_info(lc,info); /*add authentication info to LinphoneCore*/
    }

    // configure proxy entries
    linphone_proxy_config_set_identity(proxy_cfg, Identity.toLatin1().data()); /*set identity with user name and domain*/
    const char* server_addr = linphone_address_get_domain(from); /*extract domain address from identity*/
    linphone_proxy_config_set_server_addr(proxy_cfg,server_addr); /* we assume domain = proxy server address*/
    linphone_proxy_config_enable_register(proxy_cfg,TRUE); /*activate registration for this proxy config*/
    linphone_address_destroy(from); /*release resource*/

    linphone_core_add_proxy_config(lc,proxy_cfg); /*add proxy config to linphone core*/
    linphone_core_set_default_proxy(lc,proxy_cfg); /*set to default proxy*/

    do {
         linphone_core_iterate(lc); /* first iterate initiates registration */
         ms_usleep(1000000);
    }while(  running && linphone_proxy_config_get_state(proxy_cfg) == LinphoneRegistrationProgress);


    QByteArray path_ringback = (PATH+"/sounds/ringback.wav").toLatin1();
    linphone_core_set_ringback(lc, path_ringback.data());

    //Enable codec of video (H264)
    linphonec_codec_disable(VIDEO,lc, -1);
    //linphonec_codec_enable(VIDEO, lc, 0);
    //Enable codec of audio (PCMA)
    linphonec_codec_disable(AUDIO, lc, -1);
    QMap<QString,int> codec_list;
    linphonec_codec_list(AUDIO, lc, codec_list);
    for(int i=0; i< audiocodecs->codecs.length(); i++){
        if(audiocodecs->codecs[i]["enabled"].toInt()){
            QString codec_name = audiocodecs->codecs[i]["name"]
                         +"/"+audiocodecs->codecs[i]["clock"]
                         +"/"+audiocodecs->codecs[i]["channel"];
            linphonec_codec_enable(AUDIO, lc, codec_list[codec_name]);
        }
    }
    //linphonec_codec_enable(AUDIO, lc, -1);

     /* main loop for receiving notifications and doing background linphonecore work: */
     while(running){
        linphone_core_iterate(lc); /* first iterate initiates registration */

        if (CallIncoming){
            emit signalIncomingCall();
            qDebug() << "EMIT SIGNAL CALL-INCOMING";
            this->IncomingEnabled = TRUE;
            CallIncoming = FALSE;
            CallConnected = FALSE;
            CallEnd = FALSE;
        }else if(CallConnected){
            qDebug() << "EMIT SIGNAL CALL-CONNECTED";
            emit signalConnectedCall();
            CallIncoming = FALSE;
            CallConnected = FALSE;
            CallEnd = FALSE;
        }
        else if(CallEnd){
            qDebug() << "EMIT SIGNAL CALL-END";
            emit signalEndCall();
            this->IncomingEnabled = FALSE;
            CallIncoming = FALSE;
            CallConnected = FALSE;
            CallEnd = FALSE;
        }

        QMutex mutex;
        mutex.lock();

        if((l_state == LinphoneRegistrationFailed) || (l_state==LinphoneRegistrationNone) || (l_state==LinphoneRegistrationCleared)) {emit RegistrationFailed();}
        else {emit RegistrationOk();}

        if(exitSoftphone) running = FALSE;

        QStringList list= evento.split(",");

        if (list.at(0) == "Call"){
         //QByteArray d      = QString(list.at(1)).toLatin1();
         QByteArray d      = QString("sip:" + list.at(1) + "@" +settings->sipDomain).toLatin1();
         strcpy(dest, d.data());
        }

        callEvents(QString(list.at(0)), dest);
        evento="";
        mutex.unlock();

        ms_usleep(50000);

    }
     linphone_core_get_default_proxy(lc,&proxy_cfg); /* get default proxy config*/
     linphone_proxy_config_edit(proxy_cfg); /*start editing proxy configuration*/
     linphone_proxy_config_enable_register(proxy_cfg,FALSE); /*de-activate registration for this proxy config*/
     linphone_proxy_config_done(proxy_cfg); /*initiate REGISTER with expire = 0*/

     while(linphone_proxy_config_get_state(proxy_cfg) !=  LinphoneRegistrationCleared){
        linphone_core_iterate(lc); /*to make sure we receive call backs before shutting down*/
        ms_usleep(50000);
     }

     qDebug() << "Shutting down...\n";
     linphone_core_destroy(lc);
     qDebug() << "Exited\n";

}

void Softphone::finSoft()
{
        linphone_core_get_default_proxy(lc,&proxy_cfg); /* get default proxy config*/
        linphone_proxy_config_edit(proxy_cfg); /*start editing proxy configuration*/
        linphone_proxy_config_enable_register(proxy_cfg,FALSE); /*de-activate registration for this proxy config*/
        linphone_proxy_config_done(proxy_cfg); /*initiate REGISTER with expire = 0*/

        while(linphone_proxy_config_get_state(proxy_cfg) !=  LinphoneRegistrationCleared){
           linphone_core_iterate(lc); /*to make sure we receive call backs before shutting down*/
           ms_usleep(50000);
        }

        qDebug() << "Shutting down...\n";
        linphone_core_destroy(lc);
        qDebug() << "Exited\n";
}
