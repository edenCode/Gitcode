#include "video.h"
#include "ui_video.h"

Video::Video(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Video)
{
    ui->setupUi(this);

    qDebug() << "Video::Video";
    setWindowTitle(QString("MPlayer"));
    // decThread = new VideoDecThread(this);
  //  connect(decThread,SIGNAL(display(QImage)),this,SLOT(showVideo(QImage)));
    decThread = new VideoDecThread(this);
    connect(decThread,SIGNAL(display(QImage)),this,SLOT(showVideo(QImage)));
   // connect(decThread,SIGNAL(decThreadexit()),this,SLOT(showVideo(QImage)));
}

Video::~Video()
{
    delete decThread;
    delete ui;
}


void Video::on_play_clicked()
{
    qDebug() <<"play clicked\n";
    decThread->start();
    ui->play->setVisible(false);
    ui->open->setVisible(false);
}
void Video::showVideo(QImage img)
{

    QPixmap pix;
    qDebug() << "show Image";
    pix = pix.fromImage(img);//.scaledToWidth(ui->VideoShow->width());
    ui->label->setPixmap(pix);

}

void Video::on_open_clicked()
{
     FileName = QFileDialog::getOpenFileName(this,"open",QDir::currentPath(),"Video(*.mp4);;All(*.*)");
    qDebug() << FileName;
//    QByteArray byte = FileName.toLocal8Bit();
//    qDebug() <<"data :" << byte.data()<<"length :" << byte.length();
    decThread->setPath(FileName);

    // OVideoDec->FileName = byte.data();
    //qDebug() <<OVideoDec->FileName;
}

// 按键控制
#if 0
void Video::onFocus()
{

}
#endif
