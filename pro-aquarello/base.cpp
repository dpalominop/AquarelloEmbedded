#include "base.h"
#include "barcode.h"

Base::Base(QWidget *parent) :
    QMainWindow(parent)
{
    //load config.xml, if blank load default parameters
    manager=new Configuration();
    cfgftp=new ConfigFtp();
    manager->checkAppVersion();

    //load welcome.xml, if blank load default parameters
    welcome=new WelcomeConfiguration();

    foregroundPath=getForeground();
    backgroundPath=getBackground();

    localipaddr = getIpAddr();
    terminal = manager->getTerminal();
    QString str = "";
    if (QString::compare(terminal,"Aquarelle",Qt::CaseInsensitive) == 0)
    {
	tcpclient=new TcpClient();
	tcpclient->Connect(manager->getServerip(),manager->getServerport(),3000);
	if(tcpclient->isConnected()){
                QString data = "001\t"+QString(SERIALNMBR)+"\t"+QString(DEVICETYPEID)+"\t"+localipaddr+"\t0\t\t\t\n";
        	tcpclient->senData(data.toLatin1());//initial parameter to wet app configuration
                str=tcpclient->receiveData(3000);
    	}
    	//qDebug()<<str;
        display =new Display();
	display->setTerminal(terminal);
        display->strData(str);
        display->setForeImgPath(foregroundPath);
        welcomepage = QString(WELCOME_HTML);
        display->createWelcomePage(welcomepage);
    }else if (QString::compare(terminal,"Genio",Qt::CaseInsensitive) == 0){
        udpclient=new UdpClient();
        display =new Display();
	display->setTerminal(terminal);
        display->setForeImgPath(foregroundPath);
        welcomepage = QString(WELCOME_HTML);
        display->createWelcomePage(welcomepage);
    }else if (QString::compare(terminal,"WEB",Qt::CaseInsensitive) == 0){
        display =new Display();
	display->setTerminal(terminal);
        display->setForeImgPath(foregroundPath);
        welcomepage = QString(WELCOME_HTML);
        display->createWelcomePage(welcomepage);

    }else if (QString::compare(terminal,"TCP-CRS",Qt::CaseInsensitive) == 0){
        tcpclient=new TcpClient();
        display =new Display();
	display->setTerminal(terminal);
        display->setForeImgPath(foregroundPath);
        welcomepage = QString(WELCOME_HTML);
        display->createWelcomePage(welcomepage);
    }else{
	//default Genio
	udpclient=new UdpClient();
        display =new Display();
	display->setTerminal(terminal);
        display->setForeImgPath(foregroundPath);
        welcomepage = QString(WELCOME_HTML);
        display->createWelcomePage(welcomepage);
    }

    getSlideShow();
    qDebug()<<slidelist;

    //making thread to read de barcode
    QTimer *timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(barcodeSlot()));
    timer->start(100);

    codeFlag=true;
    remote=false;
    web=new QWebView();
    web->page()->mainFrame()->setScrollBarPolicy(Qt::Horizontal, Qt::ScrollBarAlwaysOff);
    web->page()->mainFrame()->setScrollBarPolicy(Qt::Vertical, Qt::ScrollBarAlwaysOff);
    connect(web, SIGNAL(loadFinished(bool)), this, SLOT(finishLoading(bool)));
    connect(web, SIGNAL(loadFinished(bool)), SLOT(adjustLocation()));
    connect(web, SIGNAL(linkClicked(QUrl)), SLOT(linkClicked(QUrl)));
    web->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
    //web->setMaximumSize(720, 480);

    welcomePage();
    //qDebug()<<"time code"<<manager->getTimeCode();

    //making threads to slide show
    slidetimer=new QTimer(this);
    slidetimer->setInterval(manager->getTimeSlide()*1000);
    connect(slidetimer,SIGNAL(timeout()),this,SLOT(normal_status()));

    //making thread to code
    codetimer =new QTimer(this);
    codetimer->setInterval(manager->getTimeCode()*1000);
    connect(codetimer,SIGNAL(timeout()),this,SLOT(welcome_status()));

    if(lengthSlideShow > 0){
	slidetimer->start();
    }
    //set image number to show
    imageNumberToShow=0;
    setCentralWidget(web);

    QThread *thread = new QThread( );
    tcpserver = new TcpServer();
    tcpserver->moveToThread(thread);

    connect( thread, SIGNAL(started()), tcpserver, SLOT(doWork()) );
    thread->start();
    //setLayout(layout);
    setWindowTitle("TCC app");
//    setGeometry(0,0,720,480);
    //setWindowFlags(Qt::ToolTip);
}
void Base::doPut(QString filename)
{
    qDebug() << "DO PUT ="+filename;
    QDateTime qdt = convertDate(cfgftp->getFtpFileRef());
    QDateTime qdt1 = convertDate(filename);
    if(qdt1 > qdt){
        if(codetimer->isActive()){
            codetimer->stop();
        }
        if(slidetimer->isActive()){
            slidetimer->stop();
        }
        if(processZipFile(filename)){
            if(filename.size() > 0){
                cfgftp->setFtpFileRef(filename);
                cfgftp->updateConfigFile();
                slidelist.clear();
                getSlideShow();
            }
        }
        codetimer->start();
        codeFlag=true;
    }
}

