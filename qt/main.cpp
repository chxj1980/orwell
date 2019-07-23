#include <iostream>
#include "MediaStream.h"
#include "FfmpegDecoder.h"
#include <QApplication>
#include <QMediaPlayer>
#include <QPushButton>
#include <boost/thread.hpp>
//#include <boost/chrono.hpp>

#include <QGuiApplication>

#include <QtQuick/QQuickView>

#include "OpenGlBufferQtQuick.h"
#include "OpenGlVideoQtQuick.h"
#include "OpenGlVideoQtQuick2.h"
#include "OpenGlMaterialQQuickItem.h"
#include <QObject>
#include <QtQml>
#include "Glue.h"
#include <memory>


int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);
    QCoreApplication::addLibraryPath("./");
    qmlRegisterType<OpenGlVideoQtQuick>("OpenGlVideoQtQuick", 1, 0, "OpenGlVideoQtQuick");
    qmlRegisterType<OpenGlVideoQtQuick2>("OpenGlVideoQtQuick2", 1, 0, "OpenGlVideoQtQuick2");
    qmlRegisterType<OpenGlBufferItem>("OpenGlBufferQtQuick", 1, 0, "OpenGlBufferQtQuick");
    qmlRegisterType<OpenGlMaterialQQuickItem>("OpenGlMaterialQQuickItem", 1, 0, "OpenGlMaterialQQuickItem");

    QQuickView view;
    //Glue* glue = new Glue(&view);
    //Glue glue;
    //Glue::instance()->streamList.insert("cam1", QVariant("hello"));
    GlueObject glueObject;
    glueObject.mediaStream = std::make_shared<MediaStream>("rtsp://admin:19929394@192.168.0.101:10554/tcp/av0_1");
    FfmpegDecoder* ffmpegDecoder = new FfmpegDecoder();
    //ffmpegDecoder.setVideoReceiver();
    //For debug purposes only
    //ffmpegDecoder.uri = this->uri;
    glueObject.mediaStream->setDecoder(ffmpegDecoder);
    glueObject.mediaThread = std::make_shared<boost::thread>(&MediaStream::run, glueObject.mediaStream);
    Glue::instance()->add("cam1", glueObject);
    //glue->list()->insert(QString("test"), QVariant("hi"));
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    //view.rootContext()->setContextProperty("Glue", glue);
    //view.setProperty("Glue", glue);
    view.setSource(QUrl("qrc:/main.qml"));
    view.setColor(QColor(0,0,0,255));
    view.show();

    return app.exec();
}

