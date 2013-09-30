#include "linemessages.h"
#include "base.h"

LineMessages::LineMessages(QObject *parent) :
    QObject(parent)
{
    //default
    PATH =QCoreApplication::applicationDirPath();
    Line1MsgX = 200;
    Line1MsgY = 50;
    Line1MsgForecolor = "53E61E";
    Line1MsgColor = "F51E0F";
    Line1MsgSize = 40;
    Line1MsgEffect = "Fixed";
    Line1MsgStyle = "Normal";
    Line1MsgWeight = "Normal";
    Line2MsgX = 200;
    Line2MsgY = 150;
    Line2MsgForecolor = "53E61E";
    Line2MsgColor = "F51E0F";
    Line2MsgSize = 40;
    Line2MsgEffect = "Fixed";
    Line2MsgStyle = "Normal";
    Line2MsgWeight = "Normal";
    Line3MsgX = 200;
    Line3MsgY = 250;
    Line3MsgForecolor = "53E61E";
    Line3MsgColor = "F51E0F";
    Line3MsgSize = 40;
    Line3MsgEffect = "Fixed";
    Line3MsgStyle = "Normal";
    Line3MsgWeight = "Normal";
    Line4MsgX = 200;
    Line4MsgY = 350;
    Line4MsgForecolor = "53E61E";
    Line4MsgColor = "F51E0F";
    Line4MsgSize = 40;
    Line4MsgEffect = "Fixed";
    Line4MsgStyle = "Normal";
    Line4MsgWeight = "Normal";

    this->parseXML();
    if(params[0]["Line1MsgX"]!="")
        this->Line1MsgX=params[0]["Line1MsgX"].toInt();
    if(params[0]["Line1MsgY"]!="")
        this->Line1MsgY=params[0]["Line1MsgY"].toInt();
    if(params[0]["Line1MsgForecolor"]!="")
        this->Line1MsgForecolor=params[0]["Line1MsgForecolor"];
    if(params[0]["Line1MsgColor"]!="")
        this->Line1MsgColor=params[0]["Line1MsgColor"];
    if(params[0]["Line1MsgSize"]!="")
        this->Line1MsgSize=params[0]["Line1MsgSize"].toInt();
    if(params[0]["Line1MsgEffect"]!="")
        this->Line1MsgEffect=params[0]["Line1MsgEffect"];
    if(params[0]["Line1MsgStyle"]!="")
        this->Line1MsgStyle=params[0]["Line1MsgStyle"];
    if(params[0]["Line1MsgWeight"]!="")
        this->Line1MsgWeight=params[0]["Line1MsgWeight"];
    //
    if(params[0]["Line2MsgX"]!="")
        this->Line2MsgX=params[0]["Line2MsgX"].toInt();
    if(params[0]["Line2MsgY"]!="")
        this->Line2MsgY=params[0]["Line2MsgY"].toInt();
    if(params[0]["Line2MsgForecolor"]!="")
        this->Line2MsgForecolor=params[0]["Line2MsgForecolor"];
    if(params[0]["Line2MsgColor"]!="")
        this->Line2MsgColor=params[0]["Line2MsgColor"];
    if(params[0]["Line2MsgSize"]!="")
        this->Line2MsgSize=params[0]["Line2MsgSize"].toInt();
    if(params[0]["Line2MsgEffect"]!="")
        this->Line2MsgEffect=params[0]["Line2MsgEffect"];
    if(params[0]["Line2MsgStyle"]!="")
        this->Line2MsgStyle=params[0]["Line2MsgStyle"];
    if(params[0]["Line2MsgWeight"]!="")
        this->Line2MsgWeight=params[0]["Line2MsgWeight"];
    //
    if(params[0]["Line3MsgX"]!="")
        this->Line3MsgX=params[0]["Line3MsgX"].toInt();
    if(params[0]["Line3MsgY"]!="")
        this->Line3MsgY=params[0]["Line3MsgY"].toInt();
    if(params[0]["Line3MsgForecolor"]!="")
        this->Line3MsgForecolor=params[0]["Line3MsgForecolor"];
    if(params[0]["Line3MsgColor"]!="")
        this->Line3MsgColor=params[0]["Line3MsgColor"];
    if(params[0]["Line3MsgSize"]!="")
        this->Line3MsgSize=params[0]["Line3MsgSize"].toInt();
    if(params[0]["Line3MsgEffect"]!="")
        this->Line3MsgEffect=params[0]["Line3MsgEffect"];
    if(params[0]["Line3MsgStyle"]!="")
        this->Line3MsgStyle=params[0]["Line3MsgStyle"];
    if(params[0]["Line3MsgWeight"]!="")
        this->Line3MsgWeight=params[0]["Line3MsgWeight"];
    //
    if(params[0]["Line4MsgX"]!="")
        this->Line4MsgX=params[0]["Line4MsgX"].toInt();
    if(params[0]["Line4MsgY"]!="")
        this->Line4MsgY=params[0]["Line4MsgY"].toInt();
    if(params[0]["Line4MsgForecolor"]!="")
        this->Line4MsgForecolor=params[0]["Line4MsgForecolor"];
    if(params[0]["Line4MsgColor"]!="")
        this->Line4MsgColor=params[0]["Line4MsgColor"];
    if(params[0]["Line4MsgSize"]!="")
        this->Line4MsgSize=params[0]["Line4MsgSize"].toInt();
    if(params[0]["Line4MsgEffect"]!="")
        this->Line4MsgEffect=params[0]["Line4MsgEffect"];
    if(params[0]["Line4MsgStyle"]!="")
        this->Line4MsgStyle=params[0]["Line4MsgStyle"];
    if(params[0]["Line4MsgWeight"]!="")
        this->Line4MsgWeight=params[0]["Line4MsgWeight"];
}
int LineMessages::getLine1MsgX()
{
    return this->Line1MsgX;
}
void LineMessages::setLine1MsgX(int value)
{
    this->Line1MsgX = value;
}
int LineMessages::getLine1MsgY()
{
    return this->Line1MsgY;
}
void LineMessages::setLine1MsgY(int value)
{
    this->Line1MsgY = value;
}
QString LineMessages::getLine1MsgForecolor()
{
    return this->Line1MsgForecolor;
}
void LineMessages::setLine1MsgForecolor(QString value)
{
    this->Line1MsgForecolor = value;
}
QString LineMessages::getLine1MsgColor()
{
    return this->Line1MsgColor;
}
void LineMessages::setLine1MsgColor(QString value)
{
    this->Line1MsgColor = value;
}
int LineMessages::getLine1MsgSize()
{
    return this->Line1MsgSize;
}
QString LineMessages::getLine1MsgEffect()
{
    return this->Line1MsgEffect;
}
void LineMessages::setLine1MsgEffect(QString value)
{
    this->Line1MsgEffect = value;
}
QString LineMessages::getLine1MsgStyle()
{
    return this->Line1MsgStyle;
}
void LineMessages::setLine1MsgStyle(QString value)
{
    this->Line1MsgStyle = value;
}
 QString LineMessages::getLine1MsgWeight()
 {
    return this->Line1MsgWeight;
 }
 void LineMessages::setLine1MsgWeight(QString value)
 {
    this->Line1MsgWeight = value;
 }
 //
 int LineMessages::getLine2MsgX()
 {
     return this->Line2MsgX;
 }
 void LineMessages::setLine2MsgX(int value)
 {
     this->Line2MsgX = value;
 }
 int LineMessages::getLine2MsgY()
 {
     return this->Line2MsgY;
 }
 void LineMessages::setLine2MsgY(int value)
 {
     this->Line2MsgY = value;
 }
 QString LineMessages::getLine2MsgForecolor()
 {
     return this->Line2MsgForecolor;
 }
 void LineMessages::setLine2MsgForecolor(QString value)
 {
     this->Line2MsgForecolor = value;
 }
 QString LineMessages::getLine2MsgColor()
 {
     return this->Line2MsgColor;
 }
 void LineMessages::setLine2MsgColor(QString value)
 {
     this->Line2MsgColor = value;
 }
 int LineMessages::getLine2MsgSize()
 {
     return this->Line2MsgSize;
 }
 QString LineMessages::getLine2MsgEffect()
 {
     return this->Line2MsgEffect;
 }
 void LineMessages::setLine2MsgEffect(QString value)
 {
     this->Line2MsgEffect = value;
 }
 QString LineMessages::getLine2MsgStyle()
 {
     return this->Line2MsgStyle;
 }
 void LineMessages::setLine2MsgStyle(QString value)
 {
     this->Line2MsgStyle = value;
 }
 QString LineMessages::getLine2MsgWeight()
 {
    return this->Line2MsgWeight;
 }
 void LineMessages::setLine2MsgWeight(QString value)
 {
    this->Line2MsgWeight = value;
 }