void Base::getSlideShow()
{
    QString PATH =QCoreApplication::applicationDirPath();
    QDir dir(PATH+"/"+SLIDE_DIRECTORY);
    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    dir.setSorting(QDir::Size | QDir::Reversed);

    QFileInfoList list = dir.entryInfoList();
    //SlideList
    for (int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);
        //if(fileInfo.suffix()=="png"|fileInfo.suffix()=="jpeg"|fileInfo.suffix()=="bmp"|fileInfo.suffix()=="jpg"|fileInfo.suffix()=="gif")
        if((!fileInfo.suffix().compare("png",Qt::CaseInsensitive) | !fileInfo.suffix().compare("jpeg",Qt::CaseInsensitive) | !fileInfo.suffix().compare("jpg",Qt::CaseInsensitive)
        | !fileInfo.suffix().compare("bmp",Qt::CaseInsensitive) | !fileInfo.suffix().compare("gif",Qt::CaseInsensitive)) && (fileInfo.baseName().compare("background",Qt::CaseInsensitive) != 0 && fileInfo.baseName().compare("foreground",Qt::CaseInsensitive) != 0)){
          //qDebug() << fileInfo.baseName();
          slidelist << QString("%1").arg(PATH+"/"+SLIDE_DIRECTORY+"/"+fileInfo.fileName());
        }
     }
    lengthSlideShow=slidelist.length();
}

QString Base::getForeground()
{
    QString PATH =QCoreApplication::applicationDirPath()+"/"+BACKFOREIMG_DIRECTORY;
    QString fname = "";
    QDir dir(PATH);
         dir.setFilter(QDir::Files | QDir::NoSymLinks);
    QFileInfoList list = dir.entryInfoList();

    QRegExp r("^foreground.[png|jpeg|jpg|bmp|gif]+$",Qt::CaseInsensitive);
    //qDebug() << "SIZE="+QString("%1").arg(list.size());
    for (int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);
        //qDebug() << fileInfo.fileName();
        if(r.exactMatch(fileInfo.fileName())){
            fname=PATH+"/"+fileInfo.fileName();
            break;
        }
    }
    return fname;
}
QString Base::getBackground()
{
    QString PATH =QCoreApplication::applicationDirPath()+"/"+BACKFOREIMG_DIRECTORY;
    QString bname = "";
    QDir dir(PATH);
         dir.setFilter(QDir::Files | QDir::NoSymLinks);
    QFileInfoList list = dir.entryInfoList();

    QRegExp r("^background.[png|jpeg|jpg|bmp|gif]+$",Qt::CaseInsensitive);
    //qDebug() << "SIZE="+QString("%1").arg(list.size());
    for (int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);
        //qDebug() << fileInfo.fileName();
        if(r.exactMatch(fileInfo.fileName())){
            bname=PATH+"/"+fileInfo.fileName();
            break;
        }
    }
    return bname;
}

void Base::quit()
{
    qApp->quit();
}

void Base::barcodeSlot()
{
    QString qstemp;
    QString qschar;
    int i;
        //unsigned char c[256], d,g;
        char buf_read[256]={0};
        fd = open_ttyPort(manager->getPortCom().toLatin1().data(),manager->getBaudrate());
        int res;
        if(fd <0)
        {
                printf("open failed\r\\n");
                return ;
        }

        res = read(fd,buf_read,256);
        if(res >0)
        {
             qstemp ="";
      	     for(i=0;i<res;i++)
             {
                  qschar.sprintf("%c",buf_read[i]);
                  qstemp = qstemp + qschar;
             }


             //this->editbarcode->setText(qstemp);
             this->show_product_detail(qstemp);
        }

}
void Base::welcomePage()
{
    QFile fileHTML(welcomepage);
    if (!fileHTML.open(QIODevice::ReadOnly)) {
        web->load(QUrl("qrc:/html/page.html"));
        printf("loaded qrc:/html/page.html\r\\n");
    }
    else{
        web->load(QUrl::fromLocalFile(welcomepage));
        printf("loaded welcome.html\r\n");
        fileHTML.close();
    }

}

void Base::normal_status()
{
    if(codeFlag){
        //int width=640;
        //int height=480;

        if(imageNumberToShow >= lengthSlideShow){
            imageNumberToShow=0;
        }
        if(!QFile::exists(QString("%1").arg(slidelist[imageNumberToShow]))){
            imageNumberToShow+=1;
            return;
        }
        //QUrl baseUrl = QUrl::fromLocalFile(QDir::current().absoluteFilePath("config.xml"));
        //QString msg = QString("<html><body><img STYLE='position:absolute; LEFT: 0px; TOP: 0px' src='%1' width='%2'  height='%3'/></body></html>").arg(slidelist[imageNumberToShow]).arg(width).arg(height);
//	QString msg = QString("<html><body><img STYLE='position:absolute; LEFT: 28px; TOP: 0px;' src='%1' width='%2'  height='%3'/></body></html>").arg(slidelist[imageNumberToShow]).arg(width).arg(height);
	QString msg = QString("<html><body><img STYLE='position:absolute;LEFT: 0px; TOP: 0px;' src='%1' width='100\%'  height='100\%'/></body></html>").arg(slidelist[imageNumberToShow]);
	//web->setHtml(msg,baseUrl);
	QString PATH =QCoreApplication::applicationDirPath();
	QString fileName=QString("%1/%2").arg(PATH).arg("config.xml");

        web->setUrl(QUrl::fromLocalFile(fileName));
	web->setHtml(msg,QUrl::fromLocalFile(fileName));
        imageNumberToShow+=1;
    }
}

