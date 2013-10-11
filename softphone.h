#ifndef SOFTPHONE_H
#define SOFTPHONE_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QWidget>
#include <QDebug>
#include <QThread>
#include <QMutex>
#include <QCoreApplication>
#include "linphone/linphonecore.h"
#include "linphone/lpconfig.h"
#include "softphonesettings.h"
#include "audiocodecsettings.h"
#include <signal.h>

#define AUDIO 0
#define VIDEO 1

static bool CallIncoming = FALSE;
static bool CallEnd = FALSE;
static bool CallConnected = FALSE;

static int cont_calls = 0;

static LinphoneProxyConfig* proxy_cfg;
static LinphoneCoreVTable vtable={0};

static bool_t running=TRUE;
static LinphoneCall *call = NULL;
static LinphoneCore *lc = NULL;

static LinphoneRegistrationState l_state=LinphoneRegistrationNone;

static void stop(int signum){
    qDebug() << "STOP";
        running=FALSE;
}
static void registration_state_changed(struct _LinphoneCore *lc, LinphoneProxyConfig *cfg, LinphoneRegistrationState cstate, const char *message){
                qDebug() << "New registration state "<< linphone_registration_state_to_string(cstate)
                         << "for user id" << linphone_proxy_config_get_identity(cfg)
                         << "at proxy " << linphone_proxy_config_get_addr(cfg) << endl;
                l_state = cstate;
}

static void call_state_changed(LinphoneCore *lc, LinphoneCall *call, LinphoneCallState cstate, const char *msg){
        switch(cstate){
                case LinphoneCallIdle:
                        qDebug() << "Call failure !";
                break;
                case LinphoneCallIncomingReceived:
                        qDebug() << "Incoming Call ... !\n";
                        CallIncoming = TRUE;
                        CallConnected = FALSE;
                        CallEnd = FALSE;
                break;
                case LinphoneCallOutgoingInit:
                        qDebug() << "Call Outgoing is Init !";
                break;
                case LinphoneCallOutgoingProgress:
                        qDebug() << "Call Outgoing in Progress ...\n";
                break;
                case LinphoneCallOutgoingRinging:
                        qDebug() << "It is now ringing remotely !\n";
                break;
                case LinphoneCallOutgoingEarlyMedia:
                        qDebug() << "Receiving some early media\n";
                break;
                case LinphoneCallConnected:
                        qDebug() << "We are connected !\n";
                        CallIncoming = FALSE;
                        CallConnected = TRUE;
                        CallEnd = FALSE;
                break;
                case LinphoneCallStreamsRunning:
                        qDebug() << "Media streams established !\n";
                break;
                case LinphoneCallPausing:
                        qDebug() << "Call is Pausing ...\n";
                break;
                case LinphoneCallPaused:
                        qDebug() << "Call is Paused!\n";
                break;
                case LinphoneCallResuming:
                        qDebug() << "Call Resuming ... \n";
                break;
                case LinphoneCallRefered:
                        qDebug() << "Call is Refered !\n";
                break;
                case LinphoneCallError:
                        qDebug() << "Call failure !\n";
                case LinphoneCallEnd:
                        qDebug() << "Call is terminated.\n";
                        CallIncoming = FALSE;
                        CallConnected = FALSE;
                        CallEnd = TRUE;
                        cont_calls ++;
                        qDebug() << "Cantidad de llamadas: " << cont_calls << endl;
                break;
                case LinphoneCallPausedByRemote:
                        qDebug() << "Call Paused By Remote !\n";
                break;
                case LinphoneCallUpdatedByRemote:
                        qDebug() << "Call Updated By Remote !\n";
                break;
                case LinphoneCallIncomingEarlyMedia:
                        qDebug() << "Call Incoming Early Media !\n";
                break;
                /*case LinphoneCallUpdated:
                        qDebug() << "Call is Updated !\n";*/
                break;
                case LinphoneCallReleased:
                        qDebug() << "Call is Released !";
                break;
                default:
                        qDebug() << "Unhandled notification " << cstate <<"\n";
        }
}

static void linphonec_codec_list(int type, LinphoneCore *lc, QMap<QString,int> &codec_list){
    PayloadType *pt;
    int index=0;
    const MSList *node=NULL;

    if (type == AUDIO) {
        node=linphone_core_get_audio_codecs(lc);
    } else if(type==VIDEO) {
        node=linphone_core_get_video_codecs(lc);
    }

    for(;node!=NULL;node=ms_list_next(node)){
        pt=(PayloadType*)(node->data);

        QString codec_name = QString(pt->mime_type) + "/"
                        + QString::number(pt->clock_rate) + "/"
                + ((pt->channels == 2)?"stereo":"mono");
        codec_list[codec_name] = index;

        index++;
    }

}

