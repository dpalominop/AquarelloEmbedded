#include "widgetviewer.h"
#include <stdio.h>

void LogHandler(QtMsgType type, const QMessageLogContext& context, const QString& message)
{
    QByteArray lMessage = message.toLocal8Bit();

    QString text;

    switch(type)
    {
        case QtDebugMsg:
            text = QString("Debug (%1:%2, %3): %4").arg(context.file).arg(context.line).arg(context.function).arg(lMessage.constData());
            break;

        case QtWarningMsg:
            text = QString("Warning (%1:%2, %3): %4").arg(context.file).arg(context.line).arg(context.function).arg(lMessage.constData());
            break;

        case QtCriticalMsg:
            text = QString("Critical (%1:%2, %3): %4").arg(context.file).arg(context.line).arg(context.function).arg(lMessage.constData());

        case QtFatalMsg:
            text = QString("Fatal (%1:%2, %3): %4").arg(context.file).arg(context.line).arg(context.function).arg(lMessage.constData());
            abort();
    }

    QFile file("log.txt");
    file.open(QIODevice::WriteOnly | QIODevice::Append);

    QTextStream tStream(&file);
    tStream << text << endl;

    file.close();
}

void signalhandler(int sig){
    if(sig==SIGINT){
        qApp->quit();
    }
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    qInstallMessageHandler(LogHandler);
    stdout = freopen("log.txt", "a+", stdout);

    qDebug() << "QApp Created" << endl;
    app.setApplicationName("AquarelloVideo");
    app.setOverrideCursor(QCursor(Qt::BlankCursor));

    WidgetViewer widgetview;
    widgetview.setWindowState(Qt::WindowFullScreen);
    widgetview.show();

    signal(SIGINT, signalhandler);
    return app.exec();
}




