#include "audiocodecsettings.h"


AudioCodecSettings::AudioCodecSettings(QObject *parent) :
    QObject(parent)
{
    QString PATH = QCoreApplication::applicationDirPath();
    parseXML(PATH + "/settings/audiocodecs.xml");
}

AudioCodecSettings::AudioCodecSettings(QObject *parent, QString XmlFile) :
    QObject(parent)
{
    parseXML(XmlFile);
}

void AudioCodecSettings::parseXML(QString XmlFile) {
    /* We'll parse the example.xml */
    QFile file (XmlFile);
    /* If we can't open it, let's show an error message. */
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
       /*                     "Couldn't open settings/contacts.xml",
                              QMessageBox::Ok);*/
        qDebug() << "ERROR ABRIENDO CODECS DE AUDIO: " << XmlFile << endl;
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
            if(xml.name() == "codecs") {
                continue;
            }
            /* If it's named person, we'll dig the information from there.*/
            if(xml.name() == "codec") {
                codecs.append(this->parseCodec(xml));
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
}

QMap<QString, QString> AudioCodecSettings::parseCodec(QXmlStreamReader& xml) {
    QMap<QString, QString> codec;
    /* Let's check that we're really getting a person. */
    if(xml.tokenType() != QXmlStreamReader::StartElement &&
            xml.name() == "codec") {
        return codec;
    }
    /* Let's get the attributes for person */
    QXmlStreamAttributes attributes = xml.attributes();
    /* Let's check that person has id attribute. */
    if(attributes.hasAttribute("id")) {
        /* We'll add it to the map. */
        codec["id"] = attributes.value("id").toString();
    }

    if(attributes.hasAttribute("enabled")) {
        /* We'll add it to the map. */
        codec["enabled"] = attributes.value("enabled").toString();
    }

    /* Next element... */
    xml.readNext();
    /*
     * We're going to loop over the things because the order might change.
     * We'll continue the loop until we hit an EndElement named person.
     */
    while(!(xml.tokenType() == QXmlStreamReader::EndElement &&
            xml.name() == "codec")) {
        if(xml.tokenType() == QXmlStreamReader::StartElement) {
            if(xml.name() == "name") {
                //qDebug()<< "xml.name(): " << xml.name() << endl;
                this->addElementDataToMap(xml, codec);
            }
            if(xml.name() == "clock") {
                this->addElementDataToMap(xml, codec);
            }
            if(xml.name() == "channel") {
                this->addElementDataToMap(xml, codec);
            }
        }
        /* ...and next... */
        xml.readNext();
    }
    return codec;
}

void AudioCodecSettings::addElementDataToMap(QXmlStreamReader& xml,
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
