#ifndef VIDEO_H
#define VIDEO_H

#include <QThread>
#include <QWidget>
#include <QDebug>
#include <QImage>
#include <QPixmap>
#include <QList>
#include <QTimer>
#include <QFile>
#include <QFileDialog>
//#include "videodec.h"
#include "videodecthread.h"
namespace Ui {
class Video;
}

class Video : public QWidget
{
    Q_OBJECT

public:
    explicit Video(QWidget *parent = 0);
    ~Video();
     VideoDecThread * decThread;
      QString  FileName;
#if 0
    QThread *VideoDecThread;
       VideoDec *OVideoDec;
       QString FileName;
       QTimer *readTimer;
       int BUFFINDEX;
       QFileDialog * qFileDialog;
signals:
       void init_s();
       void play_s();
#endif
//signals:
//       void onFocus();
   public slots:
       void showVideo(QImage img);
   private slots:
       void on_play_clicked();

       void on_open_clicked();

private:
    Ui::Video *ui;

};

#endif // VIDEO_H
