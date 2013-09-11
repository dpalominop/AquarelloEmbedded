#include "displaygenio.h"

DisplayGenio::DisplayGenio(QObject *parent) :
    QObject(parent)
{

}
void DisplayGenio::byteData(QByteArray data)
{
        Data = data;
        //qDebug() << Data;
}
void DisplayGenio::setBackImgPath(QString path)
{
    backImgPath = path;
}
void DisplayGenio::setForeImgPath(QString path)
{
    foreImgPath = path;
}
void DisplayGenio::createDisplayPage(QString htmlpage)
{
        //QString PATH =QCoreApplication::applicationDirPath();
        QFile file(htmlpage);
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream out(&file);
	//out.setCodec("UTF-8");
        out << headScriptHTML();
        QList<QString> list;
        list << "<BODY>\n";
        list.append("<div id='page-background'>\n");
        if(backImgPath.count() > 0){
            list.append(QString("<img src='%1' width='100\%' height='100\%'>\n").arg(backImgPath));
        }
        list.append("</div>\n");
        list.append("<div id=\"content\">\n");
        int x,y,line,row;
        int i=0;
        int count=1;
        bool status=1;

        int slen=Data.size();

        lm = new LineMessages();
        while(status)
        {
            //qDebug() << "SWITCH="+QString("%1").arg(Data.data()[i]);
            if(count > 4)
                break;
            switch(Data.data()[i]){
                case 0x1A:
                    i+=1;
                    break;
                case 0x1B:
                    //qDebug() << "0x1B";
                    if((Data.data()[i+1] == 0x50) || (Data.data()[i+1] == 0x55)){
                        line = (Data.data()[i+4]-0x30) & 0xFF;
                        row = ((Data.data()[i+5]-0x30) & 0xFF)*3;
                        i+=6;
                        i+=line*row;
                        if(i>=slen){
                            status = false;
                        }
                    }else{
                        y = (Data.data()[i+2]-0x30) & 0xFF;
                        x = (Data.data()[i+3]-0x30) & 0xFF;
                        line = (Data.data()[i+4]-0x30) & 0xFF;
                        row = (Data.data()[i+5]-0x30) & 0xFF;
                        //qDebug() << "ROW = "+QString("%1").arg(row);
                        i+=6;
                        QByteArray info = Data.mid(i,row);
                        int pos;
                        if((pos=info.indexOf((char)0xa4)) > 0){
                            //qDebug() << "POS="+QString("%1").arg(pos);
                            info.replace(pos,1,QByteArray("&#8364;"));
                        }

                        switch(count)
                        {
                            case 1:
                            {
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
                                    QString msg(info);
                                    if (QString::compare(lm->getLine1MsgEffect(),"Fixed",Qt::CaseInsensitive) == 0){
                                        list.append("'>\n"+msg+"\n</SPAN>\n");
                                    }else if (QString::compare(lm->getLine1MsgEffect(),"Blink",Qt::CaseInsensitive) == 0){
                                        list.append("'>\n"+msg+"\n</blink>\n");
                                    }else if (QString::compare(lm->getLine1MsgEffect(),"Scroll",Qt::CaseInsensitive) == 0){
                                        list.append("'>\n"+msg+"\n</MARQUEE>\n");
                                    }else{
                                        list.append("'>\n"+msg+"\n</SPAN>\n");
                                    }
                                    break;
                            }
                            case 2:
                            {
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
                                QString msg(info);
                                if (QString::compare(lm->getLine2MsgEffect(),"Fixed",Qt::CaseInsensitive) == 0){
                                    list.append("'>\n"+msg+"\n</SPAN>\n");
                                }else if (QString::compare(lm->getLine2MsgEffect(),"Blink",Qt::CaseInsensitive) == 0){
                                    list.append("'>\n"+msg+"\n</blink>\n");
                                }else if (QString::compare(lm->getLine2MsgEffect(),"Scroll",Qt::CaseInsensitive) == 0){
                                    list.append("'>\n"+msg+"\n</MARQUEE>\n");
                                }else{
                                    list.append("'>\n"+msg+"\n</SPAN>\n");
                                }
                                break;
                            }
                            case 3:
                            {
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
                                QString msg(info);
                                if (QString::compare(lm->getLine3MsgEffect(),"Fixed",Qt::CaseInsensitive) == 0){
                                    list.append("'>\n"+msg+"\n</SPAN>\n");
                                }else if (QString::compare(lm->getLine3MsgEffect(),"Blink",Qt::CaseInsensitive) == 0){
                                    list.append("'>\n"+msg+"\n</blink>\n");
                                }else if (QString::compare(lm->getLine3MsgEffect(),"Scroll",Qt::CaseInsensitive) == 0){
                                    list.append("'>\n"+msg+"\n</MARQUEE>\n");
                                }else{
                                    list.append("'>\n"+msg+"\n</SPAN>\n");
                                }
                                break;
                            }
                            case 4:
                            {
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
                                QString msg(info);
                                if (QString::compare(lm->getLine4MsgEffect(),"Fixed",Qt::CaseInsensitive) == 0){
                                    list.append("'>\n"+msg+"\n</SPAN>\n");
                                }else if (QString::compare(lm->getLine4MsgEffect(),"Blink",Qt::CaseInsensitive) == 0){
                                    list.append("'>\n"+msg+"\n</blink>\n");
                                }else if (QString::compare(lm->getLine4MsgEffect(),"Scroll",Qt::CaseInsensitive) == 0){
                                    list.append("'>\n"+msg+"\n</MARQUEE>\n");
                                }else{
                                    list.append("'>\n"+msg+"\n</SPAN>\n");
                                }
                                break;
                            }
                            default:
                                    break;
                        }
                        count++;
                        i+=row;
                        if(i>=slen){
                            status = false;
                        }
                    }
                    break;
                case 0x1C:
                    i+=2;
                    break;
                case 0x1D:
                    status = false;
                    break;
                case 0x1E:
                    line = (Data.data()[i+1]-0x30) & 0xFF;
                    row = (Data.data()[i+2]-0x30) & 0xFF;
                    i+=3;
                    i+=line*row;
                    if(i>=slen){
                        status = false;
                    }
                    break;
                case 0x1F:
                    //qDebug() << "Clear Screen";
                    i+=1;
                    break;
                case 0x20:
                case 0x21:
                {
                    y = (Data.data()[i+2]-0x30) & 0xFF;
                    x = (Data.data()[i+3]-0x30) & 0xFF;
                    line = (Data.data()[i+4]-0x30) & 0xFF;
                    row = (Data.data()[i+5]-0x30) & 0xFF;
                    i+=6;
                    //qDebug() << "20 ROW = "+QString("%1").arg(row);

                    QByteArray info = Data.mid(i,row);
                    if(int pos=info.indexOf((char)0xa4) > 0){
                        //qDebug() << "POS="+QString("%1").arg(pos);
                        info.replace(pos,1,QByteArray("&#8364;"));
                    }
                    switch(count)
                    {
                        case 1:
                        {
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
                                QString msg(info);
                                if (QString::compare(lm->getLine1MsgEffect(),"Fixed",Qt::CaseInsensitive) == 0){
                                    list.append("'>\n"+msg+"\n</SPAN>\n");
                                }else if (QString::compare(lm->getLine1MsgEffect(),"Blink",Qt::CaseInsensitive) == 0){
                                    list.append("'>\n"+msg+"\n</blink>\n");
                                }else if (QString::compare(lm->getLine1MsgEffect(),"Scroll",Qt::CaseInsensitive) == 0){
                                    list.append("'>\n"+msg+"\n</MARQUEE>\n");
                                }else{
                                    list.append("'>\n"+msg+"\n</SPAN>\n");
                                }
                                break;
                        }
                        case 2:
                        {
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
                            QString msg(info);
                            if (QString::compare(lm->getLine2MsgEffect(),"Fixed",Qt::CaseInsensitive) == 0){
                                list.append("'>\n"+msg+"\n</SPAN>\n");
                            }else if (QString::compare(lm->getLine2MsgEffect(),"Blink",Qt::CaseInsensitive) == 0){
                                list.append("'>\n"+msg+"\n</blink>\n");
                            }else if (QString::compare(lm->getLine2MsgEffect(),"Scroll",Qt::CaseInsensitive) == 0){
                                list.append("'>\n"+msg+"\n</MARQUEE>\n");
                            }else{
                                list.append("'>\n"+msg+"\n</SPAN>\n");
                            }
                            break;
                        }
                        case 3:
                        {
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
                            QString msg(info);
                            if (QString::compare(lm->getLine3MsgEffect(),"Fixed",Qt::CaseInsensitive) == 0){
                                list.append("'>\n"+msg+"\n</SPAN>\n");
                            }else if (QString::compare(lm->getLine3MsgEffect(),"Blink",Qt::CaseInsensitive) == 0){
                                list.append("'>\n"+msg+"\n</blink>\n");
                            }else if (QString::compare(lm->getLine3MsgEffect(),"Scroll",Qt::CaseInsensitive) == 0){
                                list.append("'>\n"+msg+"\n</MARQUEE>\n");
                            }else{
                                list.append("'>\n"+msg+"\n</SPAN>\n");
                            }
                            break;
                        }
                        case 4:
                        {
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
                            QString msg(info);
                            if (QString::compare(lm->getLine4MsgEffect(),"Fixed",Qt::CaseInsensitive) == 0){
                                list.append("'>\n"+msg+"\n</SPAN>\n");
                            }else if (QString::compare(lm->getLine4MsgEffect(),"Blink",Qt::CaseInsensitive) == 0){
                                list.append("'>\n"+msg+"\n</blink>\n");
                            }else if (QString::compare(lm->getLine4MsgEffect(),"Scroll",Qt::CaseInsensitive) == 0){
                                list.append("'>\n"+msg+"\n</MARQUEE>\n");
                            }else{
                                list.append("'>\n"+msg+"\n</SPAN>\n");
                            }
                            break;
                        }
                        default:
                            break;
                    }
                    count++;
                    i+=row;
                    if(i>=slen){
                        status = false;
                    }
                }
                    break;
                default:
                    //qDebug() << "NADA";
                    status = false;
                    break;
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
void DisplayGenio::createWelcomePage(QString welcomepage)
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
                list.append("border-radius:15px;padding:5px;");
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

QString DisplayGenio::headScriptHTML()
{
        QList<QString> list;
        QString result = "";
        list << "<HTML>\n";
        list.append("<HEAD>\n");
	//list.append("<meta http-equiv=Content-Type content=\"text/html; charset=UTF-8\">\n");
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
