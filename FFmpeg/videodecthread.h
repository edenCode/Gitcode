#ifndef VIDEODECTHREAD_H
#define VIDEODECTHREAD_H

#include <QImage>
#include <QThread>
#include <QObject>
#include <QDebug>
#include <QByteArray>
#include <QString>

extern "C"{
    #include "libavcodec/avcodec.h"
    #include "libavformat/avformat.h"
    #include "libavutil/avutil.h"
    #include "libavutil/mem.h"
    #include "libavutil/fifo.h"
    #include "libswscale/swscale.h"
    #include "libavutil/dict.h"
}
//D:\worksapce\FFmpeg\include\libavutil\dict.h
class VideoDecThread : public QThread
{

    Q_OBJECT
public:
   explicit VideoDecThread(QObject *parent = 0);
    int play_init(void);

    virtual void run();
    ~VideoDecThread();
    int setPath(QString path){
        if(!path.isNull()){
            qDebug() << "setPath ????? filePath: " << path;
         //  QByteArray byte = path.toLocal8Bit();
            filename = QString(path);
            qDebug() <<"FileName ： " << filename;
        }else{
            qDebug() <<"filename Null\n";
        }
        return 0;
    }
signals:
    void display(QImage);
//    void decThreadexit();
private:
 //   FILE * fp;
    bool playerState;

            AVFormatContext *pFormatContext;
            AVCodecContext *pCodecContext;
            AVCodec *pCodec;
            AVPacket *pPacket;
            AVFrame *pFrame;
            AVIOContext *pIOContext;
            SwsContext * pSwsContext;
            AVPicture *pPic;
            int videoindex;
            QString filename;
//    /        static char[1024];
          //  VideoDec * videodec;
};

#endif // VIDEODECTHREAD_H
