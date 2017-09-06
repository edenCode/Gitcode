#include "video.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    qDebug() << "Application start";
    QApplication a(argc, argv);
    qDebug() << "Application start";
    Video w;
    w.show();
    qDebug() << "w.show();";
    return a.exec();
}
