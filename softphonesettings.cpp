#include "softphonesettings.h"

SoftPhoneSettings::SoftPhoneSettings(QObject *parent) :
    QObject(parent)
{
    parseXML("./settings/softphone.xml");
}


SoftPhoneSettings::SoftPhoneSettings(QObject *parent, QString XmlFile) :
    QObject(parent)
{
    parseXML(XmlFile);
}

void SoftPhoneSettings::parseXML(QString XmlFile) {

    QFile file (XmlFile);
    /* If we can't open it, let's show an error message. */
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        /*QMessageBox::critical(this,
                              "QXSRExample::parseXML",
                              "Couldn't open settings/softphone.xml",
                              QMessageBox::Ok);*/
        qDebug() << "Couldn't open settings/softphone.xml" << endl;
        return;
    }
    /* QXmlStreamReader takes any QIODevice. */
    QXmlStreamReader xml(&file);
    QMap<QString, QString> Softphone;
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
            if(xml.name() == "softphone") {
                continue;
            }
            /* If it's named person, we'll dig the information from there.*/
            if(xml.name() == "sipRegistration") {
                Softphone = this->parseSoftphone(xml);
            }
        }
    }
    /* Error handling. */
    /*if(xml.hasError()) {
        QMessageBox::critical(this,
                              "QXSRExample::parseXML",
                              xml.errorString(),
                              QMessageBox::Ok);
    }*/
    /* Removes any device() or data from the reader
     * and resets its internal state to the initial state. */
    xml.clear();
    file.close();
    this->setSettings(Softphone);
}

QMap<QString, QString> SoftPhoneSettings::parseSoftphone(QXmlStreamReader& xml) {
    QMap<QString, QString> Softphone;
    /* Let's check that we're really getting a person. */
    if(xml.tokenType() != QXmlStreamReader::StartElement &&
            xml.name() == "sipRegistration") {
        return Softphone;
    }
    /* Let's get the attributes for person */
    QXmlStreamAttributes attributes = xml.attributes();
    /* Let's check that person has id attribute. */
    if(attributes.hasAttribute("id")) {
        /* We'll add it to the map. */
        Softphone["id"] = attributes.value("id").toString();
    }
    /* Next element... */
    xml.readNext();
    /*
     * We're going to loop over the things because the order might change.
     * We'll continue the loop until we hit an EndElement named person.
     */
    while(!(xml.tokenType() == QXmlStreamReader::EndElement &&
            xml.name() == "sipRegistration")) {
        if(xml.tokenType() == QXmlStreamReader::StartElement) {
            if(xml.name() == "sipDomain") {
                this->addElementDataToMap(xml, Softphone);
            }
            if(xml.name() == "sipUserName") {
                this->addElementDataToMap(xml, Softphone);
            }
            if(xml.name() == "sipPassword") {
                this->addElementDataToMap(xml, Softphone);
            }
            if(xml.name() == "selfPreview") {
                this->addElementDataToMap(xml, Softphone);
            }
            if(xml.name() == "externalVideo") {
                this->addElementDataToMap(xml, Softphone);
            }
        }
        /* ...and next... */
        xml.readNext();
    }
    return Softphone;
}

void SoftPhoneSettings::addElementDataToMap(QXmlStreamReader &xml,
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

void SoftPhoneSettings::setSettings(QMap<QString,QString> & Softphone) {
    if(!Softphone.isEmpty()) {

        sipDomain = Softphone["sipDomain"];
        sipUserName = Softphone["sipUserName"];
        sipPassword = Softphone["sipPassword"];

        selfPreview = (Softphone["selfPreview"]=="1")?true:false;
        externalVideo = (Softphone["externalVideo"]=="1")?true:false;

    }else{
        sipDomain = "192.168.100.15";
        sipUserName = "156";
        sipPassword = "pwd156";

        selfPreview = false;
        externalVideo = false;

    }
}
void SoftPhoneSettings::writeConfigDefault(QString filename)
{
    QFile file(filename);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    QList<QString> list;
    list.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n");
    list.append("<softphone>\n");
    list.append("   <sipRegistrations>\n");
    list.append("      <sipDomain>192.168.3.201</sipDomain>\n");
    list.append("      <sipUserName>202</sipUserName>\n");
    list.append("      <sipPassword>202</sipPassword>\n");
    list.append("      <selfPreview>1</selfPreview>\n");
    list.append("      <externalVideo>1</externalVideo>\n");
    list.append("   </sipRegistration>\n");
    list.append("</softphone>\n");
    QString result = "";
    QString s;
    foreach( s, list )
            result +=s;
    out << result;
    file.close();

}
