#include "aquarellosettings.h"

AquarelloSettings::AquarelloSettings(QWidget *parent) :
    QWidget(parent)
{
    QString PATH = QCoreApplication::applicationDirPath();
    parseXML(PATH+"/settings/aquarello.xml");
}


AquarelloSettings::AquarelloSettings (QWidget *parent, QString XmlFile) :
    QWidget(parent)
{
    parseXML(XmlFile);
}

void AquarelloSettings::parseXML(QString XmlFile)
{
    QFile file(XmlFile);
    /* If we can't open it, let's show an error message. */
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this,
                              "QXSRExample::parseXML",
                              "Couldn't open ./settings/aquarello.xml",
                              QMessageBox::Ok);
        writeConfigDefault(XmlFile);
        parseXML(XmlFile);
        return;
    }
    /* QXmlStreamReader takes any QIODevice. */
    QXmlStreamReader xml(&file);
    QMap<QString, QString> Aquarello;
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

                /* If it's named persons, we'll go to the next.*/
                if(xml.name() == "aquarello") {
                    continue;
                }
                /* If it's named person, we'll dig the information from there.*/
                if(xml.name() == "aquarelloSettings") {
                    Aquarello = this->parseAquarello(xml);
                }
        }

    }

    /* Error handling. */
    if(xml.hasError()) {
        QMessageBox::critical(this,
                              "QXSRExample::parseXML",
                              xml.errorString(),
                              QMessageBox::Ok);
        writeConfigDefault(XmlFile);
        //return;
    }
    /* Removes any device() or data from the reader
     * and resets its internal state to the initial state. */
    xml.clear();
    this->setSettings(Aquarello);
}

QMap<QString, QString> AquarelloSettings::parseAquarello(QXmlStreamReader &xml)
{
    QMap<QString, QString> Aquarello;
    /* Let's check that we're really getting a person. */
    if(xml.tokenType() != QXmlStreamReader::StartElement &&
            xml.name() == "aquarelloSettings") {
        return Aquarello;
    }
    /* Let's get the attributes for person */
    QXmlStreamAttributes attributes = xml.attributes();
    /* Let's check that person has id attribute. */
    if(attributes.hasAttribute("id")) {
        /* We'll add it to the map. */
        Aquarello["id"] = attributes.value("id").toString();
    }
    /* Next element... */
    xml.readNext();
    /*
     * We're going to loop over the things because the order might change.
     * We'll continue the loop until we hit an EndElement named person.
     */
    while(!(xml.tokenType() == QXmlStreamReader::EndElement &&
            xml.name() == "aquarelloSettings")) {
        if(xml.tokenType() == QXmlStreamReader::StartElement) {
            /* We want PictureCatalog. */
            if(xml.name() == "usingPictureCatalog") {
                this->addElementDataToMap(xml, Aquarello);
            }
            /* We want VideoCatalog. */
            if(xml.name() == "usingVideoCatalog") {
                this->addElementDataToMap(xml, Aquarello);
            }
            /* We want SoftPhone. */
            if(xml.name() == "usingSoftPhone") {
                this->addElementDataToMap(xml, Aquarello);
            }
            if(xml.name() == "usingBarcodeScanner") {
                this->addElementDataToMap(xml, Aquarello);
            }
            // We set pictureCatalogDelay_ms.
            //if(xml.name() == "pictureCatalogDelay_ms") {
            //    this->addElementDataToMap(xml, Aquarello);
            //}
            //if(xml.name() == "barcodeCatalogDelay_ms") {
            //    this->addElementDataToMap(xml, Aquarello);
            //}
            if(xml.name() == "PortCOM") {
                this->addElementDataToMap(xml, Aquarello);
            }
            if(xml.name() == "BaudRate") {
                this->addElementDataToMap(xml, Aquarello);
            }
            if(xml.name() == "Terminal") {
                this->addElementDataToMap(xml, Aquarello);
            }
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
            if(xml.name() == "Orientation") {
                this->addElementDataToMap(xml, Aquarello);
            }

        }
        /* ...and next... */
        xml.readNext();
    }
    return Aquarello;
}

