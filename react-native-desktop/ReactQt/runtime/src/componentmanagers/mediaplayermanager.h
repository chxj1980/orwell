
/**
 * Copyright (c) 2017-present, Status Research and Development GmbH.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 *
 */

#ifndef MediaPlayerManager_H
#define MediaPlayerManager_H

#include "viewmanager.h"

class PropertyHandler;
class MediaPlayerManagerPrivate;
class MediaPlayerManager : public ViewManager {
    Q_OBJECT
    Q_INTERFACES(ModuleInterface)
    Q_DECLARE_PRIVATE(MediaPlayerManager)

public:
    MediaPlayerManager(QObject* parent = 0);
    virtual ~MediaPlayerManager();

    virtual ViewManager* viewManager() override;
    virtual QString moduleName() override;

    virtual QStringList customDirectEventTypes() override;
    virtual bool shouldLayout() const override;

public slots:
    void sendValueChangeToJs(QQuickItem* picker, bool value);
protected:
    QQuickItem* createView(const QVariantMap& properties);

private:
    virtual QString qmlComponentFile(const QVariantMap& properties) const override;
    virtual void configureView(QQuickItem* view) const override;
    void updateMeasureFunction(QQuickItem* view) const;

private:
    QScopedPointer<MediaPlayerManagerPrivate> d_ptr;
};

#endif // MediaPlayerManager_H

