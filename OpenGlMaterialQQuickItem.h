#ifndef OpenGlMaterialQQuickItem_H
#define OpenGlMaterialQQuickItem_H

#include <boost/thread.hpp>
#include "MediaStream.h"
#include "reactitem.h"
#include <QtQuick/QQuickItem>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLFunctions>
#include <QtQuick/qquickwindow.h>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLContext>
#include <QString>
#include <iostream>
#include <QTimer>
#include <QMatrix4x4>
#include <QQmlListProperty>
#include <QQuickPaintedItem>
#include <QObject>


//class OpenGlMaterialQQuickItem: public QQuickItem
class OpenGlMaterialQQuickItem: public ReactItem
{
    Q_OBJECT

    public:
        Q_PROPERTY(QString uri WRITE setUri)// NOTIFY uriChanged)
        Q_PROPERTY(QString p_uri WRITE setPUri);
        Q_PROPERTY(qreal p_height WRITE _setHeight);
        Q_PROPERTY(qreal p_width WRITE _setWidth);
        std::string uri;

        QSGNode *updatePaintNode(QSGNode *node, UpdatePaintNodeData *) override;

        OpenGlMaterialQQuickItem()
        {
            setFlag(ItemHasContents, true);
        }

        void setUri(const QString &a) {
            uri = a.toStdString();
            std::cout << "OpenGlMaterialQQuickItem created with uri " << uri << std::endl;
        }
        
};
#endif 