#include "configuration.h"
#include "base.h"

Configuration::Configuration(QObject *parent) :
    QObject(parent)
{

        //defualt
       PATH =QCoreApplication::applicationDirPath();
       root = "Aquarello";
       terminal = "Aquarelle";
       serverip = "192.168.100.3";
       serverport = 1234;
       timecode = 10;
       timeslide = 15;
       portcom = "/dev/ttyACM0";
       baudrate = 9600;
       //Qint headcode = 1;
       localipaddr = "192.168.0.1";
       serialnumber = "XXXXXXXXXXXX";
       devicetypeid = "YYYYYYYYYYYY";
       password = "tccperu";
       baseurl = "http://localhost/cgi-bin/demo.cgi?gencode=";
       appversion = APP_VERSION;

       //parse XMl
       this->parseXML();
       
       if(!params.isEmpty()){
       
      	if (params[0]["PortCOM"]!="")
            this->portcom=params[0]["PortCOM"];
       	if (params[0]["BaudRate"]!="")
            this->baudrate=params[0]["BaudRate"].toInt();
       	if (params[0]["Terminal"]!="")
            this->terminal=params[0]["Terminal"];
       	if (params[0]["ServerIP"]!="")
            this->serverip=params[0]["ServerIP"];
       	if (params[0]["ServerPort"]!="")
            this->serverport=params[0]["ServerPort"].toInt();
       	if (params[0]["TimeCode"]!="")
            this->timecode=params[0]["TimeCode"].toInt();
       	if (params[0]["TimeSlide"]!="")
            this->timeslide=params[0]["TimeSlide"].toInt();
       	if (params[0]["Password"]!="")
            this->password=params[0]["Password"];
	if (params[0]["BaseURL"]!="")
            this->baseurl=params[0]["BaseURL"];
       	if (params[0]["AppVersion"]!="")
            this->appversion=params[0]["AppVersion"];
      }	
}


QString Configuration::getPortCom(){
    return this->portcom;
}

int Configuration::getBaudrate(){
    return this->baudrate;
}

QString Configuration::getTerminal(){
    return this->terminal;
}

QString Configuration::getServerip(){
    return this->serverip;
}

int Configuration::getServerport(){
    return this->serverport;
}

int Configuration::getTimeCode(){
    return this->timecode;
}

int Configuration::getTimeSlide(){
    return this->timeslide;
}

QString Configuration::getPassword(){
    return this->password;
}

QString Configuration::getAppVersion(){
    return this->appversion;
}

QString Configuration::getBaseURL(){
    return this->baseurl;
}
void Configuration::checkAppVersion(){
    if(QString::compare(this->appversion, APP_VERSION) == 0 ){
        return ;
    }else{
        this->appversion = APP_VERSION;
        //system(qPrintable(AQUARELLO_RW_CMD));
        writeConfigFile(PATH+"/"+CONFIG_FILE);
        //system(qPrintable(AQUARELLO_RD_CMD));
    }

}

/*falta crear el archivo config.xml si no existiera*/


void Configuration::parseXML() {
    /* We'll parse the example.xml */
    QFile* file = new QFile(PATH+"/"+CONFIG_FILE);

    /* If we can't open it, let's show an error message. */
    if (!file->open(QIODevice::ReadOnly | QIODevice::Text)) {
        //printf("No se pudo abrir config.xml\r\\n");
	qDebug() << "No se pudo abrir config.xml";
        writeConfigDefault(PATH+"/"+CONFIG_FILE);
        if(!file->open(QIODevice::ReadOnly | QIODevice::Text)){
            return;
        }

    }
    //printf("open config.xml\r\\n");
    qDebug() << "open config.xml";
    /* QXmlStreamReader takes any QIODevice. */
    QXmlStreamReader xml(file);

    /* We'll parse the XML until we reach end of it.*/
    while(!xml.atEnd() && !xml.hasError()) {
        /* Read next element.*/
        QXmlStreamReader::TokenType token = xml.readNext();
        /* If token is just StartDocument, we'll go to next.*/
        if(token == QXmlStreamReader::StartDocument) {
            continue;
        }
        /* If token is StartElement, we'll see if we can read it.*/
        if(token == QXmlStreamReader::StartElement) {
            /* If it's named person, we'll dig the information from there.*/
            if(xml.name() == "Aquarello") {
                params.append(this->parseAquarello(xml));
            }
        }
    }
    /* Error handling. */
    if(xml.hasError()) {
        //printf("Imposible parse config.xml\r\\n");
	qDebug() << "Imposible parse config.xml";
	file->close();
	writeConfigDefault(PATH+"/"+CONFIG_FILE);
        return;
    }
    xml.clear();
    file->close();
}

