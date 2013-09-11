#include "ftpclient.h"

FtpClient::FtpClient(QObject *parent) :
    QObject(parent)
{
    updatestatus=false;
}
void FtpClient::connectToftp(QString ipaddr, int port, int timeout){
    QString PATH =QCoreApplication::applicationDirPath();

    ftp = new QFtp(this);
    eventLoop = new QEventLoop();
    listfiles.clear();
    status=false;
    connect(ftp,SIGNAL(commandFinished(int,bool)),this,SLOT(ftpCommandFinished(int,bool)));
    connect(ftp,SIGNAL(listInfo(QUrlInfo)),this,SLOT(addToList(QUrlInfo)));
    ftp->connectToHost(ipaddr,port);
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(testNetwork()));
    timer->setSingleShot(1);
    timer->start(timeout);
    eventLoop->exec();
    timer->stop();
    newfile="";
    if(status){
        //QDateTime qdt1 = QDateTime::fromString("2013-05-09","yyyy-MM-dd");
        QDateTime qdt1 = convertDate(FileNameRef);
        int ftpcount = listfiles.size();
        //qDebug() << "COUNT = "+QString("%1").arg(ftpcount);
        if(ftpcount > 0){
            for(int i=0; i < ftpcount;++i){
               //qDebug() << listfiles[i];
               QDateTime qdt = convertDate(listfiles[i]);
               if(qdt > qdt1){
                   //qDebug() << "OLEEEEE";
                   newfile=listfiles[i];
                   qdt1 = qdt;
               }
           }
        }
    }
    if(newfile.size() > 0){
        //qDebug() << QString("FILE = %1").arg(newfile);
        //download file
        file = new QFile(QString("%1/%2").arg(TMP_DIR,newfile));
        if(file->exists()){
            qDebug() << "FILE EXISTS";
        }
        if(!file->open(QIODevice::WriteOnly)){
            //if(!file->open(QIODevice::ReadWrite)){
            delete file;
        }else{
            status=false;
            QDir dir(TMP_FTP);
            if(!dir.exists()){
                dir.mkpath(".");
            }
            ftp->get(newfile,file);
            eventLoop->exec();

            if(status){
                file->close();
                //decompress zip file
                QString command;
                command = "unzip -x "+QString(TMP_DIR)+"/"+newfile+" -d "+QString(TMP_FTP)+" >/dev/null";
                system(qPrintable(command));
                /*
                QProcess *unzip = new QProcess(this);
                qDebug() << "INIT ZIP";
                unzip->start("unzip",QStringList() << "-x" << QString("%1/%2").arg(TMP_DIR,newfile) << "-d" << TMP_FTP);
                unzip->waitForFinished(4000);
                QByteArray output = unzip->readAll();
                if(output.size() > 0){
                    qDebug() << QString(output);
                }
                unzip->terminate();
                */
                //qDebug() << "UNZIP CLOSE";
                dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
                dir.setSorting(QDir::Size | QDir::Reversed);
                QFileInfoList list = dir.entryInfoList();

                //qDebug() << "LIST SIZE = "+QString("%1").arg(list.size());
                QStringList sliderlist;
                int slidecount=0;
                QStringList backlist;
                int backcount=0;
                QStringList forelist;
                int forecount=0;
                QStringList linemsglist;
                int linemsgcount=0;

                curfilename = "";

                for (int i = 0; i < list.size(); i++) {
                    QFileInfo fileInfo = list.at(i);
                    if(!fileInfo.suffix().compare("png",Qt::CaseInsensitive) | !fileInfo.suffix().compare("jpeg",Qt::CaseInsensitive) | !fileInfo.suffix().compare("jpg",Qt::CaseInsensitive)
                    | !fileInfo.suffix().compare("bmp",Qt::CaseInsensitive) | !fileInfo.suffix().compare("gif",Qt::CaseInsensitive)) {
                        if(fileInfo.baseName().compare("background",Qt::CaseInsensitive) == 0){
                            backcount++;
                            //backlist << fileInfo.absoluteFilePath();
                            backlist << fileInfo.fileName();
                        }else if(fileInfo.baseName().compare("foreground",Qt::CaseInsensitive) == 0){
                            forecount++;
                            //forelist << fileInfo.absoluteFilePath();
                            forelist << fileInfo.fileName();
                        }else{
                            slidecount++;
                            //sliderlist << fileInfo.absoluteFilePath();
                            sliderlist << fileInfo.fileName();
                        }
                    }else if(!fileInfo.suffix().compare("xml",Qt::CaseInsensitive)){
                        if(fileInfo.baseName().compare("linemsg",Qt::CaseInsensitive) == 0){
                            linemsgcount++;
                            linemsglist << fileInfo.fileName();
                        }
                    }
                }
                //qDebug() << "ENTER DIR";
                QDir dimg(PATH+"/"+IMAGES_DIRECTORY);
                QFileInfoList listimg;

                if(dimg.exists()){
                    //qDebug() << "DIR EXISTS";
                    dimg.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
                    dimg.setSorting(QDir::Size | QDir::Reversed);
                    listimg = dimg.entryInfoList();

                    int listsize = listimg.size();
                    //qDebug() << QString("%1").arg(listsize);
                    if(slidecount > 0 || backcount > 0 || forecount > 0 || linemsgcount > 0){
                        //system(qPrintable(AQUARELLO_RW_CMD));
                    }
                    if(slidecount > 0){
                        updatestatus=true;
                        //borrar archivos slideshow
                        //qDebug() << "SLIDESHOW";
                        //qDebug() << sliderlist;

                        if(listsize > 0){
                            for (int i = 0; i < listsize; i++) {
                                QFileInfo fileInfo = listimg.at(i);
                                if((!fileInfo.suffix().compare("png",Qt::CaseInsensitive) | !fileInfo.suffix().compare("jpeg",Qt::CaseInsensitive) | !fileInfo.suffix().compare("jpg",Qt::CaseInsensitive)
                                | !fileInfo.suffix().compare("bmp",Qt::CaseInsensitive) | !fileInfo.suffix().compare("gif",Qt::CaseInsensitive))
                                && (fileInfo.baseName().compare("background",Qt::CaseInsensitive) != 0 && fileInfo.baseName().compare("foreground",Qt::CaseInsensitive) != 0)){
                                    //delete files
                                    QString path = fileInfo.absoluteFilePath();
                                    QFile file( path );
                                    file.remove();
                                }
                            }
                            for(int p=0; p < sliderlist.length(); p++){
                                QFile::copy(QString(TMP_FTP)+"/"+sliderlist[p],PATH+"/"+QString(IMAGES_DIRECTORY)+"/"+sliderlist[p]);
                            }
                        }else{
                            for(int p=0; p < sliderlist.length(); p++){
                                QFile::copy(QString(TMP_FTP)+"/"+sliderlist[p],PATH+"/"+QString(IMAGES_DIRECTORY)+"/"+sliderlist[p]);
                            }
                        }
                    }
                    if(backcount > 0){
                        updatestatus=true;
                        //qDebug() << "BACKGROUND";
                        //qDebug() << backlist;
                        if(listsize > 0){
                            for (int i = 0; i < listimg.size(); i++) {
                                QFileInfo fileInfo = listimg.at(i);
                                if((!fileInfo.suffix().compare("png",Qt::CaseInsensitive) | !fileInfo.suffix().compare("jpeg",Qt::CaseInsensitive) | !fileInfo.suffix().compare("jpg",Qt::CaseInsensitive)
                                | !fileInfo.suffix().compare("bmp",Qt::CaseInsensitive) | !fileInfo.suffix().compare("gif",Qt::CaseInsensitive))
                                && (fileInfo.baseName().compare("background",Qt::CaseInsensitive) == 0)){
                                    //delete files
                                    QString path = fileInfo.absoluteFilePath();
                                    QFile file( path );
                                    file.remove();
                                }
                            }
                            //qDebug() << QString(TMP_FTP)+"/"+backlist[0]+" - "+PATH+"/"+QString(IMAGES_DIRECTORY)+"/"+backlist[0].toLower();
                            QFile::copy(QString(TMP_FTP)+"/"+backlist[0],PATH+"/"+QString(IMAGES_DIRECTORY)+"/"+backlist[0].toLower());
                        }else{
                            //qDebug() << QString(TMP_FTP)+"/"+backlist[0]+" - "+PATH+"/"+QString(IMAGES_DIRECTORY)+"/"+backlist[0].toLower();
                            QFile::copy(QString(TMP_FTP)+"/"+backlist[0],PATH+"/"+QString(IMAGES_DIRECTORY)+"/"+backlist[0].toLower());
                        }
                    }
                    if(forecount > 0){
                        updatestatus=true;
                        //qDebug() << "FOREGROUND";
                        //qDebug() << forelist;
                        if(listsize > 0){
                            for (int i = 0; i < listimg.size(); i++) {
                                QFileInfo fileInfo = listimg.at(i);
                                if((!fileInfo.suffix().compare("png",Qt::CaseInsensitive) | !fileInfo.suffix().compare("jpeg",Qt::CaseInsensitive) | !fileInfo.suffix().compare("jpg",Qt::CaseInsensitive)
                                | !fileInfo.suffix().compare("bmp",Qt::CaseInsensitive) | !fileInfo.suffix().compare("gif",Qt::CaseInsensitive))
                                && (fileInfo.baseName().compare("foreground",Qt::CaseInsensitive) == 0)){
                                   //delete files
                                   QString path = fileInfo.absoluteFilePath();
                                   QFile file( path );
                                   file.remove();
                                 }
                            }
                            //qDebug() << QString(TMP_FTP)+"/"+forelist[0]+" - "+PATH+"/"+QString(IMAGES_DIRECTORY)+"/"+forelist[0].toLower();
                            QFile::copy(QString(TMP_FTP)+"/"+forelist[0],PATH+"/"+QString(IMAGES_DIRECTORY)+"/"+forelist[0].toLower());

                        }else{
                            //qDebug() << QString(TMP_FTP)+"/"+forelist[0]+" - "+PATH+"/"+QString(IMAGES_DIRECTORY)+"/"+forelist[0].toLower();
                            QFile::copy(QString(TMP_FTP)+"/"+forelist[0],PATH+"/"+QString(IMAGES_DIRECTORY)+"/"+forelist[0].toLower());
                        }
                    }
                    if(linemsgcount > 0){
                        updatestatus=true;
                        //qDebug() << "LINEMESSAGES";
                        QFile lmsg(PATH+"/"+linemsglist[0].toLower());
                        lmsg.remove();
                        QFile::copy(QString(TMP_FTP)+"/"+linemsglist[0],PATH+"/"+linemsglist[0].toLower());
                    }
                    if(slidecount > 0 || backcount > 0 || forecount > 0 || linemsgcount > 0){
                        curfilename = newfile;
                        //system(qPrintable(AQUARELLO_RD_CMD));
                    }
                }
                QFile::remove(QString("%1/%2").arg(TMP_DIR,newfile));
                removeDirectory(QDir(TMP_FTP));
            }
            //qDebug() << "END GET";
        }
    }
    ftp->close();

}
void FtpClient::setCredentials(QString username, QString password){
    UserName = username;
    Password = password;
}
void FtpClient::setFileRef(QString filename){
    FileNameRef = filename;
}
void FtpClient::setPrefix(QString prefix){
    Prefix = prefix;
}
bool FtpClient::getUpdateStatus()
{
    return updatestatus;
}
QString FtpClient::getCurrentFileName(){
    return curfilename;
}

