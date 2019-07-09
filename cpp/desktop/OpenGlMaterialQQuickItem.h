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
#include "VideoReceiver.h"
#include "Glue.h"
#include "componentmanagers/viewmanager.h"

class Node;
//class OpenGlMaterialQQuickItem: public QQuickItem
class OpenGlMaterialQQuickItem: public ReactItem
{
    Q_OBJECT
    Q_PROPERTY(QString uri WRITE setUri READ getUri)// NOTIFY uriChanged)
    Q_PROPERTY(QString id WRITE setId READ getId)// NOTIFY uriChanged)
    Q_PROPERTY(QString p_id WRITE setPId READ getPId)// NOTIFY uriChanged)
    Q_PROPERTY(QString p_uri WRITE setPUri READ getPUri);
    Q_PROPERTY(qreal p_height WRITE _setHeight READ getPHeight);
    Q_PROPERTY(qreal p_width WRITE _setWidth READ getPWidth);
    //Q_PROPERTY(Glue* glue READ getGlue WRITE setGlue)

    public:
        QString id;
        QString p_id;

        //Flexbox flexbox(this);
        //flexbox.setControl(this);
        //flexbox.setViewManager();
    

        QSGNode *updatePaintNode(QSGNode *node, UpdatePaintNodeData *) override;

        OpenGlMaterialQQuickItem()
        {
            flexbox = new Flexbox();
            flexbox->setControl(this);
            flexbox->setViewManager(this->viewManager);
            setFlag(ItemHasContents, true);
        }

        ~OpenGlMaterialQQuickItem()
        {
            delete node;
            node = nullptr;
        }

        void componentComplete() {
            QQuickItem::componentComplete();
        }

        void setUri(const QString &a) {
            uri = a;
        }

        void setPUri(const QString &a) {
            p_uri = a;
        }

        void setId(const QString &a) {
            id = a;
            std::cout << "OpenGlMaterialQQuickItem created with id " << id.toStdString() << std::endl;
        }

        void setPId(const QString &a) {
            p_id = a;
            std::cout << "OpenGlMaterialQQuickItem created with p_id " << p_id.toStdString() << std::endl;
        }

        QString getUri() {
            return this->uri;
        }

        QString getPUri() {
            return this->p_uri;
        }

        QString getId() {
            return this->id;
        }

        QString getPId() {
            return this->p_id;
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
/*
        void setGlue(Glue* glue) {
            this->glue = glue;
        }

        Glue* getGlue() {
            return this->glue;
        }
*/

    protected:
        void paint(QPainter* painter){}

    private:
        QString uri;
        QString p_uri;
        qreal p_width;
        qreal p_height;
        Flexbox* flexbox;
        ViewManager* viewManager;
        Node *node;
        //Glue glue;
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