#include "contactsettings.h"

ContactSettings::ContactSettings(QWidget *parent) :
    QWidget(parent)
{
    parseXML("./settings/contacts.xml");
}

ContactSettings::ContactSettings(QWidget *parent, QString XmlFile) :
    QWidget(parent)
{
    parseXML(XmlFile);
}

void ContactSettings::parseXML(QString XmlFile) {
    /* We'll parse the example.xml */
    QFile file (XmlFile);
    /* If we can't open it, let's show an error message. */
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this,
                              "QXSRExample::parseXML",
                              "Couldn't open settings/contacts.xml",
                              QMessageBox::Ok);
        return;
    }
    /* QXmlStreamReader takes any QIODevice. */
    QXmlStreamReader xml(&file);

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
            if(xml.name() == "contacts") {
                continue;
            }
            /* If it's named person, we'll dig the information from there.*/
            if(xml.name() == "contact") {
                contacts.append(this->parseContact(xml));
                qSort(contacts.begin(), contacts.end(), qMapLessThan);
            }
        }
    }
    /* Error handling. */
    if(xml.hasError()) {
        QMessageBox::critical(this,
                              "QXSRExample::parseXML",
                              xml.errorString(),
                              QMessageBox::Ok);
        contacts.clear();

    }
    /* Removes any device() or data from the reader
     * and resets its internal state to the initial state. */
    xml.clear();
}

QMap<QString, QString> ContactSettings::parseContact(QXmlStreamReader& xml) {
    QMap<QString, QString> contact;
    /* Let's check that we're really getting a person. */
    if(xml.tokenType() != QXmlStreamReader::StartElement &&
            xml.name() == "contact") {
        return contact;
    }
    /* Let's get the attributes for person */
    QXmlStreamAttributes attributes = xml.attributes();
    /* Let's check that person has id attribute. */
    if(attributes.hasAttribute("id")) {
        /* We'll add it to the map. */
        contact["id"] = attributes.value("id").toString();
    }

    if(attributes.hasAttribute("enabled")) {
        /* We'll add it to the map. */
        contact["enabled"] = attributes.value("enabled").toString();
    }

    /* Next element... */
    xml.readNext();
    /*
     * We're going to loop over the things because the order might change.
     * We'll continue the loop until we hit an EndElement named person.
     */
    while(!(xml.tokenType() == QXmlStreamReader::EndElement &&
            xml.name() == "contact")) {
        if(xml.tokenType() == QXmlStreamReader::StartElement) {
            /* We've found first name. */
            if(xml.name() == "firstname") {
                this->addElementDataToMap(xml, contact);
            }
            /* We've found surname. */
            if(xml.name() == "surname") {
                this->addElementDataToMap(xml, contact);
            }
            /* We've found phone number. */
            if(xml.name() == "phone") {
                this->addElementDataToMap(xml, contact);
            }
        }
        /* ...and next... */
        xml.readNext();
    }
    return contact;
}

void ContactSettings::addElementDataToMap(QXmlStreamReader& xml,
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
