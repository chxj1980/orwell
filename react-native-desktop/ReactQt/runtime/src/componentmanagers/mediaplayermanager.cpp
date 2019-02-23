
/**
 * Copyright (c) 2017-present, Status Research and Development GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#include <QQmlComponent>
#include <QQmlProperty>
#include <QQuickItem>
#include <QString>
#include <QVariant>

#include <QDebug>

#include "attachedproperties.h"
#include "bridge.h"
#include "layout/flexbox.h"
#include "propertyhandler.h"
#include "utilities.h"
#include "mediaplayermanager.h"
#include "OpenGlVideoQtQuick.h"

class MediaPlayerManagerPrivate {};

MediaPlayerManager::MediaPlayerManager(QObject* parent) : ViewManager(parent), d_ptr(new MediaPlayerManagerPrivate) {}

MediaPlayerManager::~MediaPlayerManager() {}

ViewManager* MediaPlayerManager::viewManager() {
    return this;
}

QString MediaPlayerManager::moduleName() {
    return "RCTMediaPlayerManager";
}

QString MediaPlayerManager::qmlComponentFile(const QVariantMap& properties) const {
    return "qrc:/qml/ReactSwitch.qml";
}

void MediaPlayerManager::configureView(QQuickItem* view) const {
    ViewManager::configureView(view);
    view->setProperty("MediaPlayerManager", QVariant::fromValue((QObject*)this));
    updateMeasureFunction(view);
}

void MediaPlayerManager::updateMeasureFunction(QQuickItem* view) const {
    Flexbox* flexbox = Flexbox::findFlexbox(view);
    if (!flexbox) {
        return;
    }

    flexbox->setMeasureFunction([=](YGNodeRef, float, YGMeasureMode, float, YGMeasureMode) {
        float w = view->width();
        float h = view->height();
        return YGSize{w, h};
    });
}

QStringList MediaPlayerManager::customDirectEventTypes() {
    return QStringList{};
}

bool MediaPlayerManager::shouldLayout() const {
    return true;
}

void MediaPlayerManager::sendValueChangeToJs(QQuickItem* switchItem, bool value) {
    if (!switchItem)
        return;
    //UNCOMMENT THAT LATER
    //notifyJsAboutEvent(tag(switchItem), EVENT_ON_CHANGE, QVariantMap{{"value", value}});
}

QQuickItem* MediaPlayerManager::createView(const QVariantMap& properties) {
    QQuickItem* item = new OpenGlVideoQtQuick();
    return item;
    //RETURN A CUSTOM VIEW HERE!!!!!! It is the QQuickItem that displays video
    /*
    QString qmlSrc = qmlComponentFile(properties);

    if (!m_components.contains(qmlSrc)) {
        m_components[qmlSrc] = createComponentFromSourceFile(m_bridge->qmlEngine(), QUrl(qmlSrc));
    }

    QmlComponentPtr component = m_components[qmlSrc];
    QQuickItem* item = createQMLItemFromComponent(component);
    if (item == nullptr) {
        qCCritical(VIEWMANAGER) << QString("Can't create QML item for component %1").arg(qmlSrc);
    } else {
        qCDebug(VIEWMANAGER) << "Created view: " << item << ". Source QML file: " << qmlSrc
                             << ". Props keys: " << properties.keys();
    }
    */
}

#include "mediaplayermanager.moc"

