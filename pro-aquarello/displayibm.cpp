#include "displayibm.h"

DisplayIBM::DisplayIBM(QObject *parent) :
    QObject(parent)
{

}
//void DisplayIBM::strData(QString str)
//{
//	strdata = str;
//}
void DisplayIBM::setBackImgPath(QString path)
{
    backImgPath = path;
}
void DisplayIBM::setForeImgPath(QString path)
{
    foreImgPath = path;
}
void DisplayIBM::setDataAccessFault(QString message)
{
    errMessage = message;
}

void DisplayIBM::createDisplayPage(QString htmlpage)
{
    //QString PATH =QCoreApplication::applicationDirPath();
    QFile file(htmlpage);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    out << headScriptHTML();
    QList<QString> list;
    list << "<BODY>\n";
    list.append("<div id='page-background'>\n");
    if(backImgPath.count() > 0){
        list.append(QString("<img src='%1' width='100\%' height='100\%'>\n").arg(backImgPath));
    }
    list.append("</div>\n");
    list.append("<div id=\"content\">\n");


    lm = new LineMessages();

    if(errMessage.count() > 0 || (this->ItemName.count() <=0 && this->ItemCode.count() <= 0 )){
        QString err = "";
        int posi,posf;
        if((posi=errMessage.indexOf("DataAccessFault=")) > -1){
            if((posf=errMessage.indexOf("|",posi+16)) > -1){
                 err = errMessage.mid(posi+16,posf-posi-16);
                 if(QString::compare(err,"Requested Keys Not Available!") == 0){
                     err = "Article inconnu";
                 }
            }
        }else{
            err = "Article inconnu";
        }
        //qDebug() << err;
        //Line1
        if (QString::compare(lm->getLine1MsgEffect(),"Fixed",Qt::CaseInsensitive) == 0){
            list.append(QString("<SPAN style='position:absolute;left:%1px;top:%2px;").arg(lm->getLine1MsgX()).arg(lm->getLine1MsgY()));
        }else if (QString::compare(lm->getLine1MsgEffect(),"Blink",Qt::CaseInsensitive) == 0){
            list.append(QString("<blink style='position:absolute;left:%1px;top:%2px;").arg(lm->getLine1MsgX()).arg(lm->getLine1MsgY()));
        }else if (QString::compare(lm->getLine1MsgEffect(),"Scroll",Qt::CaseInsensitive) == 0){
            list.append(QString("<MARQUEE style='position:absolute;left:%1px;top:%2px;").arg(lm->getLine1MsgX()).arg(lm->getLine1MsgY()));
        }else{
            list.append(QString("<SPAN style='position:absolute;left:%1px;top:%2px;").arg(lm->getLine1MsgX()).arg(lm->getLine1MsgY()));
        }

        if (QString::compare(lm->getLine1MsgForecolor(),"ffffff",Qt::CaseInsensitive) == 0){
            list.append("background-color:transparent;");
        }else{
            list.append(QString("background-color:#%1;").arg(lm->getLine1MsgForecolor()));
            list.append("border-radius:15px;padding:5px;");
        }
        list.append(QString("color:#%1;").arg(lm->getLine1MsgColor()));
        list.append(QString("font-size:%1pt;").arg(lm->getLine1MsgSize()));
        list.append("font-family:arial;");
        list.append(QString("font-style:%1;").arg(lm->getLine1MsgStyle()));
        list.append(QString("font-weight:%1;").arg(lm->getLine1MsgWeight()));

        if (QString::compare(lm->getLine1MsgEffect(),"Fixed",Qt::CaseInsensitive) == 0){
            list.append("'>\n"+err+"\n</SPAN>\n");
        }else if (QString::compare(lm->getLine1MsgEffect(),"Blink",Qt::CaseInsensitive) == 0){
            list.append("'>\n"+err+"\n</blink>\n");
        }else if (QString::compare(lm->getLine1MsgEffect(),"Scroll",Qt::CaseInsensitive) == 0){
            list.append("'>\n"+err+"\n</MARQUEE>\n");
        }else{
            list.append("'>\n"+err+"\n</SPAN>\n");
        }


    }else{
    //Line 1
        if (QString::compare(lm->getLine1MsgEffect(),"Fixed",Qt::CaseInsensitive) == 0){
            list.append(QString("<SPAN style='position:absolute;left:%1px;top:%2px;").arg(lm->getLine1MsgX()).arg(lm->getLine1MsgY()));
        }else if (QString::compare(lm->getLine1MsgEffect(),"Blink",Qt::CaseInsensitive) == 0){
            list.append(QString("<blink style='position:absolute;left:%1px;top:%2px;").arg(lm->getLine1MsgX()).arg(lm->getLine1MsgY()));
        }else if (QString::compare(lm->getLine1MsgEffect(),"Scroll",Qt::CaseInsensitive) == 0){
            list.append(QString("<MARQUEE style='position:absolute;left:%1px;top:%2px;").arg(lm->getLine1MsgX()).arg(lm->getLine1MsgY()));
        }else{
            list.append(QString("<SPAN style='position:absolute;left:%1px;top:%2px;").arg(lm->getLine1MsgX()).arg(lm->getLine1MsgY()));
        }

        if (QString::compare(lm->getLine1MsgForecolor(),"ffffff",Qt::CaseInsensitive) == 0){
            list.append("background-color:transparent;");
        }else{
            list.append(QString("background-color:#%1;").arg(lm->getLine1MsgForecolor()));
            list.append("border-radius:15px;padding:5px;");
        }
        list.append(QString("color:#%1;").arg(lm->getLine1MsgColor()));
        list.append(QString("font-size:%1pt;").arg(lm->getLine1MsgSize()));
        list.append("font-family:arial;");
        list.append(QString("font-style:%1;").arg(lm->getLine1MsgStyle()));
        list.append(QString("font-weight:%1;").arg(lm->getLine1MsgWeight()));

        if (QString::compare(lm->getLine1MsgEffect(),"Fixed",Qt::CaseInsensitive) == 0){
            list.append("'>\n"+this->ItemName+"\n</SPAN>\n");
        }else if (QString::compare(lm->getLine1MsgEffect(),"Blink",Qt::CaseInsensitive) == 0){
            list.append("'>\n"+this->ItemName+"\n</blink>\n");
        }else if (QString::compare(lm->getLine1MsgEffect(),"Scroll",Qt::CaseInsensitive) == 0){
            list.append("'>\n"+this->ItemName+"\n</MARQUEE>\n");
        }else{
            list.append("'>\n"+this->ItemName+"\n</SPAN>\n");
        }
        //Line 2
        if (QString::compare(lm->getLine2MsgEffect(),"Fixed",Qt::CaseInsensitive) == 0){
            list.append(QString("<SPAN style='position:absolute;left:%1px;top:%2px;").arg(lm->getLine2MsgX()).arg(lm->getLine2MsgY()));
        }else if (QString::compare(lm->getLine2MsgEffect(),"Blink",Qt::CaseInsensitive) == 0){
            list.append(QString("<blink style='position:absolute;left:%1px;top:%2px;").arg(lm->getLine2MsgX()).arg(lm->getLine2MsgY()));
        }else if (QString::compare(lm->getLine2MsgEffect(),"Scroll",Qt::CaseInsensitive) == 0){
            list.append(QString("<MARQUEE style='position:absolute;left:%1px;top:%2px;").arg(lm->getLine2MsgX()).arg(lm->getLine2MsgY()));
        }else{
            list.append(QString("<SPAN style='position:absolute;left:%1px;top:%2px;").arg(lm->getLine2MsgX()).arg(lm->getLine2MsgY()));
        }

        if (QString::compare(lm->getLine2MsgForecolor(),"ffffff",Qt::CaseInsensitive) == 0){
            list.append("background-color:transparent;");
        }else{
            list.append(QString("background-color:#%1;").arg(lm->getLine2MsgForecolor()));
            list.append("border-radius:15px;padding:5px;");
        }
        list.append(QString("color:#%1;").arg(lm->getLine2MsgColor()));
        list.append(QString("font-size:%1pt;").arg(lm->getLine2MsgSize()));
        list.append("font-family:arial;");
        list.append(QString("font-style:%1;").arg(lm->getLine2MsgStyle()));
        list.append(QString("font-weight:%1;").arg(lm->getLine2MsgWeight()));

        float fprice = (float)this->SalePrice.toInt()/100;
        QString sprice = QString::number(fprice,'f',2);
        sprice = sprice.replace(".",",")+" "+QString("&#8364");

        if (QString::compare(lm->getLine2MsgEffect(),"Fixed",Qt::CaseInsensitive) == 0){
            list.append("'>\n"+sprice+"\n</SPAN>\n");
        }else if (QString::compare(lm->getLine2MsgEffect(),"Blink",Qt::CaseInsensitive) == 0){
            list.append("'>\n"+sprice+"\n</blink>\n");
        }else if (QString::compare(lm->getLine2MsgEffect(),"Scroll",Qt::CaseInsensitive) == 0){
            list.append("'>\n"+sprice+"\n</MARQUEE>\n");
        }else{
            list.append("'>\n"+sprice+"\n</SPAN>\n");
        }

        if(this->Discount.count() > 0){
            //Line 3
            if (QString::compare(lm->getLine3MsgEffect(),"Fixed",Qt::CaseInsensitive) == 0){
                list.append(QString("<SPAN style='position:absolute;left:%1px;top:%2px;").arg(lm->getLine3MsgX()).arg(lm->getLine3MsgY()));
            }else if (QString::compare(lm->getLine3MsgEffect(),"Blink",Qt::CaseInsensitive) == 0){
                list.append(QString("<blink style='position:absolute;left:%1px;top:%2px;").arg(lm->getLine3MsgX()).arg(lm->getLine3MsgY()));
            }else if (QString::compare(lm->getLine3MsgEffect(),"Scroll",Qt::CaseInsensitive) == 0){
                list.append(QString("<MARQUEE style='position:absolute;left:%1px;top:%2px;").arg(lm->getLine3MsgX()).arg(lm->getLine3MsgY()));
            }else{
                list.append(QString("<SPAN style='position:absolute;left:%1px;top:%2px;").arg(lm->getLine3MsgX()).arg(lm->getLine3MsgY()));
            }

            if (QString::compare(lm->getLine3MsgForecolor(),"ffffff",Qt::CaseInsensitive) == 0){
                list.append("background-color:transparent;");
            }else{
                list.append(QString("background-color:#%1;").arg(lm->getLine3MsgForecolor()));
                list.append("border-radius:15px;padding:5px;");
            }
            list.append(QString("color:#%1;").arg(lm->getLine3MsgColor()));
            list.append(QString("font-size:%1pt;").arg(lm->getLine3MsgSize()));
            list.append("font-family:arial;");
            list.append(QString("font-style:%1;").arg(lm->getLine3MsgStyle()));
            list.append(QString("font-weight:%1;").arg(lm->getLine3MsgWeight()));

            if (QString::compare(lm->getLine3MsgEffect(),"Fixed",Qt::CaseInsensitive) == 0){
                list.append("'>\n"+QString("PROMO")+"\n</SPAN>\n");
            }else if (QString::compare(lm->getLine3MsgEffect(),"Blink",Qt::CaseInsensitive) == 0){
                list.append("'>\n"+QString("PROMO")+"\n</blink>\n");
            }else if (QString::compare(lm->getLine3MsgEffect(),"Scroll",Qt::CaseInsensitive) == 0){
                list.append("'>\n"+QString("PROMO")+"\n</MARQUEE>\n");
            }else{
                list.append("'>\n"+QString("PROMO")+"\n</SPAN>\n");
            }

            //Line 4
            if (QString::compare(lm->getLine4MsgEffect(),"Fixed",Qt::CaseInsensitive) == 0){
                list.append(QString("<SPAN style='position:absolute;left:%1px;top:%2px;").arg(lm->getLine4MsgX()).arg(lm->getLine4MsgY()));
            }else if (QString::compare(lm->getLine4MsgEffect(),"Blink",Qt::CaseInsensitive) == 0){
                list.append(QString("<blink style='position:absolute;left:%1px;top:%2px;").arg(lm->getLine4MsgX()).arg(lm->getLine4MsgY()));
            }else if (QString::compare(lm->getLine4MsgEffect(),"Scroll",Qt::CaseInsensitive) == 0){
                list.append(QString("<MARQUEE style='position:absolute;left:%1px;top:%2px;").arg(lm->getLine4MsgX()).arg(lm->getLine4MsgY()));
            }else{
                list.append(QString("<SPAN style='position:absolute;left:%1px;top:%2px;").arg(lm->getLine4MsgX()).arg(lm->getLine4MsgY()));
            }

            if (QString::compare(lm->getLine4MsgForecolor(),"ffffff",Qt::CaseInsensitive) == 0){
                list.append("background-color:transparent;");
            }else{
                list.append(QString("background-color:#%1;").arg(lm->getLine4MsgForecolor()));
                list.append("border-radius:15px;padding:5px;");
            }
            list.append(QString("color:#%1;").arg(lm->getLine4MsgColor()));
            list.append(QString("font-size:%1pt;").arg(lm->getLine4MsgSize()));
            list.append("font-family:arial;");
            list.append(QString("font-style:%1;").arg(lm->getLine4MsgStyle()));
            list.append(QString("font-weight:%1;").arg(lm->getLine4MsgWeight()));

            float fprice = (float)this->Discount.toInt()/100;
            QString sprice = QString::number(fprice,'f',2);
            sprice = sprice.replace(".",",")+" "+QString("&#8364");

            if (QString::compare(lm->getLine4MsgEffect(),"Fixed",Qt::CaseInsensitive) == 0){
                list.append("'>\n"+sprice+"\n</SPAN>\n");
            }else if (QString::compare(lm->getLine4MsgEffect(),"Blink",Qt::CaseInsensitive) == 0){
                list.append("'>\n"+sprice+"\n</blink>\n");
            }else if (QString::compare(lm->getLine4MsgEffect(),"Scroll",Qt::CaseInsensitive) == 0){
                list.append("'>\n"+sprice+"\n</MARQUEE>\n");
            }else{
                list.append("'>\n"+sprice+"\n</SPAN>\n");
            }
        }
    }

    list.append("</div>\n");
    list.append("</BODY>\n");
    list.append("</HTML>\n");
    QString result = "";
    QString s;
    foreach( s, list )
        result +=s;
    out << result;
    file.close();
	
}
QString DisplayIBM::backgroundColor(QString color)
{
	if (QString::compare(color,"0",Qt::CaseInsensitive) == 0) return "background-color:transparent;";
        else if (QString::compare(color,"5",Qt::CaseInsensitive) == 0) return "background-color:yellow;";
        else if (QString::compare(color,"35",Qt::CaseInsensitive) == 0) return "background-color:red;";
        else if (QString::compare(color,"185",Qt::CaseInsensitive) == 0) return "background-color:green;";
        else if (QString::compare(color,"206",Qt::CaseInsensitive) == 0) return "background-color:blue;";
        else if (QString::compare(color,"215",Qt::CaseInsensitive) == 0) return "background-color:black;";
        else return "background-color:red;";
}
QString DisplayIBM::foregroundColor(QString color)
{
        if (QString::compare(color,"0",Qt::CaseInsensitive) == 0) return "color:transparent;";
        else if (QString::compare(color,"5",Qt::CaseInsensitive) == 0) return "color:yellow;";
        else if (QString::compare(color,"35",Qt::CaseInsensitive) == 0) return "color:red;";
        else if (QString::compare(color,"185",Qt::CaseInsensitive) == 0) return "color:green;";
        else if (QString::compare(color,"206",Qt::CaseInsensitive) == 0) return "color:blue;";
        else if (QString::compare(color,"215",Qt::CaseInsensitive) == 0) return "color:black;";
        else return "color:red;";
}
QString DisplayIBM::styleFont(QString style)
{
	if (QString::compare(style,"0",Qt::CaseInsensitive) == 0) return "font-style:normal;";
	else if(QString::compare(style,"1",Qt::CaseInsensitive) == 0) return "font-style:italic;";
	else if(QString::compare(style,"2",Qt::CaseInsensitive) == 0) return "font-weight:bold;";
	else return "font-style:normal;";
}
void DisplayIBM::createWelcomePage(QString welcomepage)
{
    msg=new WelcomeConfiguration();
    //QString PATH =QCoreApplication::applicationDirPath();
    QFile file(welcomepage);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    out << headScriptHTML();
    QList<QString> list;
    list << "<BODY>\n";
    list.append("<div id='page-background'>\n");
    if(foreImgPath.count() > 0){
        list.append(QString("<img src='%1' width='100\%' height='100\%'>\n").arg(foreImgPath));
    }
    list.append("</div>\n");
    list.append("<div id=\"content\">\n");

    if(QString::compare(msg->getMsgVisible(),"true",Qt::CaseInsensitive) == 0){
        QString effect = msg->getMsgEffect();
        if(QString::compare(effect,"Normal",Qt::CaseInsensitive) == 0){
                list.append(QString("<SPAN style='position:absolute;left:%1px;top:%2px;").arg(msg->getMsgX()).arg(msg->getMsgY()));
        }else if(QString::compare(effect,"Blink",Qt::CaseInsensitive) == 0){
                list.append(QString("<blink style='position:absolute;left:%1px;top:%2px;").arg(msg->getMsgX()).arg(msg->getMsgY()));
        }else if(QString::compare(effect,"Scroll",Qt::CaseInsensitive) == 0){
                list.append(QString("<MARQUEE style='position:absolute;left:%1px;top:%2px;").arg(msg->getMsgX()).arg(msg->getMsgY()));
        }else{
                list.append(QString("<SPAN style='position:absolute;left:%1px;top:%2px;").arg(msg->getMsgX()).arg(msg->getMsgY()));
        }
        if(QString::compare(msg->getMsgForeColor(),"ffffff",Qt::CaseInsensitive) == 0){
                list.append("background-color:transparent;");
        }else{
                list.append(QString("background-color:#%1;").arg(msg->getMsgForeColor()));
        }
        list.append(QString("color:#%1;").arg(msg->getMsgColor()));
        list.append(QString("font-size:%1pt;").arg(msg->getMsgSize()));
        list.append("font-family:arial;");
        list.append(QString("font-style:%1;").arg(msg->getMsgStyle()));
        list.append(QString("font-weight:%1;").arg(msg->getMsgWeight()));

        if(QString::compare(effect,"Normal",Qt::CaseInsensitive) == 0){
                list.append(QString("'>\n%1\n</SPAN>\n").arg(msg->getMsgTxt()));
        }else if(QString::compare(effect,"Blink",Qt::CaseInsensitive) == 0){
                list.append(QString("'>\n%1\n</blink>\n").arg(msg->getMsgTxt()));
        }else if(QString::compare(effect,"Scroll",Qt::CaseInsensitive) == 0){
                list.append(QString("'>\n%1\n</MARQUEE>\n").arg(msg->getMsgTxt()));
        }else{
                list.append(QString("'>\n%1\n</SPAN>\n").arg(msg->getMsgTxt()));
        }
    }
    list.append("</div>\n");
    list.append("</BODY>\n");
    list.append("</HTML>\n");

    QString result = "";
    QString s;
    foreach( s, list )
            result +=s;
    out << result;
    file.close();

}
QString DisplayIBM::headScriptHTML()
{
	QList<QString> list;
	QString result = "";
	list << "<HTML>\n";
        list.append("<HEAD>\n");
        list.append("<SCRIPT type=\"text/javascript\">\n");
        list.append("<!--\n");
        list.append("speed=750;\n");
        list.append("blink=document.all.tags(\"blink\");\n");
        list.append("swi=1;\n");
        list.append("bringBackBlinky();\n");
        list.append("function bringBackBlinky() {\n");
        list.append("if (swi == 1) {\n");
        list.append("sho=\"visible\";\n");
        list.append("swi=0;\n");
        list.append("}\n");
        list.append("else {\n");
        list.append("sho=\"hidden\";\n");
        list.append("swi=1;\n");
        list.append("}\n");
        list.append("for(i=0;i<blink.length;i++) {\n");
        list.append("blink[i].style.visibility=sho;\n");
        list.append("}\n");
        list.append("setTimeout(\"bringBackBlinky()\", speed);\n");
        list.append("}\n");
        list.append("// -->\n");
        list.append("</SCRIPT>\n");
        list.append("<style type=\"text/css\">\n");
        list.append("body {height:100%; margin:0; padding:0;}\n");
        list.append("#page-background {position:fixed; top:0; left:0; width:100%; height:100%;}\n");
        list.append("#content {position:relative; z-index:1;padding:0px;}\n");
        list.append("</style>\n");
        list.append("</HEAD>\n");

	QString s;
	foreach( s, list )
		result +=s;        
	return result;
}
void DisplayIBM::parseXML(QString data) {
    /* We'll parse the example.xml */
    /* QXmlStreamReader takes any QString */
    QXmlStreamReader xml(data);

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
            if(xml.name() == "NormalSaleItem") {
                this->parseMessage(xml);
            }
        }
    }
    /* Error handling. */
    if(xml.hasError()) {
        printf("Imposible parse config.xml\r\\n");
    }
    xml.clear();
}
void DisplayIBM::parseMessage(QXmlStreamReader& xml) {

    /* Let's check that we're really getting a person. */
    if(xml.tokenType() != QXmlStreamReader::StartElement &&
       xml.name() == "NormalSaleItem") {
        return;
    }
    /* Next element... */
    xml.readNext();
    /*
     * We're going to loop over the things because the order might change.
     * We'll continue the loop until we hit an EndElement named person.
     */
    while(!(xml.tokenType() == QXmlStreamReader::EndElement &&
            xml.name() == "NormalSaleItem")) {
        if(xml.tokenType() == QXmlStreamReader::StartElement) {
            /* We've found first name.

            */
            if(xml.name() == "ItemCode") {
                if(xml.tokenType() == QXmlStreamReader::StartElement) {
                    xml.readNext();
                    if(xml.tokenType() == QXmlStreamReader::Characters) {
                        this->ItemCode = xml.text().toString();
                    }
                }
            }
            if(xml.name() == "SalePrice") {
                if(xml.tokenType() == QXmlStreamReader::StartElement) {
                    xml.readNext();
                    if(xml.tokenType() == QXmlStreamReader::Characters) {
                        this->SalePrice = xml.text().toString();
                    }
                }
            }
            if(xml.name() == "ItemName") {
                if(xml.tokenType() == QXmlStreamReader::StartElement) {
                    xml.readNext();
                    if(xml.tokenType() == QXmlStreamReader::Characters) {
                        this->ItemName = xml.text().toString();
                    }
                }
            }
            if(xml.name() == "RestrictedSaleType") {
                if(xml.tokenType() == QXmlStreamReader::StartElement) {
                    xml.readNext();
                    if(xml.tokenType() == QXmlStreamReader::Characters) {
                        this->RestrictedSaleType = xml.text().toString();
                    }
                }
            }
            if(xml.name() == "Discount") {
                if(xml.tokenType() == QXmlStreamReader::StartElement) {
                    xml.readNext();
                    if(xml.tokenType() == QXmlStreamReader::Characters) {
                        this->Discount = xml.text().toString();
                    }
                }
            }

        }
        /* ...and next... */
        xml.readNext();
    }

}

