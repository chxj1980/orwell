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

class OpenGlBufferItemRenderer: public QQuickFramebufferObject::Renderer, public QOpenGLFunctions
{
public:
    OpenGlBufferItemRenderer();
    void initialization();
    void render() override;
    QOpenGLFramebufferObject* createFramebufferObject(const QSize &size) override;
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
     //TODO: make this variable according to video data
    int width = 0;
    int height = 0;
    int x = 0;
    int y = 0;
};

class OpenGlBufferItem: public QQuickFramebufferObject
{
    Q_OBJECT
public:
    OpenGlBufferItem();
    Renderer *createRenderer() const;
    void updateData(unsigned char**data);

private:
    QString m_texturePath;
    QImage m_textureImage;
    QThreadPool m_pool;
    QNetworkAccessManager m_manger;
    QMutex m_mutex;
    bool m_textureDirty = false;
};

#endif // OpenGlBufferItem_H
