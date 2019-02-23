
/**
 * Copyright (c) 2017-present, Status Research and Development GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#ifndef OrwellMediaPlayerManager_H
#define OrwellMediaPlayerManager_H

#include "orwellmediaplayer.h"
#include "moduleinterface.h"
#include <QQuickItem>

class PropertyHandler;
class OrwellMediaPlayerManagerPrivate;
class OrwellMediaPlayerManager : public ViewManager {
    Q_OBJECT
    Q_INTERFACES(ModuleInterface)
    Q_DECLARE_PRIVATE(OrwellMediaPlayerManager)

public:
    OrwellMediaPlayerManager(QObject* parent = 0);
    virtual ~OrwellMediaPlayerManager();

    virtual ViewManager* viewManager();
    virtual QString moduleName();

    virtual QStringList customDirectEventTypes();
    virtual bool shouldLayout() const;

protected:
    QQuickItem* createView(const QVariantMap& properties);

private:
    virtual QString qmlComponentFile(const QVariantMap& properties) const;
    virtual void configureView(QQuickItem* view) const ;

private:
    QScopedPointer<OrwellMediaPlayerManagerPrivate> d_ptr;
};

#endif // OrwellMediaPlayerManager_H

