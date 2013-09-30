#include "videocatalog.h"

VideoCatalog::VideoCatalog(QWidget *parent) :
QVideoWidget(parent),
catalogIsFinished(false),
catalog_player(new QMediaPlayer()),
catalog_playlist(new QMediaPlaylist(catalog_player)),//not using this yet
videosDir(new QDir(QCoreApplication::applicationDirPath()+"/catalog_videos")),
videoCatalogCounter(0),
catalogSize(0)
{
    QStringList filters;
    filters << "*.mp4" << "*.avi" << "*.flv" << "*.mpg" << "*.mpeg" <<"*.mp3"<< "*.3gp";
    videosDir->setNameFilters(filters);
    videoList = videosDir->entryInfoList();
    catalogSize = videoList.size();
    connect(catalog_player,SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)),this,SIGNAL(videoFinished(QMediaPlayer::MediaStatus)));
    catalog_player->setVideoOutput(this);



}

void VideoCatalog::showNextVideo()
{
    QFileInfo fileInfo;
    if(catalogSize > 0 && videoCatalogCounter < catalogSize)
        {
        fileInfo = videoList.at(videoCatalogCounter);
        catalog_player->setMedia(QUrl::fromLocalFile(fileInfo.absoluteFilePath()));
        catalog_player->play();
        videoCatalogCounter++;
        catalogIsFinished=false;
        }
    if(videoCatalogCounter == catalogSize)
        {

        videoCatalogCounter = 0;
        catalogIsFinished=true;
        }
}

void VideoCatalog::stopPlaylist()
{
//catalog_player->pause();
catalog_player->stop();
videoCatalogCounter = 0;
catalogIsFinished=true;
qDebug() << "VideoCatalog : Video Stop Requested\n";
}

void VideoCatalog::pickState(QMediaPlayer::State state)
{
    if(state == QMediaPlayer::StoppedState)
    {
        qDebug() << "VideoCatalog : STOPPED state\n";
    }
}

void VideoCatalog::printState(QMediaPlayer::State state)
{
    qDebug() << "Video Catalog : Video Qthread STARTED!";
}
