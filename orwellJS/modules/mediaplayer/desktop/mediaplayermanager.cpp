#include "mediaplayermanager.h"
#include "bridge.h"
#include "eventdispatcher.h"

#include <QCoreApplication>
#include <QDebug>
#include <QDesktopServices>
#include <QUrl>
#include <QFileOpenEvent>
#include "OpenGlBufferQtQuick.h"
#include "OpenGlVideoQtQuick.h"

#include <iostream>

namespace {
struct RegisterQMLMetaType {
  RegisterQMLMetaType() { qRegisterMetaType<MediaPlayerManager *>(); }
} registerMetaType;
} // namespace

class MediaPlayerManagerPrivate {
public:
  Bridge *bridge = nullptr;
};

MediaPlayerManager::MediaPlayerManager(QObject *parent)
    : ViewManager(parent), d_ptr(new MediaPlayerManagerPrivate) {

}

ViewManager* MediaPlayerManager::viewManager() {
    return this;
}

MediaPlayerManager::~MediaPlayerManager() {
}


QString MediaPlayerManager::moduleName() { return "RCTMediaPlayerManager"; }

QQuickItem* MediaPlayerManager::createView(const QVariantMap& properties) {
    OpenGlBufferItem* item = new OpenGlBufferItem;
    //OpenGlVideoQtQuick* item = new OpenGlVideoQtQuick();

    item->setUri("rtsp://admin:19929394@192.168.0.103:10554/tcp/av0_0");
    //item->render();
    item->setWidth(640);
    item->setHeight(360);
    std::cout << "returning custom item" << std::endl;
    return item;
}


