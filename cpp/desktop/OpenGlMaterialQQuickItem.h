#ifndef OpenGlMaterialQQuickItem_H
#define OpenGlMaterialQQuickItem_H

#include <QtQuick/QQuickItem>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLFunctions>
#include <QtQuick/qquickwindow.h>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLContext>
#include <QString>
#include <iostream>
#include "MediaStream.h"
#include <boost/thread.hpp>
#include <QTimer>
#include <QMatrix4x4>
#include <QQmlListProperty>
#include <QQuickPaintedItem>
#include <QSGSimpleMaterial>
#include "reactitem.h"
#include "flexbox.h"


//class OpenGlMaterialQQuickItem: public QQuickItem
class OpenGlMaterialQQuickItem: public ReactItem
{
    Q_OBJECT
    Q_PROPERTY(QString uri WRITE setUri READ getUri)// NOTIFY uriChanged)
    Q_PROPERTY(QString p_uri WRITE setPUri READ getPUri);
    Q_PROPERTY(qreal p_height WRITE _setHeight READ getPHeight);
    Q_PROPERTY(qreal p_width WRITE _setWidth READ getPWidth);


    public:
    
        //Flexbox flexbox(this);
        //flexbox.setControl(this);
        //flexbox.setViewManager();
    

        QSGNode *updatePaintNode(QSGNode *node, UpdatePaintNodeData *) override;

        OpenGlMaterialQQuickItem()
        {
            setFlag(ItemHasContents, true);
            //QVariant  parent().property("Glue");
            //std::cout << "indexOfProperty: " << parent()->metaObject()->indexOfProperty("Glue") << std::endl;
        }

        void setUri(const QString &a) {
            uri = a;
            std::cout << "OpenGlMaterialQQuickItem created with uri " << uri.toStdString() << std::endl;
        }

        void setPUri(const QString &a) {
            p_uri = a;
            std::cout << "OpenGlMaterialQQuickItem created with uri " << p_uri.toStdString() << std::endl;
        }

        QString getUri() {
            return this->uri;
        }

        QString getPUri() {
            return this->p_uri;
        }

        qreal getPHeight() {
            return this->p_height;
        }

        qreal getPWidth() {
            return this->p_width;
        }

        void _setWidth(qreal width) {
            p_width = width;
        }
        void _setHeight(qreal height) {
            p_height = height;
        }

    protected:
        void paint(QPainter* painter){}

    
    private:
        QString uri;
        QString p_uri;
        qreal p_width;
        qreal p_height;
    /*
    private slots:
        void parentWidthChanged(){
            setWidth(parentItem()->width());
        }
        void parentHeightChanged(){
            setHeight(parentItem()->height());
        }
    */
        
};
#endif  //OpenGlMaterialQQuickItem_H