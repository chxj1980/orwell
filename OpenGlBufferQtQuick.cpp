#include "OpenGlBufferQtQuick.h"
#include <QOpenGLFramebufferObjectFormat>
#include <QRunnable>
#include <QEventLoop>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QMutexLocker>
#include <memory>
#include <iostream>


#define GET_STR(x) #x
#define A_VER 3
#define T_VER 4

static const GLfloat ver[] = {
    -1.0f,-1.0f,
     1.0f,-1.0f,
    -1.0f, 1.0f,
     1.0f, 1.0f
};

static const GLfloat tex[] = {
    0.0f, 1.0f,
    1.0f, 1.0f,
    0.0f, 0.0f,
    1.0f, 0.0f
};

//Simple shader. Outpus the same location as input, I guess
const char *vString3 = GET_STR(
    attribute vec4 vertexIn;
    attribute vec2 textureIn;
    varying vec2 textureOut;
    uniform mat4 u_transform;   
    void main(void)
    {
        gl_Position = u_transform * vertexIn;
        textureOut = textureIn;
    }
);


//The matrix below does YUV420P to RGB conversion https://en.wikipedia.org/wiki/YUV#Y%E2%80%B2UV420p_(and_Y%E2%80%B2V12_or_YV12)_to_RGB888_conversion
//This texture shader replaces the color of the pixel with the new color, but in RGB. (I guess)
const char *tString3 = GET_STR(
    varying vec2 textureOut;
    uniform sampler2D tex_y;
    uniform sampler2D tex_u;
    uniform sampler2D tex_v;
    void main(void)
    {
        vec3 yuv;
        vec3 rgb;
        yuv.x = texture2D(tex_y, textureOut).r;
        yuv.y = texture2D(tex_u, textureOut).r - 0.5;
        yuv.z = texture2D(tex_v, textureOut).r - 0.5;
        rgb = mat3(1.0, 1.0, 1.0,
            0.0, -0.39465, 2.03211,
            1.13983, -0.58060, 0.0) * yuv;
        gl_FragColor = vec4(rgb, 1.0);
    }

);


OpenGlBufferItemRenderer::OpenGlBufferItemRenderer(){
    std::cout << "renderer created " << std::endl;
}


void OpenGlBufferItemRenderer::initialization(){
}

void OpenGlBufferItemRenderer::render() {
    std::cout << "render()" << std::endl;
    this->frameWidth = 1920;
    this->frameHeight = 1080;
    if (this->frameWidth >0 && this->frameHeight>0) {
        if (this->firstRender) {
            std::cout << "Creating QOpenGLShaderProgram " << std::endl;
            program = new QOpenGLShaderProgram();
            initializeOpenGLFunctions();
            //this->m_F  = QOpenGLContext::currentContext()->functions();
            std::cout << "frameWidth: " << frameWidth << + " frameHeight: " << frameHeight << std::endl;
            datas[0] = new unsigned char[frameWidth*frameHeight];	//Y
            datas[1] = new unsigned char[frameWidth*frameHeight/4];	//U
            datas[2] = new unsigned char[frameWidth*frameHeight/4];	//V

            std::cout << "Fragment Shader compilation: " << program->addShaderFromSourceCode(QOpenGLShader::Fragment, tString3) << std::endl;
            std::cout << "Vertex Shader compilation: " << program->addShaderFromSourceCode(QOpenGLShader::Vertex, vString3) << std::endl;

            program->bindAttributeLocation("vertexIn",A_VER);
            program->bindAttributeLocation("textureIn",T_VER);
            std::cout << "program->link() = " << program->link() << std::endl;

            glGenTextures(3, texs);//TODO: ERASE THIS WITH glDeleteTextures
            this->firstRender = false;
        }
        

        // Not strictly needed for this example, but generally useful for when
        // mixing with raw OpenGL.
        //m_window->resetOpenGLState();//COMMENT OR NOT?
        
        std::cout << "bind - 105 " << std::endl;


        program->bind();

        QMatrix4x4 transform;
        transform.setToIdentity();
        /*
            width and height are the sizes of the QQuickItem, 
            while frameWidth and frameHeight are the sizes of
            the frame being decoded. width/frameWidth, and 
            height/frameHeight are precisely the values we
            need to scale the image so it gets the size of
            the QQuickItem.

        */ 
        //transform.translate(mapToMinus11(this->x, frameWidth),-1*mapToMinus11(this->y, frameHeight));
        //transform.translate(1,1);
        //transform.scale(0.5,0.5);
        //transform.scale((float)width/(float)frameWidth, (float)height/(float)frameHeight);

        //transform.translate(mapToMinus11(this->x, width),-1*mapToMinus11(this->y, height));
        //std::cout << "real width: " << width << " real height " << height << std::endl;
        //std::cout << "width: " << mapToMinus11(this->x, width) << " height " << mapToMinus11(this->y, height) << std::endl;
        //std::cout << "x: " << x << " y: " << y << std::endl;
        //QMatrix4x4 translate;
        //translate.viewport(-width/2, -height/2, width, height);
        //translate.inverted();

        //transform = translate*transform;
        //program->setUniformValue("u_transform", transform);
        program->setUniformValue("u_transform", transform);

        //glViewport(50, 50, 50, 50);

        glVertexAttribPointer(A_VER, 2, GL_FLOAT, 0, 0, ver);
        glEnableVertexAttribArray(A_VER);

        glVertexAttribPointer(T_VER, 2, GL_FLOAT, 0, 0, tex);
        glEnableVertexAttribArray(T_VER);
        std::cout << "145 " << std::endl;

        unis[0] = program->uniformLocation("tex_y");
        unis[1] = program->uniformLocation("tex_u");
        unis[2] = program->uniformLocation("tex_v");
        
        //Y
        glBindTexture(GL_TEXTURE_2D, texs[0]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, frameWidth, frameHeight, 0, GL_RED, GL_UNSIGNED_BYTE, 0);

        //U
        glBindTexture(GL_TEXTURE_2D, texs[1]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, frameWidth/2, frameHeight / 2, 0, GL_RED, GL_UNSIGNED_BYTE, 0);

        //V
        glBindTexture(GL_TEXTURE_2D, texs[2]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, frameWidth / 2, frameHeight / 2, 0, GL_RED, GL_UNSIGNED_BYTE, 0);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texs[0]);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, frameWidth, frameHeight, GL_RED, GL_UNSIGNED_BYTE, datas[0]);
        glUniform1i(unis[0], 0);


        glActiveTexture(GL_TEXTURE0+1);
        glBindTexture(GL_TEXTURE_2D, texs[1]); 
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, frameWidth/2, frameHeight / 2, GL_RED, GL_UNSIGNED_BYTE, datas[1]);
        glUniform1i(unis[1],1);


        glActiveTexture(GL_TEXTURE0+2);
        glBindTexture(GL_TEXTURE_2D, texs[2]);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, frameWidth / 2, frameHeight / 2, GL_RED, GL_UNSIGNED_BYTE, datas[2]);
        glUniform1i(unis[2], 2);

        glDrawArrays(GL_TRIANGLE_STRIP,0,4);

        program->disableAttributeArray(A_VER);
        program->disableAttributeArray(T_VER);
        program->release();

        //window->resetOpenGLState();
        update();
    }
}

