#include "OpenGlVideoQtQuick2.h"

#define GET_STR(x) #x
#define A_VER 3
#define T_VER 4

//Simple shader. Outpus the same location as input, I guess
const char *vString4 = GET_STR(
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
const char *tString4 = GET_STR(
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
        gl_FragColor = vec4(1.0,0,0, 1.0);
    }

);

OpenGlVideoQtQuick2::OpenGlVideoQtQuick2()
    : m_t(0),
    openGlVideoQtQuick2Renderer2(nullptr)
{
    connect(this, &QQuickItem::windowChanged, this, &OpenGlVideoQtQuick2::handleWindowChanged);
    std::cout << "constructor called" << std::endl;

    //setWidth(640);
    //setHeight(480);
}

OpenGlVideoQtQuick2::OpenGlVideoQtQuick2(std::string uri): m_t(0),
    openGlVideoQtQuick2Renderer2(nullptr) {
        this->uri = uri;
        connect(this, &QQuickItem::windowChanged, this, &OpenGlVideoQtQuick2::handleWindowChanged);
        std::cout << "constructor2 called" << std::endl;
        std::cout << "I got created with uri " << uri << std::endl;
    }

void OpenGlVideoQtQuick2::update()
{
    //std::cout << "onTimer called!" << std::endl;

    if (window())
        window()->update();
}

void OpenGlVideoQtQuick2::setT(qreal t)
{
    if (t == m_t)
        return;
    m_t = t;
    emit tChanged();
    if (window())
        window()->update();
}

void OpenGlVideoQtQuick2::handleWindowChanged(QQuickWindow *win)
{
        std::cout << "handleWindowChanged called" << std::endl;

    if (win) {
        connect(win, &QQuickWindow::beforeSynchronizing, this, &OpenGlVideoQtQuick2::sync, Qt::DirectConnection);
        //connect(win, &QQuickWindow::sceneGraphInvalidated, this, &OpenGlVideoQtQuick2::cleanup, Qt::DirectConnection);
        // If we allow QML to do the clearing, they would clear what we paint
        // and nothing would show.
        win->setClearBeforeRendering(false);
    }
}

void OpenGlVideoQtQuick2::cleanup()
{
   if (openGlVideoQtQuick2Renderer2) {
        delete openGlVideoQtQuick2Renderer2;
        openGlVideoQtQuick2Renderer2 = nullptr;
    }
}

OpenGlVideoQtQuick2Renderer2::~OpenGlVideoQtQuick2Renderer2()
{
    delete program;
}

void OpenGlVideoQtQuick2::sync()
{
    //std::cout << "sync called" << std::endl;
    if (!openGlVideoQtQuick2Renderer2) {
        openGlVideoQtQuick2Renderer2 = new OpenGlVideoQtQuick2Renderer2();
        connect(window(), &QQuickWindow::beforeRendering, openGlVideoQtQuick2Renderer2, &OpenGlVideoQtQuick2Renderer2::render, Qt::DirectConnection);
        //connect(window(), &QQuickWindow::afterRendering, OpenGlVideoQtQuick2Renderer2, &OpenGlVideoQtQuick2Renderer2::render, Qt::DirectConnection);
        connect(window(), &QQuickWindow::afterRendering, this, &OpenGlVideoQtQuick2::update, Qt::DirectConnection);
        
        //OpenGlHelper* openGlHelper = new OpenGlHelper(this, OpenGlVideoQtQuick2Renderer2);
        MediaStream* camera1 = new MediaStream(this->uri);
        camera1->setFrameUpdater((FrameUpdater *) openGlVideoQtQuick2Renderer2);
        //TODO: put mutex on std::cout of this thread
        boost::thread mediaThread(&MediaStream::run, camera1);
    }
    //OpenGlVideoQtQuick2Renderer2->setViewportSize(window()->size() * window()->devicePixelRatio());
    //std::cout << "updating matrix " << std::endl;
    //this->OpenGlVideoQtQuick2Renderer2->qQuickVideoMatrix = getModelMatrix();
    openGlVideoQtQuick2Renderer2->setT(m_t);
    openGlVideoQtQuick2Renderer2->setWindow(window());
    openGlVideoQtQuick2Renderer2->setWidth(width());
    openGlVideoQtQuick2Renderer2->setHeight(height());
    openGlVideoQtQuick2Renderer2->setX(x());
    openGlVideoQtQuick2Renderer2->setY(y());
}

