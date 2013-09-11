#include "display.h"

Display::Display(QObject *parent) :
    QObject(parent)
{

}
void Display::strData(QString str)
{
	strdata = str;
}
void Display::byteData(QByteArray data)
{
        bData = data;
        //qDebug() << Data;
}
void Display::setTerminal(QString terminal)
{
    termDisplay = terminal;
}

void Display::setBackImgPath(QString path)
{
    backImgPath = path;
}
void Display::setForeImgPath(QString path)
{
    foreImgPath = path;
}
void Display::setDataAccessFault(QString message)
{
    errMessage = message;
}
void Display::createDisplayPage(QString htmlpage)
{
    if (QString::compare(termDisplay,"Aquarelle",Qt::CaseInsensitive) == 0){
        createDisplayPageAqua(htmlpage);

    }else if (QString::compare(termDisplay,"Genio",Qt::CaseInsensitive) == 0){
        createDisplayPageGenio(htmlpage);

    }else if (QString::compare(termDisplay,"TCP-CRS",Qt::CaseInsensitive) == 0){
        createDisplayPageIBM(htmlpage);

    }else if (QString::compare(termDisplay,"WEB",Qt::CaseInsensitive) == 0){
        createDisplayPageGenio(htmlpage);
    }else{
        createDisplayPageGenio(htmlpage);
    }

}

void Display::createDisplayPageAqua(QString htmlpage)
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
	//bool status=1;
	//char buf[1024];
	//char buftmp[100];
	//char *ptri,*ptrf,*ptrz;
	//int n;
	int posi=0;
	int posf,command;
	
	//ptri=buf;
	bool status=1;
	QString tmp;
	//strcpy(buf, strdata.toLocal8Bit().constData());
	//strcpy(buf, strdata.toUtf8().constData());
	//qDebug() << strdata;
	while(status){
	    if((posf=strdata.indexOf('\t', posi)) < 0){
                break;
            }
	    tmp = strdata.mid(posi, posf-posi);
	    command=tmp.toInt();		
	    switch(command)
	    {

		case 25:
		{
			QStringList msglist;
			int pos11=posf+1;
                        int pos12;
                        for (int i=0; i<10; i++){
                            if(i==9){
                                if((pos12=strdata.indexOf('\n', pos11)) > 0){
                                     msglist.append(strdata.mid(pos11, pos12-pos11));
                                }else{
				     msglist.append(strdata.mid(pos11));
				     pos12=strdata.size();
                                }
                             }else{
                                if((pos12=strdata.indexOf('\t', pos11)) > 0){
                                     msglist.append(strdata.mid(pos11, pos12-pos11));
                                }else{
				     goto final;
                             	}
                             }
                             pos11=pos12+1;
                        }
                        posi=pos11;
                        //qDebug() << msglist[9];

			int effect = msglist[8].toInt();
			switch(effect)
			{
				case 0:
					list.append(QString("<SPAN style='position:absolute;left:%1px;top:%2px;").arg(msglist[0]).arg(msglist[1]));
					break;
				case 1:
					list.append(QString("<blink style='position:absolute;left:%1px;top:%2px;").arg(msglist[0]).arg(msglist[1]));
                                        break;
				case 2:
					list.append(QString("<MARQUEE style='position:absolute;left:%1px;top:%2px;").arg(msglist[0]).arg(msglist[1]));
                                        break;
				default:
					break;
			}
			list.append(backgroundColor(msglist[2]));
			list.append(foregroundColor(msglist[3]));
			list.append(QString("font-size:%1pt;").arg(msglist[4]));
			list.append("font-family:arial;");
			list.append(styleFont(msglist[5]));
                        //qDebug() << msglist[9];
			if(msglist[9].indexOf((char)0xa4) > 0){
				msglist[9] = msglist[9].replace(QChar('\xA4'),QString("&#8364"));
			}
			if(msglist[9].indexOf("&#164;") > 0){
				msglist[9] = msglist[9].replace(QString("&#164;"),QString("&#8364;"));
			}
			//QString text = msglist[9].replace(QChar('\xA4'),QString("&#8364"));
			//qDebug() << text;
			switch(effect){
                        	case 0:
                                	list.append(QString("'>\n%1\n</SPAN>\n").arg(msglist[9]));
                                	break;
                        	case 1:
                                	list.append(QString("'>\n%1\n</blink>\n").arg(msglist[9]));
                                	break;
                        	case 2:
                                	list.append(QString("'>\n%1\n</MARQUEE>\n").arg(msglist[9]));
                                	break;
                        	default:
                                	list.append(QString("'>\n%1\n</SPAN>\n").arg(msglist[9]));
                                	break;
                    	}

			break;
		}
		default:
			//ptri+=1;
			status=false;
			break;	
	    }
	}
