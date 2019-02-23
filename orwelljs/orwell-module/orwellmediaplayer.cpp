#include "OrwellMediaPlayer.h"
#include "bridge.h"

#include <QCoreApplication>
#include <QDebug>
#include <QMenu>
#include <QCursor>

Q_LOGGING_CATEGORY(OrwellMediaPlayer, "OrwellMediaPlayer")

namespace {
  struct RegisterQMLMetaType {
    RegisterQMLMetaType() { qRegisterMetaType<OrwellMediaPlayer *>(); }
  } registerMetaType;
} // namespace

class OrwellMediaPlayerPrivate {
  public:
    Bridge *bridge = nullptr;
};

OrwellMediaPlayer::OrwellMediaPlayer(QObject *parent)
    : QObject(parent), d_ptr(new OrwellMediaPlayerPrivate) {
}

OrwellMediaPlayer::~OrwellMediaPlayer() {
}

void OrwellMediaPlayer::setBridge(Bridge *bridge) {
  Q_D(OrwellMediaPlayer);

  d->bridge = bridge;
}

ViewManager* OrwellMediaPlayer::viewManager() {

  //return ;
}

QString OrwellMediaPlayer::moduleName() { return "OrwellMediaPlayerManager"; }

QList<ModuleMethod *> OrwellMediaPlayer::methodsToExport() {
  return QList<ModuleMethod *>{};
}

QVariantMap OrwellMediaPlayer::constantsToExport() { return QVariantMap(); }

void OrwellMediaPlayer::show(const QStringList& items, double callback) {
  Q_D(OrwellMediaPlayer);
  d_ptr->createMenu(items, callback);

}


