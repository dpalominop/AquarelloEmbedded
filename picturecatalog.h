#ifndef PICTURECATALOG_H
#define PICTURECATALOG_H

#include <QGraphicsWidget>
#include <QLabel>
#include <QDir>
#include <QCoreApplication>


class PictureCatalog : public QLabel
{
    Q_OBJECT
public:
    explicit PictureCatalog(QWidget *parent = 0);

    void showNextPicture();
    bool catalogIsFinished;
    
private:

    int catalogCounter;
    int catalogSize;
    QDir *picturesDir;
    //QFileInfoList pictureList;
    QStringList pictureList;
    
};

#endif // PICTURECATALOG_H