void Base::welcome_status()
{
	welcomePage();
	if(codetimer->isActive()){
		codetimer->stop();
	}
	if(lengthSlideShow > 0){
		slidetimer->start();
	}
}
void Base::show_product_detail(QString rcodigo)
{
//    QSound::play("/opt/aquarello/sounds/beep.wav");
    QString codigo = "";
    int rlen;

    codeFlag=false;
    if(rcodigo.indexOf("|") >= 0){
        if((rlen=rcodigo.indexOf("<END>")) > 0){
            codigo = qrcode+rcodigo.mid(0,rlen);
            qrcode = "";
        }else{
            qrcode += rcodigo.remove(QRegExp("[\\n\\r]"));
            return;
        }
        qrcodeSettings(codigo);
        QString program = "/sbin/reboot";
        QProcess *myProcess = new QProcess(this);
        myProcess->execute(program);
        //codeFlag=true;
        return;
    }

    qrcode="";
    codigo = rcodigo.remove(QRegExp("[AF\\n\\t\\r]"));
    QRegExp r("^\\d+$");
    if(!r.exactMatch(codigo)){
        qDebug() << codigo;
        qDebug() << "INVALID";
        codeFlag=true;
        return;
    }
    qDebug() << codigo;
    if(codetimer->isActive())
    {
        codetimer->stop();
    }
    if(slidetimer->isActive()){
        slidetimer->stop();
    }
    if(codigo=="769903001018"){
        QString program = "/sbin/reboot";
        QProcess *myProcess = new QProcess(this);
        myProcess->execute(program);
        codeFlag=true;
        return;

    }else if(codigo=="769903002008"){
        ReadCodeStatus(VIEW_FILE);
        codetimer->start();
        codeFlag=true;
        return;

    }else if(codigo=="769903002145"){
        qDebug() << "FTP Client";
        processFtp();
        codetimer->start();
        codeFlag=true;
        return;

    }else if(codigo=="769903002503"){
        QTimer::singleShot(100, this, SLOT(quit()));
    }

    QString str = "";
    if (QString::compare(manager->getTerminal(),"Aquarelle",Qt::CaseInsensitive) == 0)
    {
        tcpclient->Connect(manager->getServerip(),manager->getServerport(),5000);
        if(tcpclient->isConnected()){
            QString data = codigo+"\t"+QString(SERIALNMBR)+"\t"+QString(DEVICETYPEID)+"\t"+localipaddr+"\t0\t\t\t\n";
            tcpclient->senData(data.toLatin1());//initial parameter to wet app configuration
            str=tcpclient->receiveData(5000);
                qDebug()<<str;
            if(str.count()>0){
                QString htmlpage = VIEW_FILE;
                display->strData(str);
                display->setBackImgPath(backgroundPath);
                display->createDisplayPage(htmlpage);
                web->load(QUrl::fromLocalFile(htmlpage));
                codetimer->start();
            }else{
                if(lengthSlideShow > 0){
                    slidetimer->start();
                }else{
                    codetimer->start();
                }
            }
        }else{
            if(lengthSlideShow > 0){
                    slidetimer->start();
            }else{
                    codetimer->start();
            }
        }
    }else if (QString::compare(manager->getTerminal(),"Genio",Qt::CaseInsensitive) == 0){
        udpclient->Connect(manager->getServerip(),manager->getServerport(),5000);
        //qDebug() << QString("%1").arg(manager->getBaudrate())+" "+manager->getServerip()+" "+QString("%1").arg(manager->getServerport());
        QString data = "F"+codigo+"\t"+QString(SERIALNMBR)+"\t"+QString(DEVICETYPEID)+"\t0\t0\t0\t\t\n";
        udpclient->senData(data.toLatin1());
        str=udpclient->receiveData(5000);
        //udpclient->closeAll();
        qDebug()<<str;
        if(str.count()>0){
            QString htmlpage = VIEW_FILE;
            display->byteData(str.toLatin1());
            display->setBackImgPath(backgroundPath);
            display->createDisplayPage(htmlpage);
            web->load(QUrl::fromLocalFile(htmlpage));
            codetimer->start();
        }else{
            if(lengthSlideShow > 0){
                slidetimer->start();
            }else{
                codetimer->start();
            }
        }
    }else if (QString::compare(manager->getTerminal(),"WEB",Qt::CaseInsensitive) == 0){
        QString url = manager->getBaseURL()+codigo;
        //QString url = manager->getBaseURL();
        qDebug() << url;
        web->load(QUrl(url));
        remote=true;
    }else if (QString::compare(manager->getTerminal(),"TCP-CRS",Qt::CaseInsensitive) == 0){
        tcpclient->Connect(manager->getServerip(),manager->getServerport(),5000);
        if(tcpclient->isConnected()){
            QString data = "soeps~Message-Profile-Id=DIFDA|Table-Name=EAMITEMR|Key-Lower="+codigo+"|~";
            int len = data.count();

            QByteArray ba = intToByteArray(len);
            tcpclient->senData(ba);
            //QString data = codigo+"\t123456789012\t123456789012\t"+localipaddr+"\t0\t\t\t";
            tcpclient->senData(data.toLatin1());//initial parameter to wet app configuration
            str=tcpclient->receiveDataIBM(5000);
                //qDebug()<<str;

            if(str.count()>0){
                QString htmlpage = VIEW_FILE;
                //dis->strData(str);
                QRegExp rxlen("^(.*)[~](.*)$");
                QString procstr="";

                int pos = rxlen.indexIn(str);
                if (pos > -1) {
                    //qDebug() << rxlen.cap(1);
                    procstr = rxlen.cap(2);
                    //qDebug() << procstr;
		    display->clearFields();
                    if(procstr.count() > 10){
                        display->parseXML("NormalSaleItem",procstr);
                    }else{
                        display->setDataAccessFault(rxlen.cap(1));
                    }

                }else{
                      display->setDataAccessFault("Not Found");
                }
                //qDebug() << dis->params[0]["ItemCode"];
                //qDebug() << dis->params[0]["ItemName"];
                display->setBackImgPath(backgroundPath);
                display->createDisplayPage(htmlpage);
                web->load(QUrl::fromLocalFile(htmlpage));
                codetimer->start();
            }else{
                if(lengthSlideShow > 0){
                    slidetimer->start();
                }else{
                    codetimer->start();
                }
            }
        }else{
            if(lengthSlideShow > 0){
                    slidetimer->start();
            }else{
                    codetimer->start();
            }
        }
    }else{
	//default Genio
	udpclient->Connect(manager->getServerip(),manager->getServerport(),5000);
        //qDebug() << QString("%1").arg(manager->getBaudrate())+" "+manager->getServerip()+" "+QString("%1").arg(manager->getServerport());
        QString data = "F"+codigo+"\t"+QString(SERIALNMBR)+"\t"+QString(DEVICETYPEID)+"\t0\t0\t0\t\t\n";
        udpclient->senData(data.toLatin1());
        str=udpclient->receiveData(5000);

        //udpclient->closeAll();
        qDebug()<<str;
        if(str.count()>0){
            QString htmlpage = VIEW_FILE;
            display->byteData(str.toLatin1());
            display->setBackImgPath(backgroundPath);
            display->createDisplayPage(htmlpage);
            web->load(QUrl::fromLocalFile(htmlpage));
            codetimer->start();
        }else{
            if(lengthSlideShow > 0){
                slidetimer->start();
            }else{
                codetimer->start();
            }
        }
    }
    codeFlag=true;
}

