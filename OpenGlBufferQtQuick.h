#ifndef OpenGlBufferItem_H
#define OpenGlBufferItem_H
#include <QObject>
#include <QQuickFramebufferObject>
#include <QOpenGLFunctions>
#include <QOpenGLTexture>
#include <QOpenGLShaderProgram>
#include <QQuickWindow>
#include <QThreadPool>
#include <QRunnable>
#include <QMutex>
#include <QNetworkAccessManager>
#include <functional>
#include <boost/thread.hpp>
#include "MediaStream.h"
#include "reactitem.h"

class OpenGlBufferItemRenderer: public QQuickFramebufferObject::Renderer, public QOpenGLFunctions, public FrameUpdater
{
public:
    OpenGlBufferItemRenderer(string uri);
    void initOpenGl();
    void render() override;
    QOpenGLFramebufferObject* createFramebufferObject(const QSize &size) override;
    void updateData(unsigned char**data, int frameWidth, int frameHeight);
    void synchronize(QQuickFramebufferObject *item);
private:
    QSize m_viewportSize;
    qreal m_t;
    QOpenGLShaderProgram* program;
    QQuickWindow *m_window;
    GLuint unis[3] = {0};
    GLuint texs[3] = { 0 };
    //OpenGlVideoQtQuick* openGlVideoQtQuick;
    unsigned char *datas[3] = { 0 };
    bool firstRender = true;
    bool firstFrameReceived = false;
     //TODO: make this variable according to video data
    int width = 0;
    int height = 0;
    int x = 0;
    int y = 0;
    int frameWidth = 0;
    int frameHeight = 0;
    string uri;
};

class OpenGlBufferItem: public QQuickFramebufferObject//, public ReactItem
{
    Q_OBJECT
public:
    OpenGlBufferItem();
    Renderer *createRenderer() const;
    //void updateData(unsigned char**data);
    //void componentComplete();
    void initialization();
    MediaStream* camera;
    Q_PROPERTY(QString uri WRITE setUri)// NOTIFY uriChanged)
    std::string uri;
    //void setUri(const std::string uri) {
    //    this->uri = uri;
    //}
    void setUri(const QString &a) {
        uri = a.toStdString();
    }

private:
    QString m_texturePath;
    QImage m_textureImage;
    QThreadPool m_pool;
    QNetworkAccessManager m_manger;
    QMutex m_mutex;
    bool m_textureDirty = false;
    unsigned char *datas[3] = { 0 };
    bool firstRender = true;
    OpenGlBufferItemRenderer * openGlBufferItemRenderer;

    
};
/*
class OpenGlBufferHelper: public FrameUpdater {
    public:
        OpenGlBufferHelper(OpenGlBufferItem* openGlBufferItem, 
                           OpenGlBufferItemRenderer* openGlBufferItemRenderer) {
            this->openGlBufferItem = openGlBufferItem;
            this->openGlBufferItemRenderer = openGlBufferItemRenderer;
        }
        void updateData(unsigned char**data) { //https://github.com/zhenyouluo/qt-qml-opengles-triangle/blob/e2d95e35f760cbe5805954eee685a2355524d8aa/gles-triangle-view.cpp#L89
            //std::cout << "updating data in helper" << std::endl;
            this->openGlBufferItemRenderer->updateData(data);
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
        OpenGlBufferItem * openGlBufferItem;
        OpenGlBufferItemRenderer * openGlBufferItemRenderer;
};
*/
#endif // OpenGlBufferItem_H
