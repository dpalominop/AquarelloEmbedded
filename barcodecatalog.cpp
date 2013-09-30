#include "barcodecatalog.h"

BarcodeCatalog::BarcodeCatalog(QWidget *parent) :
    QWebView(parent)
{   myThread = new QThread;
    PATH = QCoreApplication::applicationDirPath();
    mySound = new QSound(PATH+"/sounds/beep.wav");
    connect(this, SIGNAL(startSound()), mySound, SLOT(play()));
    mySound->moveToThread(myThread);
    myThread->start();

    connect(this, SIGNAL(startServerConsulting(QString)), this, SLOT(serverConsulting(QString)));
}
void BarcodeCatalog::setSettings(AquarelloSettings *settings)
{
    this->settings = settings;
}
void BarcodeCatalog::start()
{
    init();
}

void BarcodeCatalog::queryCatalog(QString barcode)
{
    QString codigo = "";
    codigo = barcode.remove(QRegExp("[AF\\n\\t\\r]"));
    QRegExp r("^\\d+$");
    if(!r.exactMatch(codigo)){
        qDebug() << codigo;
        qDebug() << "INVALID";
        //codeFlag=true;
        emit catalogTimeout();
        return;
    }
    remote=false;
    qDebug() << "Barcode Catalog : " << codigo << endl;
    emit startSound();
    //QMetaObject::invokeMethod(this,"startSound", Qt::DirectConnection);

    if(codigo=="769903002503")
    {
        //emit queryReceived();
        if(timer->isActive()){
            timer->stop();
        }
        qDebug() << "Barcode Catalog : Quit request" << endl;
        emit quitRequest();
        return;
    }if(codigo=="769903001018"){
        if(timer->isActive()){
            qDebug() << "reboot timer stop";
            timer->stop();
        }
        QString program = "sudo reboot";
        QProcess *myProcess = new QProcess(this);
        myProcess->execute(program);
        return;

    }else if(codigo=="769903002008"){
        emit queryReceived();

        ReadCodeStatus(VIEW_FILE);
        if(timer->isActive()){
            qDebug() << "timer stop";
            timer->stop();
        }
        timer->start();
        return;

    }else if(codigo=="3040521760554"){
        emit queryReceived();

        ReadCodeStatus(VIEW_FILE);
        if(timer->isActive()){
            qDebug() << "timer stop";
            timer->stop();
        }
        timer->start();
        return;

    }

    emit queryReceived();
    emit startServerConsulting(codigo);
    serialCom->serialClear();
}


void BarcodeCatalog::serverConsulting(QString codigo)
{
    if (QString::compare(settings->terminal,"Aquarelle",Qt::CaseInsensitive) == 0)
    {
        TcpClient *tcpclient = new TcpClient();
        qDebug() << "ServerPort:" << settings->serverport;
        tcpclient->Connect(settings->serverip,settings->serverport,5000);
        if(tcpclient->isConnected()){
            QString data = codigo+"\t"+QString(SERIALNMBR)+"\t"+QString(DEVICETYPEID)+"\t"+settings->localipaddr+"\t0\t\t\t\n";
            tcpclient->senData(data.toLatin1());//initial parameter to wet app configuration
            QString str=tcpclient->receiveData(5000);
            qDebug()<<str;
            Display *display =new Display();
            display->setTerminal(settings->terminal);
            //display->setForeImgPath(foregroundPath);
            if(str.count()>0){
                QString htmlpage = VIEW_FILE;
                display->strData(str);
                display->setBackImgPath(backgroundImagePath);
                display->createDisplayPage(htmlpage);
                this->load(QUrl::fromLocalFile(htmlpage));
                //this->load(QUrl("http://192.168.100.3/view.html"));
                //this->load(QUrl("http://www.google.com.pe"));
                if(timer->isActive()){
                    qDebug() << "timer stop";
                    timer->stop();
                }
                timer->start();

            }else{
                emit catalogTimeout();
            }
            delete display;
        }else{
            emit catalogTimeout();
        }

        delete tcpclient;
    }else if (QString::compare(settings->terminal,"Genio",Qt::CaseInsensitive) == 0){
        UdpClient *udpclient = new UdpClient();
        udpclient->Connect(settings->serverip,settings->serverport,5000);
        //qDebug() << QString("%1").arg(manager->getBaudrate())+" "+manager->getServerip()+" "+QString("%1").arg(manager->getServerport());
        QString data = "F"+codigo+"\t"+QString(SERIALNMBR)+"\t"+QString(DEVICETYPEID)+"\t0\t0\t0\t\t\n";
        udpclient->senData(data.toLatin1());
        QString str=udpclient->receiveData(5000);
        //udpclient->closeAll();
        qDebug()<<str;
        Display *display =new Display();
        display->setTerminal(settings->terminal);
        if(str.count()>0){
            QString htmlpage = VIEW_FILE;
            display->byteData(str.toLatin1());
            display->setBackImgPath(backgroundImagePath);
            display->createDisplayPage(htmlpage);
            this->load(QUrl::fromLocalFile(htmlpage));
            if(timer->isActive()){
                qDebug() << "timer stop";
                timer->stop();
            }
            timer->start();
        }else{
            emit catalogTimeout();
        }
        delete udpclient;
        delete display;
    }else if (QString::compare(settings->terminal,"WEB",Qt::CaseInsensitive) == 0){
        QString url = settings->baseurl+codigo;
        //QString url = manager->getBaseURL();
        qDebug() << url;
        this->load(QUrl(url));
        remote=true;
    }else if (QString::compare(settings->terminal,"TCP-CRS",Qt::CaseInsensitive) == 0){
        TcpClient *tcpclient = new TcpClient();
        tcpclient->Connect(settings->serverip,settings->serverport,5000);
        if(tcpclient->isConnected()){
            QString data = "soeps~Message-Profile-Id=DIFDA|Table-Name=EAMITEMR|Key-Lower="+codigo+"|~";
            int len = data.count();

            QByteArray ba = intToByteArray(len);
            tcpclient->senData(ba);
            //QString data = codigo+"\t123456789012\t123456789012\t"+localipaddr+"\t0\t\t\t";
            tcpclient->senData(data.toLatin1());//initial parameter to wet app configuration
            QString str=tcpclient->receiveDataIBM(5000);
                //qDebug()<<str;
            Display *display =new Display();
            display->setTerminal(settings->terminal);
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
                display->setBackImgPath(backgroundImagePath);
                display->createDisplayPage(htmlpage);
                this->load(QUrl::fromLocalFile(htmlpage));
                if(timer->isActive()){
                    qDebug() << "timer stop";
                    timer->stop();
                }
                timer->start();
            }else{
                emit catalogTimeout();
            }
        }else{
             emit catalogTimeout();
        }
        delete tcpclient;
    }
}

