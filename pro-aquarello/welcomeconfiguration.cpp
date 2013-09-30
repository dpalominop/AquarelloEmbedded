#include "welcomeconfiguration.h"
#include "base.h"

WelcomeConfiguration::WelcomeConfiguration(QObject *parent) :
    QObject(parent)
{
    //default
    PATH =QCoreApplication::applicationDirPath();
    msgtxt="";
    msgx=0;
    msgy=0;
    msgforecolor="FFFFFF";
    msgcolor="FFFFFF";
    msgsize=0;
    msgeffect="";
    msgstyle="";
    msgweight="";
    msgvisible="";
    //parse XML
    this->parseXML();
    if(params[0]["WelcomeMsgTxt"]!="")
        this->msgtxt=params[0]["WelcomeMsgTxt"];
    if(params[0]["WelcomeMsgX"]!="")
        this->msgx=params[0]["WelcomeMsgX"].toInt();
    if(params[0]["WelcomeMsgY"]!="")
        this->msgy=params[0]["WelcomeMsgY"].toInt();
    if(params[0]["WelcomeMsgForecolor"]!="")
        this->msgforecolor=params[0]["WelcomeMsgForecolor"];
    if(params[0]["WelcomeMsgColor"]!="")
        this->msgcolor=params[0]["WelcomeMsgColor"];
    if(params[0]["WelcomeMsgSize"]!="")
        this->msgsize=params[0]["WelcomeMsgSize"].toInt();
    if(params[0]["WelcomeMsgEffect"]!="")
        this->msgeffect=params[0]["WelcomeMsgEffect"];
    if(params[0]["WelcomeMsgStyle"]!="")
        this->msgstyle=params[0]["WelcomeMsgStyle"];
    if(params[0]["WelcomeMsgWeight"]!="")
        this->msgweight=params[0]["WelcomeMsgWeight"];
    if(params[0]["WelcomeMsgVisible"]!="")
        this->msgvisible=params[0]["WelcomeMsgVisible"];
}

void WelcomeConfiguration::setMsgTxt(QString value){
    this->msgtxt=value;
}

QString WelcomeConfiguration::getMsgTxt(){
    return this->msgtxt;
    }

int WelcomeConfiguration::getMsgX(){
    return this->msgx;
    }

int WelcomeConfiguration::getMsgY(){
    return this->msgy;
    }

QString WelcomeConfiguration::getMsgForeColor(){
    return this->msgforecolor;
    }

QString WelcomeConfiguration::getMsgColor(){
    return this->msgcolor;
    }

int WelcomeConfiguration::getMsgSize(){
    return this->msgsize;
    }

QString WelcomeConfiguration::getMsgEffect(){
    return this->msgeffect;
    }

QString WelcomeConfiguration::getMsgStyle(){
    return this->msgstyle;
    }

QString WelcomeConfiguration::getMsgWeight(){
    return this->msgweight;
    }

QString WelcomeConfiguration::getMsgVisible(){
    return this->msgvisible;
    }

void WelcomeConfiguration::parseXML() {
    /* We'll parse the example.xml */
    QFile* file = new QFile(PATH+"/"+WELCOME_FILE);
    /* If we can't open it, let's show an error message. */
    if (!file->open(QIODevice::ReadOnly | QIODevice::Text)) {
        printf("No se pudo abrir config.xml\r\\n");
        writeConfigDefault(PATH+"/"+WELCOME_FILE);
        if(!file->open(QIODevice::ReadOnly | QIODevice::Text)){
            return;
        }
    }
    printf("open welcome.xml\r\\n");
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
            if(xml.name() == "Message") {
                params.append(this->parseMessage(xml));
            }
        }
    }
    /* Error handling. */
    if(xml.hasError()) {
        printf("Imposible parse config.xml\r\\n");
    }
    xml.clear();
    file->close();
}

QMap<QString, QString> WelcomeConfiguration::parseMessage(QXmlStreamReader& xml) {
    QMap<QString, QString> Message;
    /* Let's check that we're really getting a person. */
    if(xml.tokenType() != QXmlStreamReader::StartElement &&
       xml.name() == "Message") {
        return Message;
    }
    /* Next element... */
    xml.readNext();
    /*
     * We're going to loop over the things because the order might change.
     * We'll continue the loop until we hit an EndElement named person.
     */
    while(!(xml.tokenType() == QXmlStreamReader::EndElement &&
            xml.name() == "Message")) {
        if(xml.tokenType() == QXmlStreamReader::StartElement) {
            /* We've found first name.

            */
            if(xml.name() == "WelcomeMsgTxt") {
                this->addElementDataToMap(xml, Message);
            }
            if(xml.name() == "WelcomeMsgX") {
                this->addElementDataToMap(xml, Message);
            }
            if(xml.name() == "WelcomeMsgY") {
                this->addElementDataToMap(xml, Message);
            }
            if(xml.name() == "WelcomeMsgForecolor") {
                this->addElementDataToMap(xml, Message);
            }
            if(xml.name() == "WelcomeMsgColor") {
                this->addElementDataToMap(xml, Message);
            }
            if(xml.name() == "WelcomeMsgSize") {
                this->addElementDataToMap(xml, Message);
            }
            if(xml.name() == "WelcomeMsgEffect") {
                this->addElementDataToMap(xml, Message);
            }
            if(xml.name() == "WelcomeMsgStyle") {
                this->addElementDataToMap(xml, Message);
            }
            if(xml.name() == "WelcomeMsgWeight") {
                this->addElementDataToMap(xml, Message);
            }
            if(xml.name() == "WelcomeMsgVisible") {
                this->addElementDataToMap(xml, Message);
            }
        }
        /* ...and next... */
        xml.readNext();
    }
    return Message;
}

void WelcomeConfiguration::addElementDataToMap(QXmlStreamReader& xml,
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
void WelcomeConfiguration::writeConfigDefault(QString filename)
{
    QFile file(filename);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    QList<QString> list;
    list.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n");
    list.append("<Message>\n");
    list.append("<WelcomeMsgTxt>GEMPSY-TCC</WelcomeMsgTxt>\n");
    list.append("<WelcomeMsgX>150</WelcomeMsgX>\n");
    list.append("<WelcomeMsgY>100</WelcomeMsgY>\n");
    list.append("<WelcomeMsgForecolor>53E61E</WelcomeMsgForecolor>\n");
    list.append("<WelcomeMsgColor>F51E0F</WelcomeMsgColor>\n");
    list.append("<WelcomeMsgSize>40</WelcomeMsgSize>\n");
    list.append("<WelcomeMsgEffect>Fixed</WelcomeMsgEffect>\n");
    list.append("<WelcomeMsgStyle>Normal</WelcomeMsgStyle>\n");
    list.append("<WelcomeMsgWeight>Normal</WelcomeMsgWeight>\n");
    list.append("<WelcomeMsgVisible>false</WelcomeMsgVisible>\n");
    list.append("</Message>\n");
    QString result = "";
    QString s;
    foreach( s, list )
            result +=s;
    out << result;
    file.close();

}

