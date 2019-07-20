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
#include "flexbox.h"
#include "componentmanagers/viewmanager.h"
#include <boost/thread.hpp>
#include "MediaStream.h"
#include "reactitem.h"
#include "VideoReceiver.h"
#include "Glue.h"

class OpenGlBufferItemRenderer: public QQuickFramebufferObject::Renderer, public QOpenGLFunctions, public VideoReceiver
{
public:
    OpenGlBufferItemRenderer();
    void initOpenGl();
    void render() override;
    QOpenGLFramebufferObject* createFramebufferObject(const QSize &size) override;
    void receiveVideo(unsigned char**videoBuffer, int frameWidth, int frameHeight);
    void synchronize(QQuickFramebufferObject *item);


private:
    QSize m_viewportSize;
    qreal m_t;
    QOpenGLShaderProgram* program;
    QQuickWindow *m_window;
    GLuint unis[3] = {0};
    GLuint texs[3] = { 0 };
    unsigned char *datas[3] = { 0 };
    bool firstRender = true;
    bool firstFrameReceived = false;
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
    Q_PROPERTY(QString uri WRITE setUri)// NOTIFY uriChanged)
    Q_PROPERTY(QString p_uri WRITE setPUri);
    Q_PROPERTY(qreal p_height WRITE _setHeight);
    Q_PROPERTY(qreal p_width WRITE _setWidth);
    Q_PROPERTY(QString id WRITE setId READ getId)// NOTIFY uriChanged)
    Q_PROPERTY(QString p_id WRITE setPId READ getPId)// NOTIFY uriChanged)
    Q_PROPERTY(Flexbox* flexbox READ getFlexbox);
    Q_PROPERTY(ViewManager* viewManager);
public:
    Renderer *createRenderer() const;

    OpenGlBufferItem() {
        flexbox->setControl(this);
        flexbox->setViewManager(this->viewManager);
    }
    ~OpenGlBufferItem() {
        if (this->id!=nullptr) {
            Glue::instance()->get(this->id).mediaStream->ffmpegDecoder->disableVideoReceiver();
        } else if (this->p_id!=nullptr) {
        if(Glue::instance()->get(this->p_id).mediaStream==nullptr) {
            //TODO (VERY IMPORTANT): retry every x millisseconds until we have a definition, or find a better solution
            std::cout << "/1/1/1/1/1/1/1/1/11/1/1 ERROR: mediaStream is undefined for " << this->p_id.toStdString() << std::endl;
        } else {
            Glue::instance()->get(this->p_id).mediaStream->ffmpegDecoder->disableVideoReceiver();
        }
    }else {
        std::cout << "ERROR, id not set or not set yet " << std::endl;
    }
    }
    void initialization();
    QString p_uri;
    QString id;
    QString p_id;
    QString uri;
    qreal p_width;
    qreal p_height;
    Flexbox* flexbox = new Flexbox();
    ViewManager* viewManager = nullptr;
    
    void setUri(const QString &a) {
        uri = a;
    }

    void setPUri(const QString &a) {
        p_uri = a;
    }

    void setId(const QString &a) {
        id = a;
    }

    void setPId(const QString &a) {
        p_id = a;
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

    Flexbox* getFlexbox() {
        return this->flexbox;
    }

    
    explicit OpenGlBufferItem(QQuickItem* parent){
        //TODO: why this is never called? I've read somewhere that I must use this constructor
        flexbox->setControl(this);
        flexbox->setViewManager(this->viewManager);
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
