#ifndef VIDEODEC_H
#define VIDEODEC_H
#include <QDebug>

extern "C"{
    #include "libavcodec/avcodec.h"
    #include "libavformat/avformat.h"
    #include "libavutil/avutil.h"
    #include "libavutil/mem.h"
    #include "libavutil/fifo.h"
    #include "libswscale/swscale.h"
}
#include <QObject>
#include <QImage>
#include <QThread>
#include <QDebug>
#include <QList>
//#include "videodec.h"

class VideoDec : public QObject
{
    Q_OBJECT
public:
    explicit VideoDec(QObject *parent = 0);
    struct ImgPacket{
        QImage PImage;
        struct ImgPacket *next;
    };

    static int read_buffer(void *opaque, uint8_t *buf, int buf_size);
    FILE *getFp(){return fp;}
    int videoindex;
    int BUFFSIZE;
    int width;
    int height;
    AVFormatContext *pFormatCtx;
    AVCodecContext *pCodecCtx;
    AVCodec *pCodec;
    AVPicture * pPic;
    AVPacket* pPacket;
    AVFrame *pFrame,*pFrameRGB;
    char *FileName = "E:/mv/1.mp4";
    QList<QImage> VideoImg;

    ~VideoDec();
signals:
    void SendImage(QImage img);

public slots:
    void init();
    void play();
private:
    FILE * fp;
  #if 0
    bool playerState;

            AVFormatContext *pFormatContext;
            AVCodecContext *pCodecContext;
            AVCodec *pCodec;
            AVPacket *pPacket;
            AVFrame *pFrame;
            AVIOContext *pIOContext;
            SwsContext * pSwsContext;
            AVPicture *pPic;
            int vIndex;

            FILE *fp;
            VideoDec * videodec;
 #endif
};
#endif // VIDEODEC_H
