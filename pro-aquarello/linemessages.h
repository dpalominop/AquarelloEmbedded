#ifndef LINEMESSAGES_H
#define LINEMESSAGES_H
#include <QObject>
#include <QMap>
#include <QApplication>
#include <QXmlStreamReader>

class LineMessages : public QObject
{
    Q_OBJECT
public:
    explicit LineMessages(QObject *parent = 0);

private:
    QString PATH;
    int Line1MsgX;
    int Line1MsgY;
    QString Line1MsgForecolor;
    QString Line1MsgColor;
    int Line1MsgSize;
    QString Line1MsgEffect;
    QString Line1MsgStyle;
    QString Line1MsgWeight;
    //
    int Line2MsgX;
    int Line2MsgY;
    QString Line2MsgForecolor;
    QString Line2MsgColor;
    int Line2MsgSize;
    QString Line2MsgEffect;
    QString Line2MsgStyle;
    QString Line2MsgWeight;
    //
    int Line3MsgX;
    int Line3MsgY;
    QString Line3MsgForecolor;
    QString Line3MsgColor;
    int Line3MsgSize;
    QString Line3MsgEffect;
    QString Line3MsgStyle;
    QString Line3MsgWeight;
    //
    int Line4MsgX;
    int Line4MsgY;
    QString Line4MsgForecolor;
    QString Line4MsgColor;
    int Line4MsgSize;
    QString Line4MsgEffect;
    QString Line4MsgStyle;
    QString Line4MsgWeight;
    //
    void parseXML();
    QList< QMap<QString,QString> > params;
    QMap<QString, QString> parseMessage(QXmlStreamReader& xml);
    void addElementDataToMap(QXmlStreamReader& xml,
                                 QMap<QString, QString>& map) const;
    void writeConfigDefault(QString);
public:
    int getLine1MsgX();
    void setLine1MsgX(int);
    int getLine1MsgY();
    void setLine1MsgY(int);
    QString getLine1MsgForecolor();
    void setLine1MsgForecolor(QString);
    QString getLine1MsgColor();
    void setLine1MsgColor(QString);
    int getLine1MsgSize();
    QString getLine1MsgEffect();
    void setLine1MsgEffect(QString);
    QString getLine1MsgStyle();
    void setLine1MsgStyle(QString);
    QString getLine1MsgWeight();
    void setLine1MsgWeight(QString);
    //
    int getLine2MsgX();
    void setLine2MsgX(int);
    int getLine2MsgY();
    void setLine2MsgY(int);
    QString getLine2MsgForecolor();
    void setLine2MsgForecolor(QString);
    QString getLine2MsgColor();
    void setLine2MsgColor(QString);
    int getLine2MsgSize();
    QString getLine2MsgEffect();
    void setLine2MsgEffect(QString);
    QString getLine2MsgStyle();
    void setLine2MsgStyle(QString);
    QString getLine2MsgWeight();
    void setLine2MsgWeight(QString);
    //
    int getLine3MsgX();
    void setLine3MsgX(int);
    int getLine3MsgY();
    void setLine3MsgY(int);
    QString getLine3MsgForecolor();
    void setLine3MsgForecolor(QString);
    QString getLine3MsgColor();
    void setLine3MsgColor(QString);
    int getLine3MsgSize();
    QString getLine3MsgEffect();
    void setLine3MsgEffect(QString);
    QString getLine3MsgStyle();
    void setLine3MsgStyle(QString);
    QString getLine3MsgWeight();
    void setLine3MsgWeight(QString);
    //
    int getLine4MsgX();
    void setLine4MsgX(int);
    int getLine4MsgY();
    void setLine4MsgY(int);
    QString getLine4MsgForecolor();
    void setLine4MsgForecolor(QString);
    QString getLine4MsgColor();
    void setLine4MsgColor(QString);
    int getLine4MsgSize();
    QString getLine4MsgEffect();
    void setLine4MsgEffect(QString);
    QString getLine4MsgStyle();
    void setLine4MsgStyle(QString);
    QString getLine4MsgWeight();
    void setLine4MsgWeight(QString);
};
#endif // LINEMESSAGES_H