//
 int LineMessages::getLine3MsgX()
 {
     return this->Line3MsgX;
 }
 void LineMessages::setLine3MsgX(int value)
 {
     this->Line3MsgX = value;
 }
 int LineMessages::getLine3MsgY()
 {
     return this->Line3MsgY;
 }
 void LineMessages::setLine3MsgY(int value)
 {
     this->Line3MsgY = value;
 }
 QString LineMessages::getLine3MsgForecolor()
 {
     return this->Line3MsgForecolor;
 }
 void LineMessages::setLine3MsgForecolor(QString value)
 {
     this->Line3MsgForecolor = value;
 }
 QString LineMessages::getLine3MsgColor()
 {
     return this->Line3MsgColor;
 }
 void LineMessages::setLine3MsgColor(QString value)
 {
     this->Line3MsgColor = value;
 }
 int LineMessages::getLine3MsgSize()
 {
     return this->Line3MsgSize;
 }
 QString LineMessages::getLine3MsgEffect()
 {
     return this->Line3MsgEffect;
 }
 void LineMessages::setLine3MsgEffect(QString value)
 {
     this->Line3MsgEffect = value;
 }
 QString LineMessages::getLine3MsgStyle()
 {
     return this->Line3MsgStyle;
 }
 void LineMessages::setLine3MsgStyle(QString value)
 {
     this->Line3MsgStyle = value;
 }
 QString LineMessages::getLine3MsgWeight()
 {
     return this->Line3MsgWeight;
 }
 void LineMessages::setLine3MsgWeight(QString value)
 {
    this->Line3MsgWeight = value;
 }