static void linphonec_codec_enable(int type, LinphoneCore *lc, int sel_index){
    PayloadType *pt;
    int index=0;
    const MSList *node=NULL;

    if (type == AUDIO) {
        node=linphone_core_get_audio_codecs(lc);
    } else if(type==VIDEO) {
        node=linphone_core_get_video_codecs(lc);
    }

    for(;node!=NULL;node=ms_list_next(node)){
        if (index == sel_index || sel_index == -1) {
            pt=(PayloadType*)(node->data);
            linphone_core_enable_payload_type (lc,pt,TRUE);
            qDebug() << "index: " << index << ", pt->mime_type" << pt->mime_type << ", pt->clock_rate" << pt->clock_rate << ", enabled";
        }
        index++;
    }
}

static void linphonec_codec_disable(int type, LinphoneCore *lc, int sel_index){
    PayloadType *pt;
    int index=0;
    const MSList *node=NULL;

    if (type == AUDIO) {
        node=linphone_core_get_audio_codecs(lc);
    } else if(type==VIDEO) {
        node=linphone_core_get_video_codecs(lc);
    }

    for(;node!=NULL;node=ms_list_next(node)){
        if (index == sel_index || sel_index == -1) {
            pt=(PayloadType*)(node->data);
            linphone_core_enable_payload_type (lc,pt,FALSE);
            qDebug() << "index: " << index << ", pt->mime_type" << pt->mime_type << ", pt->clock_rate" << pt->clock_rate << ", disabled";
        }
        index++;
    }
}

static int lpc_cmd_call_logs(LinphoneCore *lc)
{
    const MSList *elem=linphone_core_get_call_logs(lc);
    for (;elem!=NULL;elem=ms_list_next(elem))
    {
        LinphoneCallLog *cl=(LinphoneCallLog*)elem->data;
        char *str=linphone_call_log_to_str(cl);
        qDebug() << "LOGS: " << str << endl;
        ms_free(str);
    }
    return 1;
}



static void callEvents(QString event, char * dest)
{
    //qDebug() << "CALL EVENTS:" << event;
    if(event == "Call"){

        call=linphone_core_invite(lc,dest);
        if (call==NULL){
            qDebug() << "Could not place call to : " << dest << endl;
        }else qDebug() << "Call to " << dest <<" is in progress...";

    }
    else if (event == "AcceptingIncomingCall"){

        int nb=ms_list_size(linphone_core_get_calls(lc));

        if (nb==1){
            //if just one call is present answer the only one in passing NULL to the linphone_core_accept_call ...
            //linphone_core_set_native_video_window_id(lc,this->winId);
            call=linphone_core_get_current_call(lc);
            if ( -1 == linphone_core_accept_call(lc, call) )
            {
                qDebug() << "Fail to accept incoming call\n";
            }
            //linphone_call_ref(call);
        }

    }
    else if(event == "Hangup"){

        qDebug() << "Iniciando colgado ..." << endl;

        call=linphone_core_get_current_call(lc);

        if (call && linphone_call_get_state(call)!=LinphoneCallEnd){
            qDebug() << "Pasando a ..." << endl;
            linphone_core_terminate_call(lc, call);
        }
        qDebug() << "Finalizando Colgado ..." << endl;

    }
    else if(event == "HangupIncomingCall"){

        qDebug() << "Iniciando colgado ..." << endl;

        call=linphone_core_get_current_call(lc);

        if (call && linphone_call_get_state(call)!=LinphoneCallEnd){
            qDebug() << "Pasando a ..." << call <<endl;
            linphone_core_terminate_call(lc, call);
        }

        qDebug() << "Finalizando Colgado ..." << endl;
    }else if (event != ""){
        qDebug() << "evento no conocido: " << event << endl;
    }


}

class Softphone : public QObject
{
    Q_OBJECT
public:
    explicit Softphone(QObject *parent = 0);
    ~Softphone();

public:
    QString evento;
    bool IncomingEnabled;
    bool exitSoftphone = FALSE;

signals:
    void signalIncomingCall();
    void signalConnectedCall();
    void signalEndCall();
    void signalHangup();
    void finished();
    void RegistrationFailed();
    void RegistrationOk();

public slots:
    void process(void);
    void finSoft();
};

#endif // SOFTPHONE_H
