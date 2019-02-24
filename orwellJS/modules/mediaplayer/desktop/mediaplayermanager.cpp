#include "mediaplayermanager.h"
#include "bridge.h"
#include "eventdispatcher.h"

#include <QCoreApplication>
#include <QDebug>
#include <QDesktopServices>
#include <QUrl>
#include <QFileOpenEvent>
#include "OpenGlVideoQtQuick.h"

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
    QQuickItem* qQuickItem = new OpenGlVideoQtQuick();
    qQuickItem->setWidth(640);
    qQuickItem->setHeight(480);
    return qQuickItem;
}


