/**
 * Copyright (c) 2017-present, Status Research and Development GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#include "gluemanager.h"
#include "bridge.h"
#include "eventdispatcher.h"

#include <QGuiApplication>
#include <QDebug>

//Q_LOGGING_CATEGORY(NOTIFICATION, "RCTNotification")


namespace {
struct RegisterQMLMetaType {
  RegisterQMLMetaType() { qRegisterMetaType<GlueManager *>(); }
} registerMetaType;

const QString NewMessageAlert = QStringLiteral("NewMessage");
} // namespace

class GlueManagerPrivate {
public:
  Bridge *bridge = nullptr;
};

GlueManager::GlueManager(QObject *parent)
    : QObject(parent), d_ptr(new GlueManagerPrivate) {
}

GlueManager::~GlueManager() {
  
}

void GlueManager::setBridge(Bridge *bridge) {
  Q_D(GlueManager);
  d->bridge = bridge;
}

QString GlueManager::moduleName() { return "GlueManager"; }

QList<ModuleMethod *> GlueManager::methodsToExport() {
  return QList<ModuleMethod *>{};
}

QVariantMap GlueManager::constantsToExport() { return QVariantMap(); }

void GlueManager::displayNotification(QString title, QString body, bool prioritary) {
  Q_D(GlueManager);
  qCDebug(NOTIFICATION) << "::displayNotification";

  if (m_appHasFocus) {
      qCDebug(NOTIFICATION) << "Not displaying notification since an application window is active";
      return;
  }

  Snore::Notification notification(
      d_ptr->snoreApp, d_ptr->snoreApp.alerts()[NewMessageAlert], title,
      body, Snore::Icon::defaultIcon(),
      prioritary ? Snore::Notification::Prioritys::High : Snore::Notification::Prioritys::Normal);
  Snore::SnoreCore::instance().broadcastNotification(notification);
}

void GlueManager::setDockBadgeLabel(const QString label) {
  Snore::SnoreCore::instance().setDockBadgeLabel(label);
}