void BarcodeCatalog::init()
{
    PATH = QCoreApplication::applicationDirPath();

    timer = new QTimer(this);
    timer->setInterval(settings->barcodeCatalogDelay_ms);
    timer->setSingleShot(true);
    connect(timer, SIGNAL(timeout()), this,SIGNAL(catalogTimeout()));

    serialCom = new SerialCommunication(this,settings->portcom,settings->baudrate);
    connect(serialCom,SIGNAL(readyScan()),this,SLOT(startScanning()));
    connect(serialCom,SIGNAL(newStringArrived(QString)),this,SLOT(queryCatalog(QString)));
    serialCom->initSerial();

    this->page()->mainFrame()->setScrollBarPolicy(Qt::Horizontal, Qt::ScrollBarAlwaysOff);
    this->page()->mainFrame()->setScrollBarPolicy(Qt::Vertical, Qt::ScrollBarAlwaysOff);
    connect(this, SIGNAL(loadFinished(bool)), this, SLOT(finishLoading(bool)));
    connect(this, SIGNAL(loadFinished(bool)), SLOT(adjustLocation()));
    connect(this, SIGNAL(linkClicked(QUrl)), SLOT(linkClicked(QUrl)));
    this->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
    backgroundImagePath=getImageFilePath("scanner_pictures/"+settings->orientation,"default_pictures","background");
}

void BarcodeCatalog::startScanning()
{
    if(serialCom->isActive())
    {
        serialCom->scanContinuously(100);
        qDebug() << "Barcode Scanner : Scanning Continuosly";
    }
}


void BarcodeCatalog::stopScanning()
{
    if(serialCom->isActive()) serialCom->stopScanContinuously();
}

