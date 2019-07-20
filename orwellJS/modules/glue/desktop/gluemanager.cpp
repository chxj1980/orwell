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
#include "MediaStream.h"
#include "FfmpegDecoder.h"
#include <boost/thread/thread.hpp>
#include <iostream>

//Q_LOGGING_CATEGORY(NOTIFICATION, "RCTNotification")


namespace {
  struct RegisterQMLMetaType {
    RegisterQMLMetaType() { qRegisterMetaType<GlueManager *>(); }
  } registerMetaType;
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

//Register our RTSP stream in the Glue singleton so it can be used in other parts of cpp code (mainly in the video player)
void GlueManager::addRTSPStream(QString key, QString uri) {
    GlueObject glueObject;
    glueObject.mediaStream = std::make_shared<MediaStream>(uri.toStdString());
    FfmpegDecoder* ffmpegDecoder = new FfmpegDecoder();
    //For debug purposes only
    //ffmpegDecoder.uri = this->uri;
    glueObject.mediaStream->setDecoder(ffmpegDecoder);
    glueObject.mediaThread = std::make_shared<boost::thread>(&MediaStream::run, glueObject.mediaStream);
    Glue::instance()->add(key, glueObject);
}