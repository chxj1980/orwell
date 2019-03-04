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
#include "OpenGlBufferQtQuick.h"


int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);
    QCoreApplication::addLibraryPath("./");
    qmlRegisterType<OpenGlBufferItem>("OpenGLComponents", 1, 0, "OpenGlBufferItem");

    QQuickView view;
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.setSource(QUrl("qrc:/main.qml"));
    view.show();

    return app.exec();
}
