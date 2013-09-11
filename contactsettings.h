#ifndef CONTACTSETTINGS_H
#define CONTACTSETTINGS_H

#include <QWidget>
#include <QDebug>
#include <QString>
#include <QMessageBox>
#include <QtCore/QPointer>
#include <QtCore/QFile>
#include <QtCore/QIODevice>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QXmlStreamReader>

class ContactSettings : public QWidget
{
    Q_OBJECT
public:
    explicit ContactSettings(QWidget *parent = 0);
    explicit ContactSettings(QWidget *parent = 0, QString XmlFile="./settings/contacts.xml");
    
    QList< QMap<QString,QString> > contacts;

signals:
    
public slots:
    void parseXML(QString XmlFile);

private:
    QMap<QString, QString> parseContact(QXmlStreamReader& xml);
    void addElementDataToMap(QXmlStreamReader& xml, QMap<QString, QString>& map) const;
    inline static bool qMapLessThan(const QMap<QString, QString> &c1, const QMap<QString, QString> &c2)
    {
        return c1["id"].toInt()<c2["id"].toInt();
    }
};

#endif // CONTACTSETTINGS_H
