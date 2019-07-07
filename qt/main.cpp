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

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);
    QCoreApplication::addLibraryPath("./");
    qmlRegisterType<OpenGlVideoQtQuick>("OpenGlVideoQtQuick", 1, 0, "OpenGlVideoQtQuick");
    qmlRegisterType<OpenGlVideoQtQuick2>("OpenGlVideoQtQuick2", 1, 0, "OpenGlVideoQtQuick2");
    qmlRegisterType<OpenGlBufferItem>("OpenGlBufferQtQuick", 1, 0, "OpenGlBufferQtQuick");
    qmlRegisterType<OpenGlMaterialQQuickItem>("OpenGlMaterialQQuickItem", 1, 0, "OpenGlMaterialQQuickItem");

    QQuickView view;
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.setSource(QUrl("qrc:/main.qml"));
    //view.setColor(QColor(0,0,0,255));
    view.show();

    return app.exec();
}