QOpenGLFramebufferObject *OpenGlBufferItemRenderer::createFramebufferObject(const QSize &size)
{
    QOpenGLFramebufferObjectFormat format;
    format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
    format.setSamples(16);
    return new QOpenGLFramebufferObject(size, format);
}
//https://blog.qt.io/blog/2015/05/11/integrating-custom-opengl-rendering-with-qt-quick-via-qquickframebufferobject/
void OpenGlBufferItemRenderer::synchronize(QQuickFramebufferObject *item)
{
    std::cout << "synchronize called " << std::endl;
    //OpenGlHelper* openGlHelper = new OpenGlHelper(this, openGlVideoQtQuickRenderer);
    MediaStream* camera1 = new MediaStream("rtsp://admin:19929394@192.168.0.103:10554/tcp/av0_0");
    camera1->setFrameUpdater((FrameUpdater *) this);
    //TODO: put mutex on std::cout of this thread
    boost::thread mediaThread(&MediaStream::run, camera1); 
    /*
    OpenGlBufferItem *cube = static_cast<OpenGlBufferItem*>(item);
    if(cube->isTextureDirty()){
        delete m_texture;
        m_texture = nullptr;
        m_texture = new QOpenGLTexture(cube->textureImage().mirrored());
        m_texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
        m_texture->setMagnificationFilter(QOpenGLTexture::Linear);
        cube->setTextureDirty(false);
    }
    window = cube->window();
    */
}

OpenGlBufferItem::OpenGlBufferItem()
{
    //setMirrorVertically(true);
    //connect(this,&OpenGlBufferItem::textureImageReady,this,&QQuickItem::update);
}

void OpenGlBufferItemRenderer::updateData(unsigned char**data)
{
    std::cout << "updateData"<< std::endl;
    //Before first render, datas pointer isn't even created yet
    if (!firstRender) {
        std::cout << "updateData first render passed"<< std::endl;

        memcpy(datas[0], data[0], frameWidth*frameHeight);
        memcpy(datas[1], data[1], frameWidth*frameHeight/4);
        memcpy(datas[2], data[2], frameWidth*frameHeight/4);
    }
}

QQuickFramebufferObject::Renderer *OpenGlBufferItem::createRenderer() const
{
    return new OpenGlBufferItemRenderer;
}
