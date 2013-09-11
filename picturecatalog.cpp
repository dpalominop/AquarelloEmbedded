#include "picturecatalog.h"

PictureCatalog::PictureCatalog(QWidget *parent) :
    QLabel(parent)
    ,catalogCounter(0)
    ,catalogSize(0)
    ,picturesDir(new QDir(QCoreApplication::applicationDirPath()+"/catalog_pictures"))
    ,catalogIsFinished(false)

{
    picturesDir->setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    picturesDir->setSorting(QDir::Size | QDir::Reversed);

    QFileInfoList list = picturesDir->entryInfoList();
    //SlideList
    for (int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);
        if((!fileInfo.suffix().compare("png",Qt::CaseInsensitive) | !fileInfo.suffix().compare("jpeg",Qt::CaseInsensitive) | !fileInfo.suffix().compare("jpg",Qt::CaseInsensitive)
            | !fileInfo.suffix().compare("bmp",Qt::CaseInsensitive) | !fileInfo.suffix().compare("gif",Qt::CaseInsensitive)) && (fileInfo.baseName().compare("background",Qt::CaseInsensitive) != 0 && fileInfo.baseName().compare("foreground",Qt::CaseInsensitive) != 0)){

            pictureList << fileInfo.absoluteFilePath();
        }
    }
    catalogSize=pictureList.length();

}

void PictureCatalog::showNextPicture()
{
    if(catalogSize > 0 && catalogCounter < catalogSize )
    {
        QString filename = pictureList[catalogCounter];
        this->setPixmap(QPixmap::fromImage(QImage(filename)));
        catalogCounter++;
        catalogIsFinished=false;
    }

    if(catalogCounter == catalogSize)
    {
        catalogCounter = 0;
        catalogIsFinished=true;
    }
}
