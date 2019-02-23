#ifndef OrwellMediaPlayer_H
#define OrwellMediaPlayer_H

#include "moduleinterface.h"

#include <QLoggingCategory>
#include <QVariantMap>

Q_DECLARE_LOGGING_CATEGORY(MENU)

class OrwellMediaPlayerPrivate;
class OrwellMediaPlayer : public QObject, public ModuleInterface {
    Q_OBJECT
    Q_INTERFACES(ModuleInterface)

    Q_DECLARE_PRIVATE(OrwellMediaPlayer)

public:
    Q_INVOKABLE OrwellMediaPlayer(QObject* parent = 0);
    virtual ~OrwellMediaPlayer();

    void setBridge(Bridge* bridge) override;

    QString moduleName() override;
    QList<ModuleMethod*> methodsToExport() override;
    QVariantMap constantsToExport() override;
    ViewManager* viewManager();

private:
    QScopedPointer<OrwellMediaPlayerPrivate> d_ptr;
};



#endif // OrwellMediaPlayer_H