void Base::processFtp()
{
    if(QString::compare(cfgftp->getFtpServer(),"0.0.0.0") != 0){
        //ftpclient->setCredentials("gempsy","GempsyTCC2012");
        ftpclient = new FtpClient();
        ftpclient->setCredentials(cfgftp->getFtpUser(),cfgftp->getFtpPasswd());
        ftpclient->setFileRef(cfgftp->getFtpFileRef());
        ftpclient->setPrefix(cfgftp->getFtpPrefName());
        ftpclient->connectToftp(cfgftp->getFtpServer(),cfgftp->getFtpPort(),2000);
        if(ftpclient->getUpdateStatus()){
            QString curfilename = ftpclient->getCurrentFileName();
            if(curfilename.size() > 0){
                cfgftp->setFtpFileRef(curfilename);
                cfgftp->updateConfigFile();
            }
            foregroundPath=getForeground();
            backgroundPath=getBackground();
            display->setForeImgPath(foregroundPath);
            display->createWelcomePage(welcomepage);

            slidelist.clear();
            getSlideShow();
        }
    }
}

bool Base::processZipFile(QString newfile){
    QString PATH =QCoreApplication::applicationDirPath();
    bool updatestatus=false;
    QDir dir(TMP_FTP);
    if(!dir.exists()){
        dir.mkpath(".");
    }
    QString command;
    command = "unzip -x "+PATH+"/"+QString(FTP_DIR)+"/"+newfile+" -d "+QString(TMP_FTP)+" >/dev/null";
    system(qPrintable(command));

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

    //curfilename = "";

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
            //curfilename = newfile;
            //system(qPrintable(AQUARELLO_RD_CMD));
        }
    }
    removeDirectory(QDir(TMP_FTP));

    return updatestatus;
}
bool Base::removeDirectory( QDir dir )
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
QDateTime Base::convertDate(QString filename)
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
void Base::adjustLocation(){
    //qDebug() << "Adjust Location...";
}
void Base::finishLoading(bool){

    //qDebug() << "FINISH Loading...";
    if(remote){
	qDebug() << "REMOTE FINISH Loading...";
        codetimer->start();
        remote=false;
        codeFlag=true;
    }
}
void Base::linkClicked(QUrl url){
    qDebug() << "Clicked...";
    codeFlag=false;
    if(codetimer->isActive())
    {
        codetimer->stop();
    }
    if(slidetimer->isActive()){
        slidetimer->stop();
    }
    remote=true;
    web->load(url);


}
/*QString Base::getIpAddr()
{
    QString prog = "/bin/sh";//shell
    QStringList arguments;
    arguments << "-c" << "ifconfig eth0 | grep 'inet ' | awk '{print $2}' | sed 's/addr://'";
    QProcess* process = new QProcess(this);
    process->start(prog , arguments);
    process->waitForFinished();
    QString tmp = process->readAll();
    return tmp;
}*/
void Base::ReadCodeStatus(QString htmlpage)
{
    QString ipaddr_eth0 = "";
    QString ipaddr_wlan0 = "";

    QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();
    for(int i=0; i<interfaces.count(); i++){
	qDebug() << "NET:"+interfaces[i].humanReadableName();
	if(QString::compare(interfaces[i].humanReadableName(),"eth0") == 0){
		QList<QNetworkAddressEntry> eth0_addresses = interfaces[i].addressEntries();
		for(int j=0; j < eth0_addresses.count(); j++){
			ipaddr_eth0 = eth0_addresses[j].ip().toString();
		}
	}else if(QString::compare(interfaces[i].humanReadableName(),"wlan0") == 0){
		QList<QNetworkAddressEntry> wlan0_addresses = interfaces[i].addressEntries();
                for(int k=0; k < wlan0_addresses.count(); k++){
                        ipaddr_wlan0 = wlan0_addresses[k].ip().toString();
                }
	}
    }

    QFile file(htmlpage);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    QList<QString> list;

    list << "<HTML>\n";
    list.append("<HEAD>\n");
    list.append("<style type=\"text/css\">\n");
    list.append("body {height:100%; margin:0; padding:0;}\n");
    list.append("#page-background {position:fixed; top:0; left:0; width:100%; height:100%;}\n");
    list.append("#content {position:relative; z-index:1;padding:0px;}\n");
    list.append("</style>\n");
    list.append("</HEAD>\n");
    list.append("<BODY>\n");
    list.append("<div id='page-background'>\n");
    list.append("</div>\n");
    list.append("<div id=\"content\">\n");
    int posx=10;
    int posy=10;
    int yval=40;
    if(ipaddr_eth0.count() > 0){
        list.append(QString("<SPAN style='position:absolute;left:%1px;top:%2px;").arg(posx).arg(posy));
        list.append(QString("background-color:transparent;"));
        list.append(QString("color:blue;"));
        list.append(QString("font-size:30pt;"));
        list.append("font-family:arial;");
        list.append("font-style:italic;");
        list.append(QString("'>ETH0 : \n%1\n</SPAN>\n").arg(ipaddr_eth0));
        posy+=yval;
    }
    if(ipaddr_wlan0.count() > 0){
        list.append(QString("<SPAN style='position:absolute;left:%1px;top:%2px;").arg(posx).arg(posy));
        list.append(QString("background-color:transparent;"));
        list.append(QString("color:blue;"));
        list.append(QString("font-size:30pt;"));
        list.append("font-family:arial;");
        list.append("font-style:italic;");
        list.append(QString("'>WLAN0 : \n%1\n</SPAN>\n").arg(ipaddr_wlan0));
        posy+=yval;
    }
    list.append(QString("<SPAN style='position:absolute;left:%1px;top:%2px;").arg(posx).arg(posy));
    list.append(QString("background-color:transparent;"));
    list.append(QString("color:blue;"));
    list.append(QString("font-size:30pt;"));
    list.append("font-family:arial;");
    list.append("font-style:italic;");
    list.append(QString("'>PortCOM : \n%1\n</SPAN>\n").arg(manager->getPortCom()));
    posy+=yval;

    list.append(QString("<SPAN style='position:absolute;left:%1px;top:%2px;").arg(posx).arg(posy));
    list.append(QString("background-color:transparent;"));
    list.append(QString("color:blue;"));
    list.append(QString("font-size:30pt;"));
    list.append("font-family:arial;");
    list.append("font-style:italic;");
    list.append(QString("'>BaudRate : \n%1\n</SPAN>\n").arg(manager->getBaudrate()));
    posy+=yval;

    list.append(QString("<SPAN style='position:absolute;left:%1px;top:%2px;").arg(posx).arg(posy));
    list.append(QString("background-color:transparent;"));
    list.append(QString("color:blue;"));
    list.append(QString("font-size:30pt;"));
    list.append("font-family:arial;");
    list.append("font-style:italic;");
    list.append(QString("'>Terminal : \n%1\n</SPAN>\n").arg(manager->getTerminal()));
    posy+=yval;

    list.append(QString("<SPAN style='position:absolute;left:%1px;top:%2px;").arg(posx).arg(posy));
    list.append(QString("background-color:transparent;"));
    list.append(QString("color:blue;"));
    list.append(QString("font-size:30pt;"));
    list.append("font-family:arial;");
    list.append("font-style:italic;");
    list.append(QString("'>ServerIP : \n%1\n</SPAN>\n").arg(manager->getServerip()));
    posy+=yval;

    list.append(QString("<SPAN style='position:absolute;left:%1px;top:%2px;").arg(posx).arg(posy));
    list.append(QString("background-color:transparent;"));
    list.append(QString("color:blue;"));
    list.append(QString("font-size:30pt;"));
    list.append("font-family:arial;");
    list.append("font-style:italic;");
    list.append(QString("'>ServerPort : \n%1\n</SPAN>\n").arg(manager->getServerport()));
    posy+=yval;

    list.append(QString("<SPAN style='position:absolute;left:%1px;top:%2px;").arg(posx).arg(posy));
    list.append(QString("background-color:transparent;"));
    list.append(QString("color:blue;"));
    list.append(QString("font-size:30pt;"));
    list.append("font-family:arial;");
    list.append("font-style:italic;");
    list.append(QString("'>TimeCode : \n%1\n</SPAN>\n").arg(manager->getTimeCode()));
    posy+=yval;

    list.append(QString("<SPAN style='position:absolute;left:%1px;top:%2px;").arg(posx).arg(posy));
    list.append(QString("background-color:transparent;"));
    list.append(QString("color:blue;"));
    list.append(QString("font-size:30pt;"));
    list.append("font-family:arial;");
    list.append("font-style:italic;");
    list.append(QString("'>TimeSlide : \n%1\n</SPAN>\n").arg(manager->getTimeSlide()));


    list.append("</div>\n");
    list.append("</BODY>\n");
    list.append("</HTML>\n");
    QString result = "";
    QString s;
    foreach( s, list )
            result +=s;
    out << result;
    file.close();

    web->load(QUrl::fromLocalFile(htmlpage));
}

