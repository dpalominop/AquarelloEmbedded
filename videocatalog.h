#ifndef VIDEOCATALOG_H
#define VIDEOCATALOG_H

#include <QDir>
#include <QtMultimedia/QMediaPlayer>
#include <QtMultimediaWidgets/QVideoWidget>
#include <QtMultimedia/QMediaPlaylist>
#include <QThread>
#include <QCoreApplication>

class VideoCatalog : public QVideoWidget
{

    Q_OBJECT
public:
    explicit VideoCatalog(QWidget *parent = 0);
    void showNextVideo();
    void stopPlaylist();
    bool catalogIsFinished;
    QMediaPlayer * catalog_player;

private:

    QMediaPlaylist * catalog_playlist;
    QDir * videosDir;
    int videoCatalogCounter;
    int catalogSize;
    QFileInfoList videoList;



private slots:
    void pickState(QMediaPlayer::State state);
    void printState(QMediaPlayer::State state);

signals:
    void videoFinished(QMediaPlayer::MediaStatus mediaStatus);


};

#endif // VIDEOCATALOG_H
