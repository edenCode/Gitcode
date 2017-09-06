#include "videodecthread.h"

VideoDecThread::VideoDecThread(QObject *parent):QThread(parent),playerState(false),pFormatContext(NULL),
    pCodecContext(NULL),pCodec(NULL),pPacket(NULL),pFrame(NULL),
    pIOContext(NULL),videoindex(-1),pSwsContext(NULL)
{
    qDebug() <<"VideoDecThread(QObject *parent) function";
    play_init();
}


VideoDecThread::~VideoDecThread()
{
    avcodec_free_context(&pCodecContext);
    avformat_free_context(pFormatContext);

}

// video play FFmpeg Init
int VideoDecThread::play_init()
{
    int ret;
    int hour;
    int mintue;
    int seconds;
    qDebug() << "play init";
    /*
        注册编解码器
    */
//    qDebug() << avcodec_version();
    av_register_all();
     qDebug() <<"av_register_all()";
    /*
        注册协议
    */
  //  avformat_network_init();
  //   qDebug() <<"avformat_network_init()()";

    pFormatContext = avformat_alloc_context();
    if(pFormatContext == NULL){
        qDebug() << "avformat_alloc_context";
        return -1;
    }



   // qDebug() <<"fileName :"  << filename;
   if ((ret = avformat_open_input(&pFormatContext,"E:/mv/1.mp4",NULL,NULL)) != 0){
       qDebug() << filename<<"open Failed\n" << ret;
       return -1;
   }

   hour = pFormatContext->duration/3600;
   mintue=pFormatContext->duration%3600/60;
   seconds=pFormatContext->duration%3600%60;

   qDebug() <<"视频名字 ：" << pFormatContext->filename;
   qDebug() <<"视频时间 :  "<< hour<<":"<<mintue<<":"<<seconds;
   qDebug() <<"视频的个数：" <<pFormatContext->nb_streams;
   AVInputFormat * avinput =pFormatContext->iformat;

   qDebug() <<"视频格式名称："<<avinput->name;
   qDebug() <<"视频格式长名称："<<avinput->long_name;
   qDebug() <<"封装格式扩展名："<<avinput->extensions;
   qDebug() <<"封装格式ID："<<avinput->raw_codec_id;
   ret = avformat_find_stream_info(pFormatContext,NULL);
   if(ret < 0){
       qDebug() <<"avformat_find_stream_info,err code :" << ret;
       return -1;
   }

   /*
       查找视频流 索引
   */
   videoindex=-1;
   for(int i=0;i<pFormatContext->nb_streams;++i){
       if(pFormatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO){
           videoindex=i;
           break;
       }
   }

   if(videoindex==-1){
       qDebug() <<"Not Find Video Type\n";
       return -1;
   }
   qDebug() <<"AVStream视频元数据：";
   qDebug() <<"---------------------------------------------";
   AVDictionaryEntry *dict = NULL;
   while (dict = av_dict_get(pFormatContext->streams[videoindex]->metadata, "", dict, AV_DICT_IGNORE_SUFFIX))
   {
       qDebug() << "key->value :"<< dict->key <<":"<< dict->value;
   }
   qDebug() <<"---------------------------------------------";
   //获取pcodeccontext
    pCodecContext = avcodec_alloc_context3(NULL);
    avcodec_parameters_to_context(pCodecContext,pFormatContext->streams[videoindex]->codecpar);

    // pCodecContext = pFormatContext->streams[videoindex]->codec;

  // pCodec = (AVCodec *)pCodecContext->codec;


     pCodec = avcodec_find_decoder(pCodecContext->codec_id);
   if(pCodec == NULL){
       qDebug() <<"avcodec_find_decoder Failed\n";
       return -1;
   }

   ret = avcodec_open2(pCodecContext,pCodec,NULL);
   if(ret < 0){
       qDebug() << "avcodec_open2 Fail\n";
       return -1;
   }

    pFrame =(AVFrame *) av_frame_alloc(); // 填充avframe结构体
    if(pFrame == NULL){
        qDebug() << "av_frame_alloc";
        return -1;
    }
    qDebug() <<"av_frame_alloc()";
   // av_init_packet(pPacket);
    pPacket =(AVPacket *) av_malloc(sizeof(AVPacket));
    if(pPacket == NULL){
        qDebug() << "av_malloc";
        return -1;
    }

    pPic=(AVPicture *)av_frame_alloc();
    avpicture_alloc(pPic,AV_PIX_FMT_RGB24,pCodecContext->width,pCodecContext->height);//设置avpicture结构体s

    if(pPic == NULL){
        qDebug() << "av_frame_alloc";
        return -1;
    }
   //pSwsContext = sws_getContext()
   pSwsContext = sws_getContext(pCodecContext->width,pCodecContext->height,AV_PIX_FMT_YUV420P,pCodecContext->width,pCodecContext->height,AV_PIX_FMT_RGB24,SWS_BICUBIC,0,0,0);
    qDebug()<<"play init over\n";
    return 0;
}

#if 1
void VideoDecThread::run()
{
    int got_picture;
    int err;
    qDebug() <<"DecodeThread Start !!!!\n";
    while(1)
    {
        QThread::msleep(35);
        if(av_read_frame(pFormatContext,pPacket) < 0){
            qDebug() << "av_read_frame Failed\n";
            break;
        }
       if(pPacket->stream_index == videoindex && avcodec_is_open(pCodecContext))
       {

           #if 1
             if((err = avcodec_decode_video2(pCodecContext,pFrame,&got_picture,pPacket))< 0)
             {
                  qDebug() <<"avcodec_decode_video2"<< err;
                   break;
             }
           #endif
#if 1
             if(got_picture){
                qDebug() <<"width :" << pFrame->width;
                qDebug() <<"height:" << pFrame->height;
                qDebug() <<"";
                qDebug() <<"";
                sws_scale(pSwsContext,(const uint8_t*  const*)pFrame->data,pFrame->linesize,0,pCodecContext->height,pPic->data,pPic->linesize);
                QImage Img((uchar *)pPic->data[0],pCodecContext->width,pCodecContext->height,QImage::Format_RGB888);

                 if(!Img.isNull())
                {
                     Img.save("pic.png","PNG",60);
                      emit display(Img);
                 }else{
                         qDebug() <<"Image is Null\n";
                 }
             }else{


             }
#endif
        }
    }
end:
     qDebug() <<"decode Thread exit !!!";
//     emit decThreadexit();
}
#else
void VideoDecThread::run()
{
    int got_pictrue;
    int err;

    qDebug() <<"Decode Start";

    while(1)
    {

        if((err = av_read_frame(pFormatContext,pPacket)) < 0)
        {

            qDebug() << "av_read_frame error  or end of file";
            break;
        }

        if((err = avcodec_decode_video2(pCodecContext,pFrame,&got_pictrue,pPacket)) <= 0)
        {
            qDebug() <<"avcodec_decode_video2 Failed";
            break;
        }

        SwsContext *convertCtx = sws_getContext(pCodecContext->width,pCodecContext->height,AV_PIX_FMT_YUV420P,pCodecContext->width,pCodecContext->height,AV_PIX_FMT_RGB24,SWS_BICUBIC,0,0,0);
            sws_scale(convertCtx,(const uint8_t*  const*)pFrame->data,pFrame->linesize,0,pCodecContext->height,pPic->data,pPic->linesize);
          QImage Img((uchar *)pPic->data[0],pCodecContext->width,pCodecContext->height,QImage::Format_RGB32);
          if(!Img.isNull())
          emit display(Img);

    }

    qDebug() <<"Decode end";

}
#endif