//https://stackoverflow.com/a/46484719/10116440
/*
QMatrix4x4 OpenGlVideoQtQuick2::getModelMatrix() {
    QMatrix4x4 result;

    // Compose model matrix from our transform properties in the QML
    QQmlListProperty<QQuickTransform> transformations = transform();
    const int count = transformations.count(&transformations);
    for (int i=0; i<count; i++) {
        QQuickTransform *transform = transformations.at(&transformations, i);
        transform->applyTo(&result);
    }

    return result;
}
*/

void OpenGlVideoQtQuick2Renderer2::updateData(unsigned char**data, int frameWidth, int frameHeight)
{
    this->frameWidth = frameWidth;
    this->frameHeight = frameHeight;
    //Before first render, datas pointer isn't even created yet
    if (!firstRender) {
        memcpy(datas[0], data[0], frameWidth*frameHeight);
        memcpy(datas[1], data[1], frameWidth*frameHeight/4);
        memcpy(datas[2], data[2], frameWidth*frameHeight/4);
    }
}

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
float mapToMinus11_(float value, float max) {
        return -1+2*value/max;
}

//TODO: FIX THIS https://stackoverflow.com/a/54773889/6655884
void OpenGlVideoQtQuick2Renderer2::render()
{
    frameWidth = 1280;
    frameHeight = 720;
    if (this->firstRender) {
        std::cout << "Creating QOpenGLShaderProgram " << std::endl;
        program = new QOpenGLShaderProgram();
        initializeOpenGLFunctions();
        //this->m_F  = QOpenGLContext::currentContext()->functions();
        std::cout << "frameWidth: " << frameWidth << + " frameHeight: " << frameHeight << std::endl;
        datas[0] = new unsigned char[frameWidth*frameHeight];	//Y
        datas[1] = new unsigned char[frameWidth*frameHeight/4];	//U
        datas[2] = new unsigned char[frameWidth*frameHeight/4];	//V

        std::cout << "Fragment Shader compilation: " << program->addShaderFromSourceCode(QOpenGLShader::Fragment, tString4) << std::endl;
        std::cout << "Vertex Shader compilation: " << program->addShaderFromSourceCode(QOpenGLShader::Vertex, vString4) << std::endl;

        program->bindAttributeLocation("vertexIn",A_VER);
        program->bindAttributeLocation("textureIn",T_VER);
        std::cout << "program->link() = " << program->link() << std::endl;

        glGenTextures(3, texs);//TODO: ERASE THIS WITH glDeleteTextures
        this->firstRender = false;
        program->bind();
    }


    glVertexAttribPointer(A_VER, 2, GL_FLOAT, 0, 0, ver);
    glEnableVertexAttribArray(A_VER);

    glVertexAttribPointer(T_VER, 2, GL_FLOAT, 0, 0, tex);
    glEnableVertexAttribArray(T_VER);

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
 /*   
    //Only begin to render if we already know the frameWidth and frameHeight (gathered from cameras)
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

            std::cout << "Fragment Shader compilation: " << program->addShaderFromSourceCode(QOpenGLShader::Fragment, tString4) << std::endl;
            std::cout << "Vertex Shader compilation: " << program->addShaderFromSourceCode(QOpenGLShader::Vertex, vString4) << std::endl;

            program->bindAttributeLocation("vertexIn",A_VER);
            program->bindAttributeLocation("textureIn",T_VER);
            std::cout << "program->link() = " << program->link() << std::endl;

            glGenTextures(3, texs);//TODO: ERASE THIS WITH glDeleteTextures
            this->firstRender = false;
        }
        program->bind();

        QMatrix4x4 transform;
        transform.setToIdentity();
        
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
//        program->setUniformValue("u_transform", transform);
        program->setUniformValue("u_transform", this->qQuickVideoMatrix);

        glViewport(this->x, this->y, this->width, this->height);

        glVertexAttribPointer(A_VER, 2, GL_FLOAT, 0, 0, ver);
        glEnableVertexAttribArray(A_VER);

        glVertexAttribPointer(T_VER, 2, GL_FLOAT, 0, 0, tex);
        glEnableVertexAttribArray(T_VER);

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

        // Not strictly needed for this example, but generally useful for when
        // mixing with raw OpenGL.
        //m_window->resetOpenGLState();//COMMENT OR NOT?
        
    }
    */
}