#include "videodec.h"
#include <stdio.h>
VideoDec::VideoDec(QObject *parent){


}

VideoDec::~VideoDec(){


}
#if 0
void VideoDec::SendImage(QImage img){


}
#endif
#if 0
void VideoDec::init(){
    BUFFSIZE = 0;
        avcodec_register_all();
        av_register_all();
        pFormatCtx = avformat_alloc_context();
        pCodecCtx = avcodec_alloc_context3(pCodec);
        if(avformat_open_input(&pFormatCtx,FileName,NULL,NULL)!=0){
            qDebug()<<"OpenFail";
        }
        if(avformat_find_stream_info(pFormatCtx,NULL)<0){
            qDebug()<<"FindFail";
        }
        videoindex = -1;
        for(int i=0;pFormatCtx->nb_streams;i++){
            if(pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO){
                videoindex = i;
                break;
            }
        }
        if(videoindex == -1){
            qDebug()<<"Din't find video stream";
        }
        pCodecCtx = pFormatCtx->streams[videoindex]->codec;
        pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
        if(pCodec == NULL){
            qDebug()<<"codec not find";
        }
        if(avcodec_open2(pCodecCtx,pCodec,NULL)<0){
            qDebug()<<"can't open codec";
        }
        pFrame = av_frame_alloc();
    //    pFrameRGB = av_frame_alloc();
        play();
}

#else
void VideoDec::init(){


    qDebug() << "VideoDec::init() : " <<QThread::currentThreadId();
    /*
        注册编解码器
    */
    av_register_all();

    /*
        注册协议
    */
    avformat_network_init();

    /*
        申请 AVFormatContext 结构内存空间
    */
    pFormatCtx=avformat_alloc_context();


    qDebug() <<"FileName : "<<FileName;
    /*
        打开文件
    */
    fp=fopen(FileName,"rb");

    if(fp==NULL){
        qDebug() <<FileName<<"open Failed";
    }

    /*
        初始化 AVIOContext 结构
    */
    //Init AVIOContext
    unsigned char *aviobuffer=(unsigned char *)av_malloc(1024*1024*20);
    AVIOContext *avio =avio_alloc_context(aviobuffer, 1024*1024*20,0,(void *)this,VideoDec::read_buffer,NULL,NULL);
    pFormatCtx->pb=avio;


    /*
        打开 AVFormatContext 中的 输入 Streams 并读取文件头，但没有初始化 编解码器
    */
    int theErr=avformat_open_input(&pFormatCtx,NULL,NULL,NULL);
    if(theErr!=0){
        qDebug() << "avformat_open_input Error\n";

    }

    printf("Filename : %s,streams : %d \n",pFormatCtx->filename,pFormatCtx->nb_streams);
    /*
        读取多媒体文件获取流信息
    */
    theErr=avformat_find_stream_info(pFormatCtx,NULL);
    if(theErr<0){
        qDebug() << "avformat_find_stream_info Error\n";
    }

    /*
        查找视频流 索引
    */
    videoindex=-1;
    for(int i=0;i<pFormatCtx->nb_streams;++i){
        if(pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO){
            videoindex=i;
            break;
        }
    }
    if(videoindex==-1){
        qDebug() <<"Not Find Video Type\n";
    }

    pCodecCtx=pFormatCtx->streams[videoindex]->codec;

    printf("av id : %#x,av type : %d",pCodecCtx->codec_descriptor->id,pCodecCtx->codec_descriptor->type);
    qDebug() << " av id :" << pCodecCtx->codec_descriptor->id;
    qDebug() <<"av type :" << pCodecCtx->codec_descriptor->type;
    pCodec=(AVCodec *)pCodecCtx->codec;

    /*
        查找编解码器
    */
    pCodec=avcodec_find_decoder(pCodecCtx->codec_id);
    if(pCodec==NULL){
       qDebug() <<"Codec not found.\n";
    }

    /*
        根据查找到的AVCodec 初始化 AVCodecContext
    */
    if(avcodec_open2(pCodecCtx , pCodec,NULL)<0){
       qDebug() <<"Could not open codec.\n";
    }

    pFrame=av_frame_alloc();
    pPacket=(AVPacket *)av_malloc(sizeof(AVPacket));


    pPic=(AVPicture *)av_frame_alloc();
    avpicture_alloc(pPic,AV_PIX_FMT_RGB24,640,320);

    /*
        初始化 SwsContext
    */
   // pSwsContext = sws_getContext(640,320,AV_PIX_FMT_YUV420P,640,320,AV_PIX_FMT_RGB24,SWS_BICUBIC,0,0,0);
    play();
}
#endif
void VideoDec::play(){
    int got_picture;
       // av_new_packet(&packet, pCodecCtx->width*pCodecCtx->height);
        qDebug() << "VideoDec::play()"<<QThread::currentThreadId();
       // uint8_t *out_buffer;
       // out_buffer = new uint8_t[avpicture_get_size(AV_PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height)];//分配AVFrame所需内存
        //avpicture_fill((AVPicture *)pFrameRGB, out_buffer, AV_PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height);//填充AVFrame
      //  avpicture_fill((AVPicture *)pFrame, out_buffer, AV_PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height);//填充AVFrame
        while(1){
            QThread::msleep(1);
            qDebug() << "av Decode\n";
            if(BUFFSIZE>51) continue;
            if(!(av_read_frame(pFormatCtx,pPacket)>=0)){
                qDebug() <<"av_read_frame Failed\n";
                break;
            }
            if(pPacket->stream_index == videoindex){
                int rec = avcodec_decode_video2(pCodecCtx,pFrame,&got_picture,pPacket);
                if(got_picture){
                    /*struct SwsContext *sws_getContext(int srcW, int srcH, enum AVPixelFormat srcFormat,
                    int dstW, int dstH, enum AVPixelFormat dstFormat,
                    int flags, SwsFilter *srcFilter,
                    SwsFilter *dstFilter, const double *param)*/
                  //  SwsContext *convertCtx = sws_getContext(pCodecCtx->width,pCodecCtx->height
                  //                                          ,pCodecCtx->pix_fmt,pCodecCtx->width,pCodecCtx->height
                  //                                          ,AV_PIX_FMT_YUV420P,SWS_BICUBIC,NULL,NULL,NULL);
                   qDebug() <<"Get picture";
                  SwsContext *convertCtx = sws_getContext(pCodecCtx->width,pCodecCtx->height,AV_PIX_FMT_YUV420P,pCodecCtx->width,pCodecCtx->height,AV_PIX_FMT_RGB24,SWS_BICUBIC,0,0,0);
                    sws_scale(convertCtx,(const uint8_t*  const*)pFrame->data,pFrame->linesize,0
                              ,pCodecCtx->height,pPic->data,pPic->linesize);
                    QImage Img((uchar *)pPic->data[0],pCodecCtx->width,pCodecCtx->height,QImage::Format_RGB32);
                    VideoImg.append(Img.copy());
                    BUFFSIZE++;
                    sws_freeContext(convertCtx);
                }

                av_free_packet(pPacket);
            }
        }

}

int VideoDec::read_buffer(void *opaque, uint8_t *buf, int buf_size)
{
    VideoDec *dt=(VideoDec *)opaque;

    FILE *tFp=dt->getFp();
    if(!feof(tFp)){
           int true_size=fread(buf,1,buf_size,tFp);
           qDebug() << "Av packet\n";
           return true_size;
       }else{
           return -1;
       }
}

#if 0
void VideoDec::run(){

 play();
}
#endif