void AquarelloSettings::addElementDataToMap(QXmlStreamReader &xml,
                                            QMap<QString, QString> &map) const {
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

void AquarelloSettings::setSettings(QMap<QString, QString> &Aquarello)
{
    usingPictureCatalog = true;
    usingVideoCatalog = false;
    usingSoftPhone = true;
    usingBarcodeScanner = true;
    pictureCatalogDelay_ms = 10000;
    barcodeCatalogDelay_ms = 10000;
    terminal = "Aquarelle";
    serverip = "192.168.100.3";
    serverport = 1234;
    portcom = "/dev/ttyACM0";
    baudrate = 9600;
    localipaddr = "192.168.0.1";
    serialnumber = "XXXXXXXXXXXX";
    devicetypeid = "YYYYYYYYYYYY";
    password = "tccperu";
    baseurl = "http://localhost/cgi-bin/demo.cgi?gencode=";
    appversion = APP_VERSION;
    orientation = "landscape";
    width = 1024;
    height = 768;

    if(!Aquarello.isEmpty()) {
        if(Aquarello["usingPictureCatalog"]!="") usingPictureCatalog = (Aquarello["usingPictureCatalog"]=="1")?true:false;
        if(Aquarello["usingVideoCatalog"]!="") usingVideoCatalog = (Aquarello["usingVideoCatalog"]=="1")?true:false;
        if(Aquarello["usingSoftPhone"]!="") usingSoftPhone = (Aquarello["usingSoftPhone"]=="1")?true:false;
        if(Aquarello["usingBarcodeScanner"]!="") usingBarcodeScanner = (Aquarello["usingBarcodeScanner"]=="1")?true:false;

        //pictureCatalogDelay_ms = Aquarello["pictureCatalogDelay_ms"].toInt();
        //barcodeCatalogDelay_ms = Aquarello["barcodeCatalogDelay_ms"].toInt();
        if(Aquarello["TimeSlide"]!="") pictureCatalogDelay_ms = Aquarello["TimeSlide"].toInt()*1000;
        if(Aquarello["TimeCode"]!="") barcodeCatalogDelay_ms = Aquarello["TimeCode"].toInt()*1000;
        if(Aquarello["PortCOM"]!="") portcom = Aquarello["PortCOM"];
        if(Aquarello["BaudRate"]!="") baudrate = Aquarello["BaudRate"].toInt();
        if(Aquarello["Terminal"]!="") terminal = Aquarello["Terminal"];
        if(Aquarello["ServerIP"]!="") serverip = Aquarello["ServerIP"];
        if(Aquarello["ServerPort"]!="") serverport = Aquarello["ServerPort"].toInt();
        if(Aquarello["BaseURL"]!="") baseurl = Aquarello["BaseURL"];
        if(Aquarello["Password"]!="") password = Aquarello["Password"];
        if(Aquarello["AppVersion"]!="") appversion = Aquarello["AppVersion"];
        if(Aquarello["Orientation"]!="") orientation = Aquarello["Orientation"];

    }
}

void AquarelloSettings::writeConfigDefault(QString filename)
{
    QFile file(filename);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    QList<QString> list;
    list.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n");
    list.append("<aquarello>\n");
    list.append("   <aquarelloSettings>\n");
    list.append("      <usingPictureCatalog>1</usingPictureCatalog>\n");
    list.append("      <usingVideoCatalog>1</usingVideoCatalog>\n");
    list.append("      <usingSoftPhone>1</usingSoftPhone>\n");
    list.append("      <usingBarcodeScanner>1</usingBarcodeScanner>\n");
    list.append("      <PortCOM>/dev/ttyACM0</PortCOM>\n");
    list.append("      <BaudRate>9600</BaudRate>\n");
    list.append("      <Terminal>Aquarelle</Terminal>\n");
    list.append("      <ServerIP>192.168.100.6</ServerIP>\n");
    list.append("      <ServerPort>1234</ServerPort>\n");
    list.append("      <TimeCode>10</TimeCode>\n");
    list.append("      <TimeSlide>15</TimeSlide>\n");
    list.append("      <BaseURL>http://localhost/cgi-bin/demo.cgi?gencode=</BaseURL>\n");
    list.append("      <Password>tccperu</Password>\n");
    list.append("      <AppVersion>"+QString(APP_VERSION)+"</AppVersion>\n");
    list.append("   </aquarelloSettings>\n");
    list.append("</aquarello>\n");
    QString result = "";
    QString s;
    foreach( s, list )
            result +=s;
    out << result;
    file.close();

}

