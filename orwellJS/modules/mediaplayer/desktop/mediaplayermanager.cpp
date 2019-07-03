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


QString MediaPlayerManager::moduleName() { return "RCTMediaPlayerManager"; }

void MediaPlayerManager::test(QString text, const ModuleInterface::ListArgumentBlock& resolve, const ModuleInterface::ListArgumentBlock& reject) {
  Q_D(MediaPlayerManager);
  std::cout << text.toStdString() << std::endl;
}

QQuickItem* MediaPlayerManager::createView(const QVariantMap& properties) {
    //OpenGlBufferItem* item = new OpenGlBufferItem;
    //for(QVariantMap::const_iterator iter = properties.begin(); iter != properties.end(); ++iter) {
      //std::cout << iter.key().toStdString() <<std::endl;
    //}

    std::cout << "width:  (((((( " << properties["width"].toInt() << std::endl;
    std::cout << "height: (((((( " << properties["height"].toInt() << std::endl;
    //OpenGlVideoQtQuick* item = new OpenGlVideoQtQuick();
    OpenGlMaterialQQuickItem* item = new OpenGlMaterialQQuickItem();
    //OpenGlVideoQtQuick2* item = new OpenGlVideoQtQuick2();

    //item->setUri("rtsp://admin:19929394@192.168.1.178:10554/tcp/av0_0");
    //item->render();
    item->setWidth(640);
    item->setHeight(360);
    std::cout << "returning custom item" << std::endl;
    return item;
}