//
 int LineMessages::getLine4MsgX()
 {
     return this->Line4MsgX;
 }
 void LineMessages::setLine4MsgX(int value)
 {
     this->Line4MsgX = value;
 }
 int LineMessages::getLine4MsgY()
 {
     return this->Line4MsgY;
 }
 void LineMessages::setLine4MsgY(int value)
 {
     this->Line4MsgY = value;
 }
 QString LineMessages::getLine4MsgForecolor()
 {
     return this->Line4MsgForecolor;
 }
 void LineMessages::setLine4MsgForecolor(QString value)
 {
     this->Line4MsgForecolor = value;
 }
 QString LineMessages::getLine4MsgColor()
 {
     return this->Line4MsgColor;
 }
 void LineMessages::setLine4MsgColor(QString value)
 {
     this->Line4MsgColor = value;
 }
 int LineMessages::getLine4MsgSize()
 {
     return this->Line4MsgSize;
 }
 QString LineMessages::getLine4MsgEffect()
 {
     return this->Line4MsgEffect;
 }
 void LineMessages::setLine4MsgEffect(QString value)
 {
     this->Line4MsgEffect = value;
 }
 QString LineMessages::getLine4MsgStyle()
 {
     return this->Line4MsgStyle;
 }
 void LineMessages::setLine4MsgStyle(QString value)
 {
     this->Line4MsgStyle = value;
 }
  QString LineMessages::getLine4MsgWeight()
  {
     return this->Line4MsgWeight;
  }
  void LineMessages::setLine4MsgWeight(QString value)
  {
     this->Line4MsgWeight = value;
  }