void BarcodeCatalog::finishLoading(bool){

    //qDebug() << "FINISH Loading...";
    if(remote){
        qDebug() << "REMOTE FINISH Loading...";

        if(timer->isActive()){
            timer->stop();
        }
        timer->start();
        remote=false;
        //codeFlag=true;
    }
}
void BarcodeCatalog::adjustLocation(){
    //qDebug() << "Adjust Location...";
}
void BarcodeCatalog::linkClicked(QUrl url){
    qDebug() << "Clicked...";
    //codeFlag=false;
    if(timer->isActive())
    {
        timer->stop();
    }
    remote=true;
    this->load(url);


}
QString BarcodeCatalog::getImageFilePath(QString dirname,QString dirdefault, QString filename)
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
QByteArray BarcodeCatalog::intToByteArray(int value)
{
    QByteArray ba;
    for (int i = 0; i < 4; i++) {
        int offset = (3 - i) * 8;
        ba[i] = ((value >> offset) & 0xFF);
    }
    return ba;

}
void BarcodeCatalog::ReadCodeStatus(QString htmlpage)
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
                        break;
                }
        }else if(QString::compare(interfaces[i].humanReadableName(),"wlan0") == 0){
                QList<QNetworkAddressEntry> wlan0_addresses = interfaces[i].addressEntries();
                for(int k=0; k < wlan0_addresses.count(); k++){
                        ipaddr_wlan0 = wlan0_addresses[k].ip().toString();
                        break;
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
        list.append("font-style:normal;");
        list.append(QString("'>ETH0 : \n%1\n</SPAN>\n").arg(ipaddr_eth0));
        posy+=yval;
    }
    if(ipaddr_wlan0.count() > 0){
        list.append(QString("<SPAN style='position:absolute;left:%1px;top:%2px;").arg(posx).arg(posy));
        list.append(QString("background-color:transparent;"));
        list.append(QString("color:blue;"));
        list.append(QString("font-size:30pt;"));
        list.append("font-family:arial;");
        list.append("font-style:normal;");
        list.append(QString("'>WLAN0 : \n%1\n</SPAN>\n").arg(ipaddr_wlan0));
        posy+=yval;
    }
    list.append(QString("<SPAN style='position:absolute;left:%1px;top:%2px;").arg(posx).arg(posy));
    list.append(QString("background-color:transparent;"));
    list.append(QString("color:blue;"));
    list.append(QString("font-size:30pt;"));
    list.append("font-family:arial;");
    list.append("font-style:normal;");
    list.append(QString("'>PortCOM : \n%1\n</SPAN>\n").arg(settings->portcom));
    posy+=yval;

    list.append(QString("<SPAN style='position:absolute;left:%1px;top:%2px;").arg(posx).arg(posy));
    list.append(QString("background-color:transparent;"));
    list.append(QString("color:blue;"));
    list.append(QString("font-size:30pt;"));
    list.append("font-family:arial;");
    list.append("font-style:normal;");
    list.append(QString("'>BaudRate : \n%1\n</SPAN>\n").arg(settings->baudrate));
    posy+=yval;

    list.append(QString("<SPAN style='position:absolute;left:%1px;top:%2px;").arg(posx).arg(posy));
    list.append(QString("background-color:transparent;"));
    list.append(QString("color:blue;"));
    list.append(QString("font-size:30pt;"));
    list.append("font-family:arial;");
    list.append("font-style:normal;");
    list.append(QString("'>Terminal : \n%1\n</SPAN>\n").arg(settings->terminal));
    posy+=yval;

    list.append(QString("<SPAN style='position:absolute;left:%1px;top:%2px;").arg(posx).arg(posy));
    list.append(QString("background-color:transparent;"));
    list.append(QString("color:blue;"));
    list.append(QString("font-size:30pt;"));
    list.append("font-family:arial;");
    list.append("font-style:normal;");
    list.append(QString("'>ServerIP : \n%1\n</SPAN>\n").arg(settings->serverip));
    posy+=yval;

    list.append(QString("<SPAN style='position:absolute;left:%1px;top:%2px;").arg(posx).arg(posy));
    list.append(QString("background-color:transparent;"));
    list.append(QString("color:blue;"));
    list.append(QString("font-size:30pt;"));
    list.append("font-family:arial;");
    list.append("font-style:normal;");
    list.append(QString("'>ServerPort : \n%1\n</SPAN>\n").arg(settings->serverport));
    posy+=yval;

    list.append(QString("<SPAN style='position:absolute;left:%1px;top:%2px;").arg(posx).arg(posy));
    list.append(QString("background-color:transparent;"));
    list.append(QString("color:blue;"));
    list.append(QString("font-size:30pt;"));
    list.append("font-family:arial;");
    list.append("font-style:normal;");
    list.append(QString("'>TimeCode : \n%1\n</SPAN>\n").arg(settings->barcodeCatalogDelay_ms));
    posy+=yval;

    list.append(QString("<SPAN style='position:absolute;left:%1px;top:%2px;").arg(posx).arg(posy));
    list.append(QString("background-color:transparent;"));
    list.append(QString("color:blue;"));
    list.append(QString("font-size:30pt;"));
    list.append("font-family:arial;");
    list.append("font-style:normal;");
    list.append(QString("'>TimeSlide : \n%1\n</SPAN>\n").arg(settings->pictureCatalogDelay_ms));

    list.append("</div>\n");
    list.append("</BODY>\n");
    list.append("</HTML>\n");
    QString result = "";
    QString s;
    foreach( s, list )
        result +=s;
    out << result;
    file.close();

    this->load(QUrl::fromLocalFile(htmlpage));
}
