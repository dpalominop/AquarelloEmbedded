#include "barcodescannersettings.h"

BarcodeScannerSettings::BarcodeScannerSettings(QWidget *parent) :
    QWidget(parent)
{
    fillSerialPortMap();
    fillAllowedBaudRates();
    parseXML("./settings/scanner.xml");
}


BarcodeScannerSettings::BarcodeScannerSettings(QWidget *parent , QString configFile) :
    QWidget(parent)
{
    fillSerialPortMap();
    fillAllowedBaudRates();
    parseXML(configFile);

}

void BarcodeScannerSettings::fillSerialPortMap()
{
    serialPortMap["/dev/ttyS0"]=0;
    serialPortMap["/dev/ttyS1"]=1;
    serialPortMap["/dev/ttyS2"]=2;
    serialPortMap["/dev/ttyS3"]=3;
    serialPortMap["/dev/ttyS4"]=4;
    serialPortMap["/dev/ttyS5"]=5;
    serialPortMap["/dev/ttyS6"]=6;
    serialPortMap["/dev/ttyS7"]=7;
    serialPortMap["/dev/ttyS8"]=8;
    serialPortMap["/dev/ttyS9"]=9;
    serialPortMap["/dev/ttyS10"]=10;
    serialPortMap["/dev/ttyS11"]=11;
    serialPortMap["/dev/ttyS12"]=12;
    serialPortMap["/dev/ttyS13"]=13;
    serialPortMap["/dev/ttyS14"]=14;
    serialPortMap["/dev/ttyS15"]=15;
    serialPortMap["/dev/ttyACM0"]=16;
    serialPortMap["/dev/ttyUSB0"]=17;
    serialPortMap["/dev/ttyUSB1"]=18;
    serialPortMap["/dev/ttyUSB2"]=19;
    serialPortMap["/dev/ttyUSB3"]=20;
    serialPortMap["/dev/ttyUSB4"]=21;
    serialPortMap["/dev/ttyUSB5"]=22;

    serialPortMap["COM1"]=0;
    serialPortMap["COM2"]=1;
    serialPortMap["COM3"]=2;
    serialPortMap["COM4"]=3;
    serialPortMap["COM5"]=4;
    serialPortMap["COM6"]=5;
    serialPortMap["COM7"]=6;
    serialPortMap["COM8"]=7;
    serialPortMap["COM9"]=8;
    serialPortMap["COM10"]=9;
    serialPortMap["COM11"]=10;
    serialPortMap["COM12"]=11;
    serialPortMap["COM13"]=12;
    serialPortMap["COM14"]=13;
    serialPortMap["COM15"]=14;
    serialPortMap["COM16"]=15;

}

void BarcodeScannerSettings::fillAllowedBaudRates()
{

allowedBaudRates <<
                    50  <<
                    75  <<
                    110 <<
                    134 <<
                    150 <<
                    200 <<
                    300 <<
                    600 <<
                    1200 <<
                    1800 <<
                    2400 <<
                    4800 <<
                    9600 <<
                    19200 <<
                    38400 <<
                    57600 <<
                    115200 <<
                    230400 <<
                    460800 <<
                    500000 <<
                    576000  <<
                    921600  <<
                    1000000;


}

void BarcodeScannerSettings::parseXML(QString XmlFile)
{
    QFile file(XmlFile);
    /* If we can't open it, let's show an error message. */
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this,
                              "QXSRExample::parseXML",
                              "Couldn't open ./settings/scanner.xml",
                              QMessageBox::Ok);
        return;
    }
    /* QXmlStreamReader takes any QIODevice. */
    QXmlStreamReader xml(&file);
    QMap<QString, QString> Scanner;
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
                if(xml.name() == "scanner") {
                    continue;
                }
                /* If it's named person, we'll dig the information from there.*/
                if(xml.name() == "scannerSettings") {
                    Scanner = this->parseScanner(xml);
                }
        }

    }

    /* Error handling. */
    if(xml.hasError()) {
        QMessageBox::critical(this,
                              "QXSRExample::parseXML",
                              xml.errorString(),
                              QMessageBox::Ok);
    }
    /* Removes any device() or data from the reader
     * and resets its internal state to the initial state. */
    xml.clear();
    this->setSettings(Scanner);
}

QMap<QString, QString> BarcodeScannerSettings::parseScanner(QXmlStreamReader &xml)
{
    QMap<QString, QString> Scanner;
    /* Let's check that we're really getting a person. */
    if(xml.tokenType() != QXmlStreamReader::StartElement &&
            xml.name() == "scannerSettings") {
        return Scanner;
    }
    /* Let's get the attributes for person */
    QXmlStreamAttributes attributes = xml.attributes();
    /* Let's check that person has id attribute. */
    if(attributes.hasAttribute("id")) {
        /* We'll add it to the map. */
        Scanner["id"] = attributes.value("id").toString();
    }
    /* Next element... */
    xml.readNext();
    /*
     * We're going to loop over the things because the order might change.
     * We'll continue the loop until we hit an EndElement named person.
     */
    while(!(xml.tokenType() == QXmlStreamReader::EndElement &&
            xml.name() == "scannerSettings")) {
        if(xml.tokenType() == QXmlStreamReader::StartElement) {
            if(xml.name() == "serialBaudRate") {
                this->addElementDataToMap(xml, Scanner);
            }
            if(xml.name() == "serialPort") {
                this->addElementDataToMap(xml, Scanner);
            }

        }
        /* ...and next... */
        xml.readNext();
    }
    return Scanner;
}

void BarcodeScannerSettings::addElementDataToMap(QXmlStreamReader &xml,
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

void BarcodeScannerSettings::setSettings(QMap<QString, QString> &Scanner)
{
    if(!Scanner.isEmpty()) {

        if(Scanner["serialBaudRate"] != "")
        {
            QString serialBaudRateStr = Scanner["serialBaudRate"] ;
            serialBaudRateStr = serialBaudRateStr.trimmed();
            baudRate = serialBaudRateStr.toInt();
            if(allowedBaudRates.indexOf(baudRate)!=-1)
            {
                std::cout<<"Scanner settings parser:  baudRate="<<baudRate<<std::endl;
            }  else
            {
                std::cout<<"Scanner settings parser:    NOT VALID serialBaudRate value. Using default."<<std::endl;
                baudRate = 9600;
            }

        }   else
        {

            std::cout<<"Scanner settings parser:    MISSING serialBaudRate value. Using default."<<std::endl;
            baudRate = 9600;
        }

        if(Scanner["serialPort"] != "")
        {
            QString serialPortStr = Scanner["serialPort"] ;
            serialPortStr = serialPortStr.trimmed();
            if(serialPortMap.contains(serialPortStr))
            {
                serialPort=serialPortMap.value(serialPortStr);
                std::cout<<"Scanner settings parser:  serialPort="<<serialPortStr.toStdString()
                           <<" (#"<<serialPort<<")"<<std::endl;

            }  else
            {
                std::cout<<"Scanner settings parser:    NOT VALID serialPort value. Using default."<<std::endl;
                serialPort = serialPortMap.value("/dev/ttyUSB0");
            }

        }   else
        {

            std::cout<<"Scanner settings parser:    MISSING serialPort value. Using default"<<std::endl;
            serialPort = serialPortMap.value("/dev/ttyUSB0");
        }

    }else{

        baudRate = 9600;
        serialPort = serialPortMap.value("/dev/ttyACM0");
    }

    std::cout<<"Scanner settings parser: JOB DONE!"<<std::endl;
}
