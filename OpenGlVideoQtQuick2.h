#ifndef OpenGlVideoQtQuick2_H
#define OpenGlVideoQtQuick2_H

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
#include "reactitem.h"
//#include "reactitem.h"


class OpenGlVideoQtQuick2Renderer2 : public QObject, protected QOpenGLFunctions, public FrameUpdater
{
    Q_OBJECT

public:
    OpenGlVideoQtQuick2Renderer2() : m_t(0) {        
    }
    ~OpenGlVideoQtQuick2Renderer2();
    void updateData(unsigned char**data, int frameWidth, int frameHeight);
    void setT(qreal t) { m_t = t; }
    void setViewportSize(const QSize &size) { m_viewportSize = size; }
    void setWindow(QQuickWindow *window) { m_window = window; }

    void setWidth(int width) {
        this->width = width;
    }

    void setHeight(int height) {
        this->height = height;
    }

    void setX(int x) {
        this->x = x;
    }

    void setY(int y) {
        this->y = y;
    }
    QMatrix4x4 qQuickVideoMatrix;

public slots:
    void render();

private:
    //std::string a;
    QSize m_viewportSize;
    qreal m_t;
    QOpenGLShaderProgram* program;
    QQuickWindow *m_window;
    GLuint unis[3] = {0};
    GLuint texs[3] = { 0 };
    //OpenGlVideoQtQuick2* OpenGlVideoQtQuick2;
    unsigned char *datas[3] = { 0 };
    bool firstRender = true;
     //TODO: make this variable according to video data
    int width = 0;
    int height = 0;
    int x = 0;
    int y = 0;
};

//class OpenGlVideoQtQuick2 : public QQuickItem
class OpenGlVideoQtQuick2 : public ReactItem
{
    Q_OBJECT
    Q_PROPERTY(qreal t READ t WRITE setT NOTIFY tChanged)
    Q_PROPERTY(QString uri WRITE setUri)// NOTIFY uriChanged)


public:
    OpenGlVideoQtQuick2();
    OpenGlVideoQtQuick2(std::string uri);
    QMatrix4x4 getModelMatrix();
    qreal t() const { return m_t; }
    void setT(qreal t);
//    Q_INVOKABLE void initRtspMedia(const QString &uri);
    std::string uri;
    //void setUri(const std::string uri) {
    //    this->uri = uri;
    //}
    void setUri(const QString &a) {
        uri = a.toStdString();
    }
    
    /*
    
    void setWidth(qreal width) {
        this->width = width;
    }
    void setHeight(qreal height) {
        this->height = height;
    }
    void setX(qreal x) {
        this->x = x;
    }
    void setY(qreal y) {
        this->y = y;
    }
    */

signals:
    void tChanged();

public slots:
    void sync();
    void cleanup();
    void update();//Updates the window


private slots:
    void handleWindowChanged(QQuickWindow *win);

private:
    qreal m_t;
    OpenGlVideoQtQuick2Renderer2 *openGlVideoQtQuick2Renderer2;

};

/*
class OpenGlHelper: public FrameUpdater {
    public:
        OpenGlHelper(OpenGlVideoQtQuick2* OpenGlVideoQtQuick2, 
                     OpenGlVideoQtQuick2Renderer2* OpenGlVideoQtQuick2Renderer2){
            this->OpenGlVideoQtQuick2 = OpenGlVideoQtQuick2;
            this->OpenGlVideoQtQuick2Renderer2 = OpenGlVideoQtQuick2Renderer2;
        }
        void updateData(unsigned char**data) { //https://github.com/zhenyouluo/qt-qml-opengles-triangle/blob/e2d95e35f760cbe5805954eee685a2355524d8aa/gles-triangle-view.cpp#L89
            //std::cout << "updating data in helper" << std::endl;
            this->OpenGlVideoQtQuick2Renderer2->updateData(data, 0, 0);
            //Updates the view matrix. //TODO: make this update occur less often
            //this->OpenGlVideoQtQuick2Renderer2->qQuickVideoMatrix = this->OpenGlVideoQtQuick2->getModelMatrix();
            //this->OpenGlVideoQtQuick2Renderer2->render();
            //std::cout << "uri: " << this->OpenGlVideoQtQuick2->uri.toStdString() << std::endl;
            //if (this->OpenGlVideoQtQuick2->window()) {
             //   std::cout << "window update" << std::endl;
                //this->OpenGlVideoQtQuick2->update();
              //  this->OpenGlVideoQtQuick2->window()->update();
                //this->OpenGlVideoQtQuick2->setT(m_t);
                //this->OpenGlVideoQtQuick2->setWindow(window());
            //}
        }
    protected:
        OpenGlVideoQtQuick2 * OpenGlVideoQtQuick2;
        OpenGlVideoQtQuick2Renderer2 * OpenGlVideoQtQuick2Renderer2;
};
*/
#endif // OpenGlVideoQtQuick2_H