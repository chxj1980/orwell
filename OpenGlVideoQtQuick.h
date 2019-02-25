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
#include "reactitem.h"


class OpenGlVideoQtQuickRenderer : public QObject, protected QOpenGLFunctions, public FrameUpdater
{
    Q_OBJECT

public:
    OpenGlVideoQtQuickRenderer() : m_t(0) {        
    }
    ~OpenGlVideoQtQuickRenderer();
    void updateData(unsigned char**data);
    void setT(qreal t) { m_t = t; }
    void setViewportSize(const QSize &size) { m_viewportSize = size; }
    void setWindow(QQuickWindow *window) { m_window = window; }

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
    //OpenGlVideoQtQuick* openGlVideoQtQuick;
    unsigned char *datas[3] = { 0 };
    bool firstRun = true;
     //TODO: make this variable according to video data
    int width = 1920;
    int height = 1080;
    //boost::thread mediaThread;
};

class OpenGlVideoQtQuick : public QQuickItem//, public ReactItem
{
    Q_OBJECT
    Q_PROPERTY(qreal t READ t WRITE setT NOTIFY tChanged)
    Q_PROPERTY(QString uri WRITE setUri)// NOTIFY uriChanged)


public:
    OpenGlVideoQtQuick();
    OpenGlVideoQtQuick(QString uri);
    QString uri;

    qreal t() const { return m_t; }
    void setT(qreal t);
//    Q_INVOKABLE void initRtspMedia(const QString &uri);
    void setUri(const QString &a) {
        if (a != uri) {
            uri = a;
           // emit authorChanged();
        }
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
    qreal m_t;
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
            std::cout << "updating data in helper" << std::endl;
            this->openGlVideoQtQuickRenderer->updateData(data);
            //this->openGlVideoQtQuickRenderer->render();
            std::cout << "uri: " << this->openGlVideoQtQuick->uri.toStdString() << std::endl;
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