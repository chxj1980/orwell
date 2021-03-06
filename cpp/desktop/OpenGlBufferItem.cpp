#include "OpenGlBufferItem.h"
#include <QOpenGLFramebufferObjectFormat>
#include <QRunnable>
#include <QEventLoop>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QMutexLocker>
#include <memory>
#include <iostream>
#include <QTimer>

#define GET_STR(x) #x
#define A_VER 0
#define T_VER 1

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
        textureOut = vec2(textureIn.x, 1.0 - textureIn.y);
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
}


void OpenGlBufferItemRenderer::render() {
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glClearColor(0.f, 0.f, 0.f, 0.f);
    f->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (firstFrameReceived) {
        if (this->firstRender) {
            program = new QOpenGLShaderProgram();
            f->initializeOpenGLFunctions();

            int ve = program->addShaderFromSourceCode(QOpenGLShader::Vertex, vString3);
            int fr = program->addShaderFromSourceCode(QOpenGLShader::Fragment, tString3);
            if (ve!=1)
                std::cout << "Vertex Shader compilation problem: " << ve << std::endl;
            if (fr!=1)
                std::cout << "Fragment Shader compilation problem: " << fr << std::endl;
        

            program->bindAttributeLocation("vertexIn",A_VER);
            program->bindAttributeLocation("textureIn",T_VER);

            int li = program->link();
            if (li!=1)
                std::cout << "Program linking problem: " << li << std::endl;

            f->glGenTextures(3, texs);//TODO: ERASE THIS WITH glDeleteTextures

            //Y
            f->glBindTexture(GL_TEXTURE_2D, texs[0]);
            f->glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, frameWidth, frameHeight, 0, GL_RED, GL_UNSIGNED_BYTE, 0);
            f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            //U
            f->glBindTexture(GL_TEXTURE_2D, texs[1]);
            f->glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, frameWidth/2, frameHeight / 2, 0, GL_RED, GL_UNSIGNED_BYTE, 0);
            f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            //V
            f->glBindTexture(GL_TEXTURE_2D, texs[2]);
            f->glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, frameWidth / 2, frameHeight / 2, 0, GL_RED, GL_UNSIGNED_BYTE, 0);
            f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

            this->firstRender = false;
        
        }
        
        // Not strictly needed for this example, but generally useful for when
        // mixing with raw OpenGL.
        //m_window->resetOpenGLState();//COMMENT OR NOT?
        GLint originTextureUnit;
        f->glGetIntegerv(GL_ACTIVE_TEXTURE, &originTextureUnit);

        program->bind();

        QMatrix4x4 transform;
        transform.setToIdentity();
        program->setUniformValue("u_transform", transform);

        f->glVertexAttribPointer(A_VER, 2, GL_FLOAT, 0, 0, ver);
        f->glEnableVertexAttribArray(A_VER);

        f->glVertexAttribPointer(T_VER, 2, GL_FLOAT, 0, 0, tex);
        f->glEnableVertexAttribArray(T_VER);

        unis[0] = program->uniformLocation("tex_y");
        unis[1] = program->uniformLocation("tex_u");
        unis[2] = program->uniformLocation("tex_v");
        

        f->glActiveTexture(GL_TEXTURE0);
        f->glBindTexture(GL_TEXTURE_2D, texs[0]);
        f->glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, frameWidth, frameHeight, GL_RED, GL_UNSIGNED_BYTE, datas[0]);
        f->glUniform1i(unis[0], 0);


        f->glActiveTexture(GL_TEXTURE0+1);
        f->glBindTexture(GL_TEXTURE_2D, texs[1]); 
        f->glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, frameWidth/2, frameHeight / 2, GL_RED, GL_UNSIGNED_BYTE, datas[1]);
        f->glUniform1i(unis[1],1);


        f->glActiveTexture(GL_TEXTURE0+2);
        f->glBindTexture(GL_TEXTURE_2D, texs[2]);
        f->glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, frameWidth / 2, frameHeight / 2, GL_RED, GL_UNSIGNED_BYTE, datas[2]);
        f->glUniform1i(unis[2], 2);

        f->glDrawArrays(GL_TRIANGLE_STRIP,0,4);

        program->disableAttributeArray(A_VER);
        program->disableAttributeArray(T_VER);
        program->release();

        f->glActiveTexture(originTextureUnit);
        //f->doneCurrent();
        //QOpenGLContext::doneCurrent()
        //QOpenGLContext::currentContext()->doneCurrent();
    } 
    update();
}

QOpenGLFramebufferObject *OpenGlBufferItemRenderer::createFramebufferObject(const QSize &size)
{
    QOpenGLFramebufferObjectFormat format;
    format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
    //format.setSamples(16);
    return new QOpenGLFramebufferObject(size, format);
}


void OpenGlBufferItemRenderer::receiveVideo(unsigned char**videoBuffer, int frameWidth, int frameHeight) {
    this->frameWidth = frameWidth;
    this->frameHeight = frameHeight;
    //Before first render, datas pointer isn't even created yet
    if (!firstFrameReceived) {
        datas[0] = new unsigned char[frameWidth*frameHeight];	//Y
        datas[1] = new unsigned char[frameWidth*frameHeight/4];	//U
        datas[2] = new unsigned char[frameWidth*frameHeight/4];	//V
        firstFrameReceived = true;
    } else {
        memcpy(datas[0], videoBuffer[0], frameWidth*frameHeight);
        memcpy(datas[1], videoBuffer[1], frameWidth*frameHeight/4);
        memcpy(datas[2], videoBuffer[2], frameWidth*frameHeight/4);
    }
}


//https://blog.qt.io/blog/2015/05/11/integrating-custom-opengl-rendering-with-qt-quick-via-qquickframebufferobject/
void OpenGlBufferItemRenderer::synchronize(QQuickFramebufferObject *item)
{
    //https://github.com/quitejonny/tangram-es/blob/b457b7fc59e3e0f3c6d7bc26a0b5fe62098376fb/platforms/qt/tangram/tangramquickrenderer.cpp#L54
    OpenGlBufferItem *openGlBufferItem = static_cast<OpenGlBufferItem*>(item);
    //TODO: make it setVideoReceiver(nullptr) when it goes away

    if (openGlBufferItem->id!=nullptr) {
        Glue::instance()->getStream(openGlBufferItem->id).mediaStream->ffmpegDecoder->setVideoReceiver(this);
    } else if (openGlBufferItem->p_id!=nullptr) {
        if(Glue::instance()->getStream(openGlBufferItem->p_id).mediaStream==nullptr) {
            //TODO (VERY IMPORTANT): retry every x millisseconds until we have a definition, or find a better solution
            std::cout << "/1/1/1/1/1/1/1/1/11/1/1 ERROR: mediaStream is undefined for " << openGlBufferItem->p_id.toStdString() << std::endl;
        } else {
            Glue::instance()->getStream(openGlBufferItem->p_id).mediaStream->ffmpegDecoder->setVideoReceiver(this);
        }
    }else {
        std::cout << "ERROR, id not set or not set yet " << std::endl;
    }
            

}

QQuickFramebufferObject::Renderer *OpenGlBufferItem::createRenderer() const
{
    //TODO: how do I know createRenderer will be called after uri is setted? I'm assuming it does.
    return new OpenGlBufferItemRenderer();
}