void LineMessages::parseXML() {
    /* We'll parse the example.xml */
    QFile* file = new QFile(PATH+"/"+LINEMSG_FILE);
    /* If we can't open it, let's show an error message. */
    if (!file->open(QIODevice::ReadOnly | QIODevice::Text)) {
        printf("No se pudo abrir linemsg.xml\r\\n");
        writeConfigDefault(PATH+"/"+LINEMSG_FILE);
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
            if(xml.name() == "LineMsg") {
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
QMap<QString, QString> LineMessages::parseMessage(QXmlStreamReader& xml) {
    QMap<QString, QString> LineMsg;
    /* Let's check that we're really getting a person. */
    if(xml.tokenType() != QXmlStreamReader::StartElement &&
       xml.name() == "LineMsg") {
        return LineMsg;
    }
    /* Next element... */
    xml.readNext();
    /*
     * We're going to loop over the things because the order might change.
     * We'll continue the loop until we hit an EndElement named person.
     */
    while(!(xml.tokenType() == QXmlStreamReader::EndElement &&
            xml.name() == "LineMsg")) {
        if(xml.tokenType() == QXmlStreamReader::StartElement) {
            /* We've found first name.

            */
            if(xml.name() == "Line1MsgX") {
                this->addElementDataToMap(xml, LineMsg);
            }
            if(xml.name() == "Line1MsgY") {
                this->addElementDataToMap(xml, LineMsg);
            }
            if(xml.name() == "Line1MsgForecolor") {
                this->addElementDataToMap(xml, LineMsg);
            }
            if(xml.name() == "Line1MsgColor") {
                this->addElementDataToMap(xml, LineMsg);
            }
            if(xml.name() == "Line1MsgSize") {
                this->addElementDataToMap(xml, LineMsg);
            }
            if(xml.name() == "Line1MsgEffect") {
                this->addElementDataToMap(xml, LineMsg);
            }
            if(xml.name() == "Line1MsgStyle") {
                this->addElementDataToMap(xml, LineMsg);
            }
            if(xml.name() == "Line1MsgWeight") {
                this->addElementDataToMap(xml, LineMsg);
            }
            //
            if(xml.name() == "Line2MsgX") {
                this->addElementDataToMap(xml, LineMsg);
            }
            if(xml.name() == "Line2MsgY") {
                this->addElementDataToMap(xml, LineMsg);
            }
            if(xml.name() == "Line2MsgForecolor") {
                this->addElementDataToMap(xml, LineMsg);
            }
            if(xml.name() == "Line2MsgColor") {
                this->addElementDataToMap(xml, LineMsg);
            }
            if(xml.name() == "Line2MsgSize") {
                this->addElementDataToMap(xml, LineMsg);
            }
            if(xml.name() == "Line2MsgEffect") {
                this->addElementDataToMap(xml, LineMsg);
            }
            if(xml.name() == "Line2MsgStyle") {
                this->addElementDataToMap(xml, LineMsg);
            }
            if(xml.name() == "Line2MsgWeight") {
                this->addElementDataToMap(xml, LineMsg);
            }
            //
            if(xml.name() == "Line3MsgX") {
                this->addElementDataToMap(xml, LineMsg);
            }
            if(xml.name() == "Line3MsgY") {
                this->addElementDataToMap(xml, LineMsg);
            }
            if(xml.name() == "Line3MsgForecolor") {
                this->addElementDataToMap(xml, LineMsg);
            }
            if(xml.name() == "Line3MsgColor") {
                this->addElementDataToMap(xml, LineMsg);
            }
            if(xml.name() == "Line3MsgSize") {
                this->addElementDataToMap(xml, LineMsg);
            }
            if(xml.name() == "Line3MsgEffect") {
                this->addElementDataToMap(xml, LineMsg);
            }
            if(xml.name() == "Line3MsgStyle") {
                this->addElementDataToMap(xml, LineMsg);
            }
            if(xml.name() == "Line3MsgWeight") {
                this->addElementDataToMap(xml, LineMsg);
            }
            //
            if(xml.name() == "Line4MsgX") {
                this->addElementDataToMap(xml, LineMsg);
            }
            if(xml.name() == "Line4MsgY") {
                this->addElementDataToMap(xml, LineMsg);
            }
            if(xml.name() == "Line4MsgForecolor") {
                this->addElementDataToMap(xml, LineMsg);
            }
            if(xml.name() == "Line4MsgColor") {
                this->addElementDataToMap(xml, LineMsg);
            }
            if(xml.name() == "Line4MsgSize") {
                this->addElementDataToMap(xml, LineMsg);
            }
            if(xml.name() == "Line4MsgEffect") {
                this->addElementDataToMap(xml, LineMsg);
            }
            if(xml.name() == "Line4MsgStyle") {
                this->addElementDataToMap(xml, LineMsg);
            }
            if(xml.name() == "Line4MsgWeight") {
                this->addElementDataToMap(xml, LineMsg);
            }
        }
        /* ...and next... */
        xml.readNext();
    }
    return LineMsg;
}

void LineMessages::addElementDataToMap(QXmlStreamReader& xml,
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
void LineMessages::writeConfigDefault(QString filename)
{
    QFile file(filename);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    QList<QString> list;
    list.append("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n");
    list.append("<LineMsg>\n");
    list.append("<Line1MsgX>50</Line1MsgX>\n");
    list.append("<Line1MsgY>50</Line1MsgY>\n");
    list.append("<Line1MsgForecolor>53E61E</Line1MsgForecolor>\n");
    list.append("<Line1MsgColor>F51E0F</Line1MsgColor>\n");
    list.append("<Line1MsgSize>40</Line1MsgSize>\n");
    list.append("<Line1MsgEffect>Fixed</Line1MsgEffect>\n");
    list.append("<Line1MsgStyle>Normal</Line1MsgStyle>\n");
    list.append("<Line1MsgWeight>Normal</Line1MsgWeight>\n");
    //
    list.append("<Line2MsgX>50</Line2MsgX>\n");
    list.append("<Line2MsgY>150</Line2MsgY>\n");
    list.append("<Line2MsgForecolor>53E61E</Line2MsgForecolor>\n");
    list.append("<Line2MsgColor>4E53E6</Line2MsgColor>\n");
    list.append("<Line2MsgSize>40</Line2MsgSize>\n");
    list.append("<Line2MsgEffect>Fixed</Line2MsgEffect>\n");
    list.append("<Line2MsgStyle>Normal</Line2MsgStyle>\n");
    list.append("<Line2MsgWeight>Bold</Line2MsgWeight>\n");
    //
    list.append("<Line3MsgX>50</Line3MsgX>\n");
    list.append("<Line3MsgY>250</Line3MsgY>\n");
    list.append("<Line3MsgForecolor>53E61E</Line3MsgForecolor>\n");
    list.append("<Line3MsgColor>F51E0F</Line3MsgColor>\n");
    list.append("<Line3MsgSize>40</Line3MsgSize>\n");
    list.append("<Line3MsgEffect>Fixed</Line3MsgEffect>\n");
    list.append("<Line3MsgStyle>Normal</Line3MsgStyle>\n");
    list.append("<Line3MsgWeight>Normal</Line3MsgWeight>\n");
    //
    list.append("<Line4MsgX>50</Line4MsgX>\n");
    list.append("<Line4MsgY>350</Line4MsgY>\n");
    list.append("<Line4MsgForecolor>53E61E</Line4MsgForecolor>\n");
    list.append("<Line4MsgColor>4E53E6</Line4MsgColor>\n");
    list.append("<Line4MsgSize>40</Line4MsgSize>\n");
    list.append("<Line4MsgEffect>Fixed</Line4MsgEffect>\n");
    list.append("<Line4MsgStyle>Normal</Line4MsgStyle>\n");
    list.append("<Line4MsgWeight>Bold</Line4MsgWeight>\n");
    list.append("</LineMsg>\n");
    QString result = "";
    QString s;
    foreach( s, list )
            result +=s;
    out << result;
    file.close();

}