QString Base::getIpAddr()
{
    return "192.168.0.1";
}
QByteArray Base::intToByteArray(int value)
{
    QByteArray ba;
    for (int i = 0; i < 4; i++) {
        int offset = (3 - i) * 8;
        ba[i] = ((value >> offset) & 0xFF);
    }
    return ba;

}

void Base::qrcodeSettings(QString inputString)
{
    int posx=100;
    int posy=200;
    //int yval=40;
    QMap <QString,QString> map;
    QStringList list;
    QString qrdata = "";

    if(inputString.startsWith("QR", Qt::CaseInsensitive)){
	qrdata = inputString.mid(2);
    }else{
	qrdata = inputString;
    }
    //list = inputString.split("|");
    list = qrdata.split("|");

    //qDebug() << "qrcode";
    qDebug() << qrdata;

    foreach(QString cadena , list)
    {
        QStringList data = cadena.split("=");
        if(data.size() == 2){
            map[data[0]]=data[1];
        }
        //qDebug() << QString("[%1]").arg(value);
    }

    QList<QString> html;

    html << headerHtml() << bodyHtml(posx,posy,"Magic Code Detected") << bodyHtml(posx+50,posy+100,"Rebooting !!!") << footerHtml();
    writeHtmlView(html);

    // /etc/network/interfaces

    html.clear();

    bool intstat = true;

    if(map.contains("eth0") && map.contains("wlan0")){

        QFile filei(TMP_INTERFACES_FILE);
        filei.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream outi(&filei);
        QList<QString> interfaces;


        interfaces.append("auto lo\n");
        interfaces.append("iface lo inet loopback\n");
        interfaces.append("auto eth0 wlan0\n");

        if( map.value("eth0").compare("0") == 0){
            interfaces.append("iface eth0 inet dhcp\n");
        }else{
            interfaces.append("iface eth0 inet static\n");
            if(map.contains("eth0ip")){
                interfaces.append("address "+map.value("eth0ip")+"\n");
            }else{
                intstat = false;
            }
            if(map.contains("eth0mask")){
                interfaces.append("netmask "+map.value("eth0mask")+"\n");

            }else{
                intstat = false;
            }
            if(map.contains("eth0gw")){
                interfaces.append("gateway "+map.value("eth0gw")+"\n");

            }
        }

        if( map.value("wlan0").compare("0") == 0){
            interfaces.append("iface wlan0 inet dhcp\n");
	    interfaces.append("wpa_driver wext\n");
	    interfaces.append("wpa-conf /etc/wpa_supplicant.conf\n");
        }else{
            interfaces.append("iface wlan0 inet static\n");
	    interfaces.append("wpa_driver wext\n");
            interfaces.append("wpa-conf /etc/wpa_supplicant.conf\n");
            if(map.contains("wlan0ip")){
                interfaces.append("address "+map.value("wlan0ip")+"\n");
            }else{
                intstat = false;
            }
            if(map.contains("wlan0mask")){
                interfaces.append("netmask "+map.value("wlan0mask")+"\n");

            }else{
                intstat = false;
            }
            if(map.contains("wlan0gw")){
                interfaces.append("gateway "+map.value("wlan0gw")+"\n");

            }
        }

        QString result = "";
        QString s;
        foreach( s, interfaces )
                result +=s;
        outi << result;
        filei.close();


    }else{
        intstat = false;
    }



    // /etc/wpa_supplicant.conf

    bool wpastat = true;

    if(map.contains("wpatype")){
        QFile filew(TMP_WPA_FILE);
        filew.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream outw(&filew);
        QList<QString> wpa;

        wpa.append("ctrl_interface=/var/run/wpa_supplicant\n");
        wpa.append("network={\n");
        wpa.append("scan_ssid=1\n");

        int wtype = map.value("wpatype").toInt();
        switch(wtype){
            case 0:
                if(map.contains("ssid")){
                    wpa.append("  ssid=\""+map.value("ssid")+"\"\n");
                }else{
                    wpastat = false;
                }
                wpa.append("  key_mgmt=NONE\n");
                break;
            case 1:
                if(map.contains("ssid")){
                    wpa.append("  ssid=\""+map.value("ssid")+"\"\n");
                }else{
                    wpastat = false;
                }
                wpa.append("  key_mgmt=WPA-PSK\n");
                wpa.append("  proto=WPA\n");
                if(map.contains("wpapair")){
                    int wpair = map.value("wpapair").toInt();
                    if(wpair == 1){
                        wpa.append("  pairwise=TKIP\n");
                    }else if(wpair == 2){
                        wpa.append("  pairwise=AES\n");
                    }else if(wpair == 3){
                        wpa.append("  pairwise=AES+TKIP\n");
                    }
                }else{
                    wpastat = false;
                }
                if(map.contains("psk")){
                    wpa.append("  psk=\""+map.value("psk")+"\"\n");
                }else{
                    wpastat = false;
                }
                break;
            case 2:
                if(map.contains("ssid")){
                    wpa.append("  ssid=\""+map.value("ssid")+"\"\n");
                }else{
                    wpastat = false;
                }
                wpa.append("  key_mgmt=WPA-PSK\n");
                wpa.append("  proto=RSN\n");
                if(map.contains("wpapair")){
                    int wpair = map.value("wpapair").toInt();
                    if(wpair == 1){
                        wpa.append("  pairwise=TKIP\n");
                    }else if(wpair == 2){
                        wpa.append("  pairwise=AES\n");
                    }else if(wpair == 3){
                        wpa.append("  pairwise=AES+TKIP\n");
                    }
                }else{
                    wpastat = false;
                }
                if(map.contains("psk")){
                    wpa.append("  psk=\""+map.value("psk")+"\"\n");
                }else{
                    wpastat = false;
                }
                break;
            case 5:
                if(map.contains("ssid")){
                    wpa.append("  ssid=\""+map.value("ssid")+"\"\n");
                }else{
                    wpastat = false;
                }
                wpa.append("  key_mgmt=NONE\n");
                if(map.contains("txkey")){
                    wpa.append("  wep_tx_keyidx="+map.value("txkey")+"\n");
                    if(map.contains("psk")){
                        wpa.append("  wep_key"+map.value("txkey")+"=\""+map.value("psk")+"\"\n");
                    }else{
                        wpastat = false;
                    }
                }else{
                    wpastat = false;
                }
                break;
            default:
                break;
        }

        wpa.append("}\n");
        QString result = "";
        QString s = "";
        foreach( s, wpa )
                result +=s;
        outw << result;
        filew.close();

    }else{
        wpastat = false;
    }

    if(intstat || wpastat){
        //system(qPrintable(CONFIG_RW_CMD));
        if(intstat){
            system(qPrintable("mv "+QString("%1").arg(TMP_INTERFACES_FILE)+" "+QString("%1").arg(INTERFACES_FILE)));
        }
        if(wpastat){
            system(qPrintable("mv "+QString("%1").arg(TMP_WPA_FILE)+" "+QString("%1").arg(WPA_FILE)));
        }
        //system(qPrintable(CONFIG_RD_CMD));
    }

    QFile fileq(TMP_CONFIG_FILE);
    fileq.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream outq(&fileq);
    QList<QString> aqua;
    bool aqstat = false;

    aqua.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n");
    aqua.append("<Aquarello>\n");
    if(map.contains("com")){
        aqua.append("<PortCOM>"+map.value("com")+"</PortCOM>\n");
        aqstat = true;
    }else{
        aqua.append("<PortCOM>"+manager->getPortCom()+"</PortCOM>\n");
    }
    if(map.contains("baud")){
        aqua.append("<BaudRate>"+map.value("baud")+"</BaudRate>\n");
        aqstat = true;
    }else{
        aqua.append("<BaudRate>"+QString("%1").arg(manager->getBaudrate())+"</BaudRate>\n");
    }
    if(map.contains("term")){
        aqua.append("<Terminal>"+map.value("term")+"</Terminal>\n");
        aqstat = true;
    }else{
        aqua.append("<Terminal>"+manager->getTerminal()+"</Terminal>\n");
    }
    if(map.contains("srvip")){
        aqua.append("<ServerIP>"+map.value("srvip")+"</ServerIP>\n");
        aqstat = true;
    }else{
        aqua.append("<ServerIP>"+manager->getServerip()+"</ServerIP>\n");
    }
    if(map.contains("srvport")){
        aqua.append("<ServerPort>"+map.value("srvport")+"</ServerPort>\n");
        aqstat = true;
    }else{
        aqua.append("<ServerPort>"+QString("%1").arg(manager->getServerport())+"</ServerPort>\n");
    }
    if(map.contains("tcode")){
        aqua.append("<TimeCode>"+map.value("tcode")+"</TimeCode>\n");
        aqstat = true;
    }else{
        aqua.append("<TimeCode>"+QString("%1").arg(manager->getTimeCode())+"</TimeCode>\n");
    }
    if(map.contains("tslide")){
        aqua.append("<TimeSlide>"+map.value("tslide")+"</TimeSlide>\n");
        aqstat = true;
    }else{
        aqua.append("<TimeSlide>"+QString("%1").arg(manager->getTimeSlide())+"</TimeSlide>\n");
    }
    if(map.contains("burl")){
        aqua.append("<BaseURL>"+map.value("burl")+"</BaseURL>\n");
        aqstat = true;
    }else{
        aqua.append("<BaseURL>"+QString("%1").arg(manager->getBaseURL())+"</BaseURL>\n");
    }
    if(map.contains("pwd")){
        aqua.append("<Password>"+map.value("pwd")+"</Password>\n");
        aqstat = true;
    }else{
        aqua.append("<Password>"+manager->getPassword()+"</Password>\n");
    }
    aqua.append("<AppVersion>"+manager->getAppVersion()+"</AppVersion>\n");
    aqua.append("</Aquarello>\n");

    QString result = "";
    QString s = "";
    foreach( s, aqua )
            result +=s;
    outq << result;
    fileq.close();

    if(aqstat){
        QString PATH =QCoreApplication::applicationDirPath();
        //system(qPrintable(AQUARELLO_RW_CMD));
        system(qPrintable("mv "+QString("%1").arg(TMP_CONFIG_FILE)+" "+PATH+"/"+CONFIG_FILE));
        //system(qPrintable(AQUARELLO_RD_CMD));
    }else{
        QFile::remove(TMP_CONFIG_FILE);
    }

    QFile filef(TMP_CONFIG_FTPFILE);
    filef.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream outf(&filef);
    QList<QString> ftp;
    bool ftpstat = false;
    ftp.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n");
    ftp.append("<ConfigFtp>\n");
    if(map.contains("fsrvip")){
        ftp.append("<FtpServer>"+map.value("fsrvip")+"</FtpServer>\n");
        ftpstat = true;
    }else{
        ftp.append("<FtpServer>"+cfgftp->getFtpServer()+"</FtpServer>\n");
    }
    if(map.contains("fport")){
        ftp.append("<FtpPort>"+map.value("fport")+"</FtpPort>\n");
        ftpstat = true;
    }else{
        ftp.append("<FtpPort>"+QString("%1").arg(cfgftp->getFtpPort())+"</FtpPort>\n");
    }
    if(map.contains("fuser")){
        ftp.append("<FtpUser>"+map.value("fuser")+"</FtpUser>\n");
        ftpstat = true;
    }else{
        ftp.append("<FtpUser>"+cfgftp->getFtpUser()+"</FtpUser>\n");
    }
    if(map.contains("fpwd")){
        ftp.append("<FtpPasswd>"+map.value("fpwd")+"</FtpPasswd>\n");
        ftpstat = true;
    }else{
        ftp.append("<FtpPasswd>"+cfgftp->getFtpServer()+"</FtpPasswd>\n");
    }
    if(map.contains("fpref")){
        ftp.append("<FtpPrefName>"+map.value("fpref")+"</FtpPrefName>\n");
        ftpstat = true;
    }else{
        ftp.append("<FtpPrefName>"+cfgftp->getFtpPrefName()+"</FtpPrefName>\n");
    }
    //if(map.contains("fref")){
    //    ftp.append("<FtpFileRef>"+map.value("fref")+"</FtpFileRef>\n");
    //    ftpstat = true;
    //}else{
    //    ftp.append("<FtpFileRef>"+cfgftp->getFtpFileRef()+"</FtpFileRef>\n");
    //}
    ftp.append("<FtpFileRef>"+cfgftp->getFtpFileRef()+"</FtpFileRef>\n");
    ftp.append("</ConfigFtp>\n");
    result = "";
    s = "";
    foreach( s, ftp )
            result +=s;
    outf << result;
    filef.close();

    if(ftpstat){
        QString PATH =QCoreApplication::applicationDirPath();
        //system(qPrintable(AQUARELLO_RW_CMD));
        system(qPrintable("mv "+QString("%1").arg(TMP_CONFIG_FTPFILE)+" "+PATH+"/"+CONFIG_FTPFILE));
        //system(qPrintable(AQUARELLO_RD_CMD));
    }else{
        QFile::remove(TMP_CONFIG_FTPFILE);
    }
}
QList<QString> Base::headerHtml()
{
    QList<QString> header;

    header << "<HTML>\n";
    header.append("<HEAD>\n");
    header.append("<style type=\"text/css\">\n");
    header.append("body {height:100%; margin:0; padding:0;}\n");
    header.append("#page-background {position:fixed; top:0; left:0; width:100%; height:100%;}\n");
    header.append("#content {position:relative; z-index:1;padding:0px;}\n");
    header.append("</style>\n");
    header.append("</HEAD>\n");
    header.append("<BODY>\n");
    header.append("<div id='page-background'>\n");
    header.append("</div>\n");
    header.append("<div id=\"content\">\n");

    return header;
}

