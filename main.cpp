#include "aquarellovideo.h"

void signalhandler(int sig){
    if(sig==SIGINT){
        qApp->quit();
    }
}
int main(int argc, char *argv[])
{
    //printf("Beggining AquarelloVideo\n");
    QApplication app(argc, argv);
    printf("QApp Created\n");
    app.setApplicationName("AquarelloVideo");
//    app.setOverrideCursor(QCursor(Qt::BlankCursor));

    AquarelloVideo win;
    win.setWindowState(Qt::WindowFullScreen);
    win.show();
    signal(SIGINT, signalhandler);
    return app.exec();
}




