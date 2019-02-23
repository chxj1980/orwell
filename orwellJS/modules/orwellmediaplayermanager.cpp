
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
#include "OrwellMediaPlayermanager.h"
#include "OpenGlVideoQtQuick.h"

class OrwellMediaPlayerManagerPrivate {};

OrwellMediaPlayerManager::OrwellMediaPlayerManager(QObject* parent) : 
    ViewManager(parent), d_ptr(new OrwellMediaPlayerManagerPrivate) {}

OrwellMediaPlayerManager::~OrwellMediaPlayerManager() {}

ViewManager* OrwellMediaPlayerManager::viewManager() {
    return this;
}

QString OrwellMediaPlayerManager::moduleName() {
    return "OrwellMediaPlayer";
}

QString OrwellMediaPlayerManager::qmlComponentFile(const QVariantMap& properties) const {
    return "qrc:/qml/ReactSwitch.qml";
}

void OrwellMediaPlayerManager::configureView(QQuickItem* view) const {
    ViewManager::configureView(view);
    view->setProperty("OrwellMediaPlayerManager", QVariant::fromValue((QObject*)this));
    updateMeasureFunction(view);
}

QStringList OrwellMediaPlayerManager::customDirectEventTypes() {
    return QStringList{};
}

bool OrwellMediaPlayerManager::shouldLayout() const {
    return true;
}

QQuickItem* OrwellMediaPlayerManager::createView(const QVariantMap& properties) {
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

#include "orwellmediaplayermanager.moc"