QList<QString> Base::bodyHtml(int posx, int posy, QString text)
{
    QList<QString> body;

    body.append(QString("<SPAN style='position:absolute;left:%1px;top:%2px;").arg(posx).arg(posy));
    body.append(QString("background-color:transparent;"));
    body.append(QString("color:blue;"));
    body.append(QString("font-size:30pt;"));
    body.append("font-family:arial;");
    body.append("font-style:italic;");
    body.append("'>"+text+"</SPAN>\n");

    return body;
}

QList<QString> Base::footerHtml()
{
    QList<QString> footer;

    footer.append("</div>\n");
    footer.append("</BODY>\n");
    footer.append("</HTML>\n");

    return footer;
}

void Base::writeHtmlView(QList<QString> html)
{
    QFile fileh(VIEW_FILE);
    fileh.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream outh(&fileh);

    QString resulth = "";
    QString sh;
    foreach( sh,html )
            resulth +=sh;
    outh << resulth;
    fileh.close();
    web->load(QUrl::fromLocalFile(VIEW_FILE));

}

int open_ttyPort(char * portname, int baudrate)
{
    if(fdopenf==1)
       return fd;
    fd = open(portname, O_RDWR | O_NOCTTY | O_NDELAY | O_NONBLOCK );
    if (fd == -1) {
        printf("open_port: Unable to open %s ",portname);
        fdopenf=0;
        return -1;
    }else{
        fcntl(fd, F_SETFL, FNDELAY);
        fdopenf=1;
        init_Port(fd,baudrate);
        return (fd);
     }
}
void pushButton1_clicked();
void pushButton4_clicked();
void pushButton5_clicked();


