#ifndef OpenGlVideoQtQuick_H
#define OpenGlVideoQtQuick_H

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
#include "reactitem.h"
//#include "reactitem.h"


class OpenGlVideoQtQuickRenderer : public QObject, protected QOpenGLFunctions, public FrameUpdater
{
    Q_OBJECT

public:
    OpenGlVideoQtQuickRenderer() {        
    }
    ~OpenGlVideoQtQuickRenderer();
    void updateData(unsigned char**data, int frameWidth, int frameHeight);
    void setWindow(QQuickWindow *window) { m_window = window; }
    void setDimensions(int x, int y, int width, int height) {
        this->x = x;
        this->y = y;
        this->width = width;
        this->height = height;
    }
    QMatrix4x4 qQuickVideoMatrix;

public slots:
    void render();

private:
    QOpenGLShaderProgram* program;
    QQuickWindow *m_window;
    GLuint unis[3] = {0};
    GLuint texs[3] = {0};
    unsigned char *datas[3] = {0};
    bool firstRender = true;
    int width = 0;
    int height = 0;
    int x = 0;
    int y = 0;
};

//class OpenGlVideoQtQuick : public QQuickItem
//class OpenGlVideoQtQuick : public QQuickPaintedItem
class OpenGlVideoQtQuick : public ReactItem
{
    Q_OBJECT
    Q_PROPERTY(QString uri WRITE setUri)// NOTIFY uriChanged)
protected:
    void paint(QPainter* painter){};
    QSGNode * updatePaintNode(QSGNode *oldNode, QQuickItem::UpdatePaintNodeData *updatePaintNodeData) {}// https://stackoverflow.com/questions/56658299/opengl-strange-behaviour-in-qt-when-inheriting-from-qquickitem-versus-qquickpain

public:
    OpenGlVideoQtQuick();
    OpenGlVideoQtQuick(std::string uri);
    std::string uri;
    void setUri(const QString &a) {
        uri = a.toStdString();
    }

    

signals:
    void tChanged();

public slots:
    void sync();
    void cleanup();
    void update();//Updates the window


private slots:
    void handleWindowChanged(QQuickWindow *win);

private:
    OpenGlVideoQtQuickRenderer *openGlVideoQtQuickRenderer;
};

/*
class OpenGlHelper: public FrameUpdater {
    public:
        OpenGlHelper(OpenGlVideoQtQuick* openGlVideoQtQuick, 
                     OpenGlVideoQtQuickRenderer* openGlVideoQtQuickRenderer){
            this->openGlVideoQtQuick = openGlVideoQtQuick;
            this->openGlVideoQtQuickRenderer = openGlVideoQtQuickRenderer;
        }
        void updateData(unsigned char**data) { //https://github.com/zhenyouluo/qt-qml-opengles-triangle/blob/e2d95e35f760cbe5805954eee685a2355524d8aa/gles-triangle-view.cpp#L89
            //std::cout << "updating data in helper" << std::endl;
            this->openGlVideoQtQuickRenderer->updateData(data, 0, 0);
            //Updates the view matrix. //TODO: make this update occur less often
            //this->openGlVideoQtQuickRenderer->qQuickVideoMatrix = this->openGlVideoQtQuick->getModelMatrix();
            //this->openGlVideoQtQuickRenderer->render();
            //std::cout << "uri: " << this->openGlVideoQtQuick->uri.toStdString() << std::endl;
            //if (this->openGlVideoQtQuick->window()) {
             //   std::cout << "window update" << std::endl;
                //this->openGlVideoQtQuick->update();
              //  this->openGlVideoQtQuick->window()->update();
                //this->openGlVideoQtQuick->setT(m_t);
                //this->openGlVideoQtQuick->setWindow(window());
            //}
        }
    protected:
        OpenGlVideoQtQuick * openGlVideoQtQuick;
        OpenGlVideoQtQuickRenderer * openGlVideoQtQuickRenderer;
};
*/
#endif // OpenGlVideoQtQuick_H