final:
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
void Display::createDisplayPageGenio(QString htmlpage)
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
        int x,y,line,row;
        int i=0;
        int count=1;
        bool status=1;

        int slen=bData.size();

        lm = new LineMessages();
        while(status)
        {
            //qDebug() << "SWITCH="+QString("%1").arg(Data.data()[i]);
            if(count > 4)
                break;
            switch(bData.data()[i]){
                case 0x1A:
                    i+=1;
                    break;
                case 0x1B:
                    //qDebug() << "0x1B";
                    if((bData.data()[i+1] == 0x50) || (bData.data()[i+1] == 0x55)){
                        line = (bData.data()[i+4]-0x30) & 0xFF;
                        row = ((bData.data()[i+5]-0x30) & 0xFF)*3;
                        i+=6;
                        i+=line*row;
                        if(i>=slen){
                            status = false;
                        }
                    }else{
                        y = (bData.data()[i+2]-0x30) & 0xFF;
                        x = (bData.data()[i+3]-0x30) & 0xFF;
                        line = (bData.data()[i+4]-0x30) & 0xFF;
                        row = (bData.data()[i+5]-0x30) & 0xFF;
                        //qDebug() << "ROW = "+QString("%1").arg(row);
                        i+=6;
                        QByteArray info = bData.mid(i,row);
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
                    line = (bData.data()[i+1]-0x30) & 0xFF;
                    row = (bData.data()[i+2]-0x30) & 0xFF;
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
                    y = (bData.data()[i+2]-0x30) & 0xFF;
                    x = (bData.data()[i+3]-0x30) & 0xFF;
                    line = (bData.data()[i+4]-0x30) & 0xFF;
                    row = (bData.data()[i+5]-0x30) & 0xFF;
                    i+=6;
                    //qDebug() << "20 ROW = "+QString("%1").arg(row);

                    QByteArray info = bData.mid(i,row);
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
void Display::createDisplayPageIBM(QString htmlpage)
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
QString Display::backgroundColor(QString color)
{
	if (QString::compare(color,"0",Qt::CaseInsensitive) == 0) return "background-color:transparent;";
        else if (QString::compare(color,"5",Qt::CaseInsensitive) == 0) return "background-color:yellow;border-radius:15px;padding:5px;";
        else if (QString::compare(color,"35",Qt::CaseInsensitive) == 0) return "background-color:red;border-radius:15px;padding:5px;";
        else if (QString::compare(color,"185",Qt::CaseInsensitive) == 0) return "background-color:green;border-radius:15px;padding:5px;";
        else if (QString::compare(color,"206",Qt::CaseInsensitive) == 0) return "background-color:blue;border-radius:15px;padding:5px;";
        else if (QString::compare(color,"215",Qt::CaseInsensitive) == 0) return "background-color:black;border-radius:15px;padding:5px;";
        else return "background-color:red;border-radius:15px;padding:5px;";
}
QString Display::foregroundColor(QString color)
{
        if (QString::compare(color,"0",Qt::CaseInsensitive) == 0) return "color:transparent;";
        else if (QString::compare(color,"5",Qt::CaseInsensitive) == 0) return "color:yellow;";
        else if (QString::compare(color,"35",Qt::CaseInsensitive) == 0) return "color:red;";
        else if (QString::compare(color,"185",Qt::CaseInsensitive) == 0) return "color:green;";
        else if (QString::compare(color,"206",Qt::CaseInsensitive) == 0) return "color:blue;";
        else if (QString::compare(color,"215",Qt::CaseInsensitive) == 0) return "color:black;";
        else return "color:red;";
}
QString Display::styleFont(QString style)
{
	if (QString::compare(style,"0",Qt::CaseInsensitive) == 0) return "font-style:normal;";
	else if(QString::compare(style,"1",Qt::CaseInsensitive) == 0) return "font-style:italic;";
	else if(QString::compare(style,"2",Qt::CaseInsensitive) == 0) return "font-weight:bold;";
	else return "font-style:normal;";
}
void Display::createWelcomePage(QString welcomepage)
{
    int posi=0;
    int posf,pos1,command;
    bool status=1;
    QString tmp;
    int len = strdata.length();
    int cnt=0;
    QStringList msglist;
    //qDebug() << strdata.count;
    //qDebug() << len;
    msg=new WelcomeConfiguration();
    if(strdata.count() > 0){
        if(QString::compare(msg->getMsgVisible(),"true",Qt::CaseInsensitive) == 0){
            while(status){
                if(cnt >= len){
                    break;
                }
                if((posf=strdata.indexOf('\t', posi)) < 0){
                    break;
                }
                tmp = strdata.mid(posi, posf-posi);
                        //qDebug() << "command="+tmp;
                command=tmp.toInt();
                switch(command)
                {
                    case 19:
                        if((pos1=strdata.indexOf('\n', posf+1)) >= 0){
                                    }else{
                        }
                        posi=pos1+1;
                        break;

                    case 20:
                    {
                        int pos11=posf+1;
                        int pos12;
                        for (int i=0; i<10; i++){
                            if(i==9){
                                if((pos12=strdata.indexOf('\n', pos11)) >= 0){
                                    //msglist.append(strdata.mid(pos11, pos12-pos11));
                                }else{
                                }
                            }else{
                                if((pos12=strdata.indexOf('\t', pos11)) >= 0){
                                    //msglist.append(strdata.mid(pos11, pos12-pos11));
                                }else{
                                }
                            }
                            pos11=pos12+1;
                        }
                        posi=pos11;
                        break;
                    }
                    case 21:
                    {
                        int pos21=posf+1;
                        //qDebug() << "DATA="+strdata.mid(pos21);
                        int pos22;
                        for (int i=0; i<10; i++){
                            if(i==9){
                                if((pos22=strdata.indexOf('\n', pos21)) >= 0){
                                    msglist.append(strdata.mid(pos21, pos22-pos21));
                                }else{
                                }
                            }else{
                                if((pos22=strdata.indexOf('\t', pos21)) >= 0){
                                    msglist.append(strdata.mid(pos21, pos22-pos21));
                                }else{
                                }
                            }
                            pos21=pos22+1;
                        }
                        posi=pos21;
                        break;
                    }
                    default:
                        status=false;
                        break;
                        }
                        cnt++;
                    }
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
                    int effect = msglist[8].toInt();
                    switch(effect)
                    {
                            case 0:
                                    list.append(QString("<SPAN style='position:absolute;left:%1px;top:%2px;").arg(msglist[0]).arg(msglist[1]));
                                    break;
                            case 1:
                                    list.append(QString("<blink style='position:absolute;left:%1px;top:%2px;").arg(msglist[0]).arg(msglist[1]));
                                    break;
                            case 2:
                                    list.append(QString("<MARQUEE style='position:absolute;left:%1px;top:%2px;").arg(msglist[0]).arg(msglist[1]));
                                    break;
                            default:
                                    break;
                    }
                    list.append(backgroundColor(msglist[2]));
                    list.append(foregroundColor(msglist[3]));
                    list.append(QString("font-size:%1pt;").arg(msglist[4]));
                    list.append("font-family:arial;");
                    list.append(styleFont(msglist[5]));
                    qDebug() << msglist[9];
                    if(msglist[9].indexOf((char)0xa4) > 0){
                            msglist[9] = msglist[9].replace(QChar('\xA4'),QString("&#8364"));
                    }
                    if(msglist[9].indexOf("&#164;") > 0){
                            msglist[9] = msglist[9].replace(QString("&#164;"),QString("&#8364;"));
                    }
                    switch(effect){
                            case 0:
                                    list.append(QString("'>\n%1\n</SPAN>\n").arg(msglist[9]));
                                    break;
                            case 1:
                                    list.append(QString("'>\n%1\n</blink>\n").arg(msglist[9]));
                                    break;
                            case 2:
                                    list.append(QString("'>\n%1\n</MARQUEE>\n").arg(msglist[9]));
                                    break;
                            default:
                                    list.append(QString("'>\n%1\n</SPAN>\n").arg(msglist[9]));
                                    break;
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
        }else{
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
    }else{
            if(QString::compare(msg->getMsgVisible(),"true",Qt::CaseInsensitive) == 0){
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
		list.append("</div>\n");
		list.append("</BODY>\n");
		list.append("</HTML>\n");

		QString result = "";
		QString s;
	        foreach( s, list )
        	        result +=s;
		out << result;
		file.close();
            }else{
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
    }
}
QString Display::headScriptHTML()
{
	QList<QString> list;
	QString result = "";
	list << "<HTML>\n";
        list.append("<HEAD>\n");
	list.append("<meta http-equiv=\"Content-type\" content=\"text/html;charset=UTF-8\">\n");
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
void Display::parseXML(QString tag, QString data) {
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
            //if(xml.name() == "NormalSaleItem") {
            if(xml.name() == tag) {
                this->parseMessageIBM(xml);
            }
        }
    }
    /* Error handling. */
    if(xml.hasError()) {
        printf("Imposible parse config.xml\r\\n");
    }
    xml.clear();
}
void Display::parseMessageIBM(QXmlStreamReader& xml) {

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
void Display::clearFields(){
    this->ItemCode="";
    this->SalePrice="";
    this->ItemName="";
    this->RestrictedSaleType="";
    this->Discount="";
    this->errMessage="";
}
