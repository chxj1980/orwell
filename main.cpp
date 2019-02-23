#include <iostream>
#include "MediaStream.h"
#include "FfmpegDecoder.h"
#include "VideoConsumer.h"
#include "VideoSurface.h"
#include "mainwindow.h"
#include <QApplication>
#include <QMediaPlayer>
#include <QPushButton>
#include <boost/thread.hpp>
//#include <boost/chrono.hpp>

#include <QGuiApplication>

#include <QtQuick/QQuickView>

#include "OpenGlVideoQtQuick.h"

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);
    QCoreApplication::addLibraryPath("./");
    qmlRegisterType<OpenGlVideoQtQuick>("OpenGLComponents", 1, 0, "OpenGlVideoQtQuick");

    QQuickView view;
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.setSource(QUrl("qrc:/main.qml"));
    view.show();

    return app.exec();
}
    /*
    QApplication a(argc, argv);
    std::cout << "Orwell" << std::endl;
    //setWindowTitle("Orwell"); 

    //std::vector<MediaStream> cameras;
    //cameras.push_back(MediaStream(newCameraUtil("192.168.0.103")));
    //std::vector<MediaStream>::iterator it;

    //Start all cameras into threads
    //
    //for(it = cameras.begin(); it != cameras.end(); it++)    {
    //    boost::thread mediaThread(&MediaStream::run, it);
    //}
    

   

    MainWindow w;
    w.setFixedSize(1280,720);
    w.show();
    //boost::thread mediaThread(&MainWindow::run, camera1);
    

    return a.exec();
    */
