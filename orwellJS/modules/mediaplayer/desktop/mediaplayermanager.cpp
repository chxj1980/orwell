#include "mediaplayermanager.h"
#include "bridge.h"
#include "eventdispatcher.h"

#include <QCoreApplication>
#include <QDebug>
#include <QDesktopServices>
#include <QUrl>
#include <QFileOpenEvent>
#include "OpenGlBufferItem.h"
#include "OpenGlVideoQtQuick.h"
#include "OpenGlVideoQtQuick2.h"
#include "OpenGlMaterialQQuickItem.h"

#include <QScreen> 

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

QList<ModuleMethod *> MediaPlayerManager::methodsToExport() {
  return QList<ModuleMethod *>{};
}

void MediaPlayerManager::setBridge(Bridge *bridge) {
  Q_D(MediaPlayerManager);

  d->bridge = bridge;
}


QString MediaPlayerManager::moduleName() { return "OrwellMediaPlayer"; }

void MediaPlayerManager::test(QString text, const ModuleInterface::ListArgumentBlock& resolve, const ModuleInterface::ListArgumentBlock& reject) {
  Q_D(MediaPlayerManager);
  std::cout << text.toStdString() << std::endl;
}

QQuickItem* MediaPlayerManager::createView(const QVariantMap& properties) {
    //qDebug() << properties;
    OpenGlBufferItem* item = new OpenGlBufferItem();


    //OpenGlVideoQtQuick* item = new OpenGlVideoQtQuick();
    //OpenGlMaterialQQuickItem* item = new OpenGlMaterialQQuickItem();
    //OpenGlVideoQtQuick2* item = new OpenGlVideoQtQuick2();

    //item->setUri("rtsp://admin:19929394@192.168.1.178:10554/tcp/av0_0");
    //item->render();
    item->setWidth(properties["width"].toInt());
    item->setHeight(properties["height"].toInt());
    return item;
}