QMap<QString, QString> Configuration::parseAquarello(QXmlStreamReader& xml) {
    QMap<QString, QString> Aquarello;
    /* Let's check that we're really getting a person. */
    if(xml.tokenType() != QXmlStreamReader::StartElement &&
       xml.name() == "Aquarello") {
        return Aquarello;
    }
    /* Next element... */
    xml.readNext();
    /*
     * We're going to loop over the things because the order might change.
     * We'll continue the loop until we hit an EndElement named person.
     */
    while(!(xml.tokenType() == QXmlStreamReader::EndElement &&
            xml.name() == "Aquarello")) {
        if(xml.tokenType() == QXmlStreamReader::StartElement) {
            /* We've found first name.

            */
            if(xml.name() == "PortCOM") {
                this->addElementDataToMap(xml, Aquarello);
            }
            if(xml.name() == "BaudRate") {
                this->addElementDataToMap(xml, Aquarello);
            }
            if(xml.name() == "Terminal") {
                this->addElementDataToMap(xml, Aquarello);
            }
            /* We've found website. */
            if(xml.name() == "ServerIP") {
                this->addElementDataToMap(xml, Aquarello);
            }
            if(xml.name() == "ServerPort") {
                this->addElementDataToMap(xml, Aquarello);
            }
            if(xml.name() == "TimeCode") {
                this->addElementDataToMap(xml, Aquarello);
            }
            if(xml.name() == "TimeSlide") {
                this->addElementDataToMap(xml, Aquarello);
            }
	    if(xml.name() == "BaseURL") {
                //qDebug() << xml.readElementText();
                this->addElementDataToMap(xml, Aquarello);
            }
	    if(xml.name() == "Password") {
                this->addElementDataToMap(xml, Aquarello);
            }
            if(xml.name() == "AppVersion") {
                this->addElementDataToMap(xml, Aquarello);
            }

        }
        /* ...and next... */
        xml.readNext();
    }
    return Aquarello;
}

void Configuration::addElementDataToMap(QXmlStreamReader& xml,
                                      QMap<QString, QString>& map) const {
    /* We need a start element, like <foo> */
    if(xml.tokenType() != QXmlStreamReader::StartElement) {
        return;
    }
    /* Let's read the name... */
    QString elementName = xml.name().toString();
    /* ...go to the next. */
    xml.readNext();
    /*
     * This elements needs to contain Characters so we know it's
     * actually data, if it's not we'll leave.
     */
    if(xml.tokenType() != QXmlStreamReader::Characters) {
        return;
    }
    /* Now we can add it to the map.*/
    map.insert(elementName, xml.text().toString());
}
void Configuration::writeConfigDefault(QString filename)
{
    QFile file(filename);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    QList<QString> list;
    list.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n");
    list.append("<Aquarello>\n");
    list.append("<PortCOM>/dev/ttyACM0</PortCOM>\n");
    list.append("<BaudRate>9600</BaudRate>\n");
    list.append("<Terminal>Aquarelle</Terminal>\n");
    list.append("<ServerIP>192.168.100.6</ServerIP>\n");
    list.append("<ServerPort>1234</ServerPort>\n");
    list.append("<TimeCode>10</TimeCode>\n");
    list.append("<TimeSlide>15</TimeSlide>\n");
    list.append("<BaseURL>http://localhost/cgi-bin/demo.cgi?gencode=</BaseURL>\n");
    list.append("<Password>tccperu</Password>\n");
    list.append("<AppVersion>"+QString(APP_VERSION)+"</AppVersion>\n");
    list.append("</Aquarello>\n");
    QString result = "";
    QString s;
    foreach( s, list )
            result +=s;
    out << result;
    file.close();

}
void Configuration::writeConfigFile(QString filename)
{
    QFile file(filename);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    QList<QString> list;
    list.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n");
    list.append("<Aquarello>\n");
    list.append("<PortCOM>"+this->portcom+"</PortCOM>\n");
    list.append("<BaudRate>"+QString("%1").arg(this->baudrate)+"</BaudRate>\n");
    list.append("<Terminal>"+this->terminal+"</Terminal>\n");
    list.append("<ServerIP>"+this->serverip+"</ServerIP>\n");
    list.append("<ServerPort>"+QString("%1").arg(this->serverport)+"</ServerPort>\n");
    list.append("<TimeCode>"+QString("%1").arg(this->timecode)+"</TimeCode>\n");
    list.append("<TimeSlide>"+QString("%1").arg(this->timeslide)+"</TimeSlide>\n");
    list.append("<BaseURL>"+QString("%1").arg(this->baseurl)+"</BaseURL>\n");
    list.append("<Password>"+this->password+"</Password>\n");
    list.append("<AppVersion>"+this->appversion+"</AppVersion>\n");
    list.append("</Aquarello>\n");
    QString result = "";
    QString s;
    foreach( s, list )
            result +=s;
    out << result;
    file.close();

}