void FtpClient::ftpCommandFinished(int, bool error)
{
    if(ftp->currentCommand() == QFtp::ConnectToHost){
        if(error){
            qDebug() << "ERROR CONNECT";
            eventLoop->exit();
            //ftp->abort();
            //ftp->deleteLater();
            return;
        }else{
            qDebug() << "LOGIN";
            ftp->login(UserName,Password);
        }

    }
    if(ftp->currentCommand() == QFtp::Login){
        if(error){
            qDebug() << "BAD AUTHETICATION";
            eventLoop->exit();
            return;
        }else{
            ftp->list();
        }
    }
    if(ftp->currentCommand() == QFtp::List){
        if(error){
            qDebug() << "Error List";
            eventLoop->exit();
        }else{
            qDebug() << listfiles;
            status=true;
            eventLoop->exit();
        }

    }
    if(ftp->currentCommand() == QFtp::Get){
        if(error){
            qDebug() << "Error Get File";
            eventLoop->exit();
        }else{
            status=true;
            eventLoop->exit();
        }
    }
}
void FtpClient::addToList(const QUrlInfo &urlInfo)
{
    QRegExp r("^(\\w+)(-)(\\d+)(\\.zip)$");
    int pos = r.indexIn(urlInfo.name());
    if(pos > -1){
        //qDebug() << urlInfo.name();
        QString nombre = r.cap(1);
        QString fecha = r.cap(3);
        if(fecha.size() == 14){
            if(Prefix.size() >0){
                if(!nombre.compare(Prefix,Qt::CaseInsensitive)){
                    listfiles << urlInfo.name();
                }

            }else{
                listfiles << urlInfo.name();
            }
        }
    }

}
void FtpClient::testNetwork()
{
    qDebug() << "TIMER END";
    eventLoop->exit();
}
bool FtpClient::removeDirectory( QDir dir )
{
    bool ok = dir.exists();
    if ( ok )
    {
        QFileInfoList entries = dir.entryInfoList( QDir::NoDotAndDotDot |
                QDir::Dirs | QDir::Files );
        foreach ( QFileInfo entryInfo, entries )
        {
            QString path = entryInfo.absoluteFilePath();
            if ( entryInfo.isDir() )
            {
                if ( ! removeDirectory( QDir( path ) ) )
                {
                    ok = false;
                    break;
                }
            }
            else
            {
                QFile file( path );
                if ( ! file.remove() )
                {
                    ok = false;
                    break;
                }
            }
        }
    }

    if ( ok && ! dir.rmdir( dir.absolutePath() ) )
        ok = false;

    return ok;
}
QDateTime FtpClient::convertDate(QString filename)
{
    QRegExp r("^(\\w+)(-)(\\d+)(\\.zip)$");
    QDateTime qdt;
    int pos = r.indexIn(filename);
    //qDebug() << "POS = "+QString("%1").arg(pos);
    if(pos > -1){
        //QString nombre = r.cap(1);
        QString fecha =  r.cap(3);

        QString year = fecha.mid(0,4);
        QString month = fecha.mid(4,2);
        QString day = fecha.mid(6,2);
        QString hour = fecha.mid(8,2);
        QString minute = fecha.mid(10,2);
        QString second = fecha.mid(12,2);

        int imonth = month.toInt();
        int iday = day.toInt();
        int ihour = hour.toInt();
        int iminute = minute.toInt();
        int isecond = second.toInt();

        if(imonth > 0 && imonth <= 12 && iday > 0 && iday <= 31 && ihour >= 0 && ihour <= 23 && iminute >=0 && iminute <= 59 && isecond >=0 && isecond <= 59){
            qdt = QDateTime::fromString(QString("%1-%2-%3 %4:%5:%6").arg(year,month,day,hour,minute,second),"yyyy-MM-dd hh:mm:ss");
        }else{
            qdt = QDateTime::fromString("2013-01-01 08:00:00","yyyy-MM-dd hh:mm:ss");
        }

    }else{

        qdt = QDateTime::fromString("2013-01-01 08:00:00","yyyy-MM-dd hh:mm:ss");
    }
    return qdt;
}
//void FtpClient::mySubProcessDone(int, QProcess::ExitStatus)
void FtpClient::mySubProcessDone()
{
    qDebug() << "EXIT";
    //eventLoop->exit();
}
void FtpClient::onProcessStarted()
{
    qDebug() << "onProcessStarted";
}
