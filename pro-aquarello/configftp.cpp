#include "configftp.h"

ConfigFtp::ConfigFtp(QObject *parent) :
    QObject(parent)
{
    PATH =QCoreApplication::applicationDirPath();
    root = "ConfigFtp";
    ftpserver = "0.0.0.0";
    ftpport = 21;
    ftpuser = "ftp";
    ftppasswd = "tccperu";
    ftpprefname = "images";

    //parse XMl
    this->parseXML();
    if(!params.isEmpty()){
        if (params[0]["FtpServer"]!="")
            this->ftpserver=params[0]["FtpServer"];
        if (params[0]["FtpPort"]!="")
            this->ftpport=params[0]["FtpPort"].toInt();
        if (params[0]["FtpUser"]!="")
            this->ftpuser=params[0]["FtpUser"];
        if (params[0]["FtpPasswd"]!="")
            this->ftppasswd=params[0]["FtpPasswd"];
        if (params[0]["FtpPrefName"]!="")
            this->ftpprefname=params[0]["FtpPrefName"];
        if (params[0]["FtpFileRef"]!="")
            this->ftpfileref=params[0]["FtpFileRef"];

    }
}
QString ConfigFtp::getFtpServer(){
    return this->ftpserver;
}
int ConfigFtp::getFtpPort(){
    return this->ftpport;
}
QString ConfigFtp::getFtpUser(){
    return this->ftpuser;
}
QString ConfigFtp::getFtpPasswd(){
    return this->ftppasswd;
}
QString ConfigFtp::getFtpPrefName(){
    return this->ftpprefname;
}
QString ConfigFtp::getFtpFileRef(){
    return this->ftpfileref;
}
void ConfigFtp::setFtpFileRef(QString filename){
    this->ftpfileref = filename;
}
void ConfigFtp::updateConfigFile(){
        //system(qPrintable(AQUARELLO_RW_CMD));
        writeConfigFile(PATH+"/"+CONFIG_FTPFILE);
        //system(qPrintable(AQUARELLO_RD_CMD));
}

void ConfigFtp::parseXML() {
    /* We'll parse the example.xml */
    QFile* file = new QFile(PATH+"/"+CONFIG_FTPFILE);

    /* If we can't open it, let's show an error message. */
    if (!file->open(QIODevice::ReadOnly | QIODevice::Text)) {
        printf("No se pudo abrir config.xml\r\\n");
        //system(qPrintable(AQUARELLO_RW_CMD));
        writeConfigDefault(PATH+"/"+CONFIG_FTPFILE);
        //system(qPrintable(AQUARELLO_RD_CMD));
        if(!file->open(QIODevice::ReadOnly | QIODevice::Text)){
            return;
        }

    }
    //printf("open configftp.xml\r\\n");
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
            if(xml.name() == root) {
                params.append(this->parseConfigFtp(xml));
            }
        }
    }
    /* Error handling. */
    if(xml.hasError()) {
        printf("Imposible parse config.xml\r\\n");
        file->close();
        //system(qPrintable(AQUARELLO_RW_CMD));
        writeConfigDefault(PATH+"/"+CONFIG_FTPFILE);
        //system(qPrintable(AQUARELLO_RD_CMD));
        return;
    }
    xml.clear();
    file->close();
}
QMap<QString, QString> ConfigFtp::parseConfigFtp(QXmlStreamReader& xml) {
    QMap<QString, QString> configftp;
    /* Let's check that we're really getting a person. */
    if(xml.tokenType() != QXmlStreamReader::StartElement &&
       xml.name() == root) {
        return configftp;
    }
    /* Next element... */
    xml.readNext();
    /*
     * We're going to loop over the things because the order might change.
     * We'll continue the loop until we hit an EndElement named person.
     */
    while(!(xml.tokenType() == QXmlStreamReader::EndElement &&
            xml.name() == root)) {
        if(xml.tokenType() == QXmlStreamReader::StartElement) {
            /* We've found first name.

            */
            if(xml.name() == "FtpServer") {
                this->addElementDataToMap(xml, configftp);
            }
            if(xml.name() == "FtpPort") {
                this->addElementDataToMap(xml, configftp);
            }
            if(xml.name() == "FtpUser") {
                this->addElementDataToMap(xml, configftp);
            }
            if(xml.name() == "FtpPasswd") {
                this->addElementDataToMap(xml, configftp);
            }
            if(xml.name() == "FtpPrefName") {
                this->addElementDataToMap(xml, configftp);
            }
            if(xml.name() == "FtpFileRef") {
                this->addElementDataToMap(xml, configftp);
            }

        }
        /* ...and next... */
        xml.readNext();
    }
    return configftp;
}

void ConfigFtp::addElementDataToMap(QXmlStreamReader& xml,
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
void ConfigFtp::writeConfigDefault(QString filename)
{
    QFile file(filename);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    QList<QString> list;
    list.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n");
    list.append("<ConfigFtp>\n");
    list.append("<FtpServer>0.0.0.0</FtpServer>\n");
    list.append("<FtpPort>21</FtpPort>\n");
    list.append("<FtpUser>ftp</FtpUser>\n");
    list.append("<FtpPasswd>tccperu</FtpPasswd>\n");
    list.append("<FtpPrefName>images</FtpPrefName>\n");
    list.append("<FtpFileRef></FtpFileRef>\n");
    list.append("</ConfigFtp>\n");
    QString result = "";
    QString s;
    foreach( s, list )
            result +=s;
    out << result;
    file.close();

}
void ConfigFtp::writeConfigFile(QString filename)
{
    QFile file(filename);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    QList<QString> list;
    list.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n");
    list.append("<ConfigFtp>\n");
    list.append("<FtpServer>"+this->ftpserver+"</FtpServer>\n");
    list.append("<FtpPort>"+QString("%1").arg(this->ftpport)+"</FtpPort>\n");
    list.append("<FtpUser>"+this->ftpuser+"</FtpUser>\n");
    list.append("<FtpPasswd>"+this->ftppasswd+"</FtpPasswd>\n");
    list.append("<FtpPrefName>"+this->ftpprefname+"</FtpPrefName>\n");
    list.append("<FtpFileRef>"+this->ftpfileref+"</FtpFileRef>\n");
    list.append("</ConfigFtp>\n");
    QString result = "";
    QString s;
    foreach( s, list )
            result +=s;
    out << result;
    file.close();

}