int init_Port(int fd, int baudrate)
{
    //printf("in init_Port\\n");
    struct termios T_new;
    long BAUD;

//    printf("baudrate=%d\n",baudrate);
    switch (baudrate)
    {
	 case 115200:
	    BAUD = B115200;
	    break;
         case 38400:
            BAUD = B38400;
            break;
         case 19200:
            BAUD  = B19200;
            break;
         case 9600:
            BAUD  = B9600;
            break;
         default:
            BAUD  = B115200;
            break;
    }
    if (tcgetattr(fd,&T_old) != 0) {
            printf("tcgetattr failed\n");
            close(fd);
            return -1;
    }
    //printf("tcgetattr ok\\n");
    T_new = T_old;
        //T_new.c_cflag = ( BAUD | CS8 | CREAD | HUPCL | CLOCAL);
	T_new.c_cflag = BAUD | CRTSCTS | CS8 | CLOCAL | CREAD;
        T_new.c_oflag = 0;
        //T_new.c_iflag = IGNPAR ;
	T_new.c_iflag = IGNPAR | ICRNL;
        //T_new.c_lflag = 0; //NOFLSH
	T_new.c_lflag = ICANON;
        T_new.c_cc[VTIME] = 0;   /* no time delay */
        T_new.c_cc[VMIN]  = 1;   /* no char delay */

	tcflush(fd, TCIFLUSH);

        if (tcsetattr(fd,TCSANOW,&T_new) != 0) {
                printf("tcsetattr failed\n");
                tcsetattr(fd,TCSANOW,&T_old);
                close(fd);
                return -1;
        }
//        printf("tcsetattr ok\\n");
/*********************************************************/
        //trigger command
/*        char cmd0[]={0x16,'M',0x0d};
        char cmd1[]={"TRGMOD0."};
        char cmd2[]={0x16,'M',0x0d};
        char cmd3[]={"dechdr0."};
        char cmd4[]={0x16,'M',0x0d};
        char cmd5[]={"TRGMOD3."};

        write(fd,cmd0,sizeof(cmd0));
        tcflush(fd, TCIFLUSH);
        write(fd,cmd1,sizeof(cmd1));
        tcflush(fd, TCIFLUSH);
        write(fd,cmd2,sizeof(cmd2));
        tcflush(fd, TCIFLUSH);
        write(fd,cmd3,sizeof(cmd3));
        tcflush(fd, TCIFLUSH);

        printf("configurating barcode\r\\n");

        write(fd,cmd4,sizeof(cmd4));
        tcflush(fd, TCIFLUSH);
        write(fd,cmd5,sizeof(cmd5));
        tcflush(fd, TCIFLUSH);
*/
        //      tcflush(fd,TCIOFLUSH);

    /*
        Button1
    char cmd0[]={0x16,'M',0x0d};
    char cmd1[]={"TRGMOD0."};
    char cmd2[]={0x16,'M',0x0d};
    char cmd3[]={"dechdr0."};

    write(fd,cmd0,sizeof(cmd0));
    write(fd,cmd1,sizeof(cmd1));
    write(fd,cmd0,sizeof(cmd2));
    write(fd,cmd1,sizeof(cmd3));

        Button4
    char cmd0[]={0x16,'M',0x0d};
    char cmd1[]={"TRGMOD3."};

    char cmd2[]={0x16,'M',0x0d};
    char cmd3[]={"dechdr0."};

    write(fd,cmd0,sizeof(cmd0));
    write(fd,cmd1,sizeof(cmd1));
    write(fd,cmd0,sizeof(cmd2));
    write(fd,cmd1,sizeof(cmd3));

        Button 5
    char cmd0[]={0x16,'T',0x0d};
    char cmd1[]={"TRGMOD3."};
    write(fd,cmd0,sizeof(cmd0));
    write(fd,cmd1,sizeof(cmd1));

    */

        return 0;
}
void close_ttyPort(int fd)
{
        close(fd);
}

