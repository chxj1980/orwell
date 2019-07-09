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
#include "VideoReceiver.h"
#include "Glue.h"

class OpenGlBufferItemRenderer: public QQuickFramebufferObject::Renderer, public QOpenGLFunctions, public FrameUpdater, public VideoReceiver
{
public:
    OpenGlBufferItemRenderer();
    void initOpenGl();
    void render() override;
    QOpenGLFramebufferObject* createFramebufferObject(const QSize &size) override;
    void updateData(unsigned char**data, int frameWidth, int frameHeight);
    void receiveVideo(unsigned char**videoBuffer, int frameWidth, int frameHeight);
    void synchronize(QQuickFramebufferObject *item);
    //Q_PROPERTY int p_width = 0;
    //Q_PROPERTY int p_height = 0;


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
    Renderer *createRenderer() const;
    //void updateData(unsigned char**data);
    //void componentComplete();
    OpenGlBufferItem() {}
    void initialization();
    MediaStream* camera;
    QString p_uri;
    QString id;
    std::string uri;
    qreal p_width;
    qreal p_height;
    Q_PROPERTY(QString uri WRITE setUri)// NOTIFY uriChanged)
    Q_PROPERTY(QString p_uri WRITE setPUri);
    Q_PROPERTY(qreal p_height WRITE _setHeight);
    Q_PROPERTY(qreal p_width WRITE _setWidth);
    Q_PROPERTY(QString id WRITE setId READ getId)// NOTIFY uriChanged)

    //void setUri(const std::string uri) {
    //    this->uri = uri;
    //}
    void setPUri(const QString &a) {
        p_uri = a;
    }
    void _setWidth(qreal width) {
        p_width = width;
    }
    void _setHeight(qreal height) {
        p_height = height;
    }
    void setUri(const QString &a) {
        uri = a.toStdString();
    }
    void setId(const QString &a) {
        id = a;
        std::cout << "OpenGlMaterialQQuickItem created with id " << id.toStdString() << std::endl;
    }
    QString getId() {
        return this->id;
    }

    
    explicit OpenGlBufferItem(QQuickItem* parent){
        if(parent){
            connect(parent, SIGNAL(widthChanged()), this, SLOT(parentWidthChanged()));
            connect(parent, SIGNAL(heightChanged()), this, SLOT(parentHeightChanged()));
            setWidth(parent->width());
            setHeight(parent->height());
        }
        else
            qWarning() << "MyItem must be initialized with a parent.";
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

private slots:
    void parentWidthChanged(){
        setWidth(parentItem()->width());
    }
    void parentHeightChanged(){
        setHeight(parentItem()->height());
    }
    
};

#endif // OpenGlBufferItem_H
