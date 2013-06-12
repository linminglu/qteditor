#include <QtWidgets>
#include <QTextCodec>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

#ifdef Q_WS_MAC
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
#endif
#ifdef Q_WS_WIN
    //    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
        QTextCodec::setCodecForTr(QTextCodec::codecForName("GBK"));
#endif



    MainWindow w;
    w.showMaximized();
//    w.show();

    return a.exec();
}
