#include "OpenGlVideoQtQuick.h"

#define GET_STR(x) #x
#define A_VER 3
#define T_VER 4

//Simple shader. Outpus the same location as input, I guess
const char *vString2 = GET_STR(
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
const char *tString2 = GET_STR(
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

OpenGlVideoQtQuick::OpenGlVideoQtQuick()
    : m_t(0),
    openGlVideoQtQuickRenderer(nullptr)
{
    connect(this, &QQuickItem::windowChanged, this, &OpenGlVideoQtQuick::handleWindowChanged);
    std::cout << "constructor called" << std::endl;

    //setWidth(640);
    //setHeight(480);
    //width=640
    //height=480
}

OpenGlVideoQtQuick::OpenGlVideoQtQuick(QString uri): m_t(0),
    openGlVideoQtQuickRenderer(nullptr) {
        this->uri = uri;
        connect(this, &QQuickItem::windowChanged, this, &OpenGlVideoQtQuick::handleWindowChanged);
        std::cout << "constructor2 called" << std::endl;
        std::cout << "I got created with uri " << uri.toStdString() << std::endl;
    }

void OpenGlVideoQtQuick::update()
{
    //std::cout << "onTimer called!" << std::endl;

    if (window())
        window()->update();
}

void OpenGlVideoQtQuick::setT(qreal t)
{
    if (t == m_t)
        return;
    m_t = t;
    emit tChanged();
    if (window())
        window()->update();
}

void OpenGlVideoQtQuick::handleWindowChanged(QQuickWindow *win)
{
    std::cout << "handleWindowChanged called" << std::endl;
    if (win) {
        connect(win, &QQuickWindow::beforeSynchronizing, this, &OpenGlVideoQtQuick::sync, Qt::DirectConnection);
        //connect(win, &QQuickWindow::sceneGraphInvalidated, this, &OpenGlVideoQtQuick::cleanup, Qt::DirectConnection);
        // If we allow QML to do the clearing, they would clear what we paint
        // and nothing would show.
        win->setClearBeforeRendering(false);
    }
}

void OpenGlVideoQtQuick::cleanup()
{
   if (openGlVideoQtQuickRenderer) {
        delete openGlVideoQtQuickRenderer;
        openGlVideoQtQuickRenderer = nullptr;
    }
}

OpenGlVideoQtQuickRenderer::~OpenGlVideoQtQuickRenderer()
{
    delete program;
}

void OpenGlVideoQtQuick::sync()
{
    //std::cout << "sync called" << std::endl;
    if (!openGlVideoQtQuickRenderer) {
        openGlVideoQtQuickRenderer = new OpenGlVideoQtQuickRenderer();
        connect(window(), &QQuickWindow::beforeRendering, openGlVideoQtQuickRenderer, &OpenGlVideoQtQuickRenderer::render, Qt::DirectConnection);
        //connect(window(), &QQuickWindow::afterRendering, openGlVideoQtQuickRenderer, &OpenGlVideoQtQuickRenderer::render, Qt::DirectConnection);
        connect(window(), &QQuickWindow::afterRendering, this, &OpenGlVideoQtQuick::update, Qt::DirectConnection);

        MediaStream* camera1 = new MediaStream(this->uri.toStdString());
        camera1->setFrameUpdater((FrameUpdater *) openGlVideoQtQuickRenderer);
        //TODO: put mutex on std::cout of this thread
        boost::thread mediaThread(&MediaStream::run, camera1);
    }
    //openGlVideoQtQuickRenderer->setViewportSize(window()->size() * window()->devicePixelRatio());
    openGlVideoQtQuickRenderer->setT(m_t);
    openGlVideoQtQuickRenderer->setWindow(window());
    openGlVideoQtQuickRenderer->setWidth(width());
    openGlVideoQtQuickRenderer->setHeight(height());
    openGlVideoQtQuickRenderer->setX(x());
    openGlVideoQtQuickRenderer->setY(y());
}


void OpenGlVideoQtQuickRenderer::updateData(unsigned char**data)
{
    memcpy(datas[0], data[0], width*height);
    memcpy(datas[1], data[1], width*height/4);
    memcpy(datas[2], data[2], width*height/4);    
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
float mapToMinus11(float value, float max) {
        return -1+2*value/max;
    }
//TODO: FIX THIS https://stackoverflow.com/a/54773889/6655884
void OpenGlVideoQtQuickRenderer::render()
{
    if (this->firstRun) {
        std::cout << "Creating QOpenGLShaderProgram " << std::endl;
        this->firstRun = false;
        program = new QOpenGLShaderProgram();
        initializeOpenGLFunctions();
        //this->m_F  = QOpenGLContext::currentContext()->functions();

        datas[0] = new unsigned char[width*height];		//Y
        datas[1] = new unsigned char[width*height/4];	//U
        datas[2] = new unsigned char[width*height/4];	//V

        std::cout << "Fragment Shader compilation: " << program->addShaderFromSourceCode(QOpenGLShader::Fragment, tString2) << std::endl;
        std::cout << "Vertex Shader compilation: " << program->addShaderFromSourceCode(QOpenGLShader::Vertex, vString2) << std::endl;

        program->bindAttributeLocation("vertexIn",A_VER);
        program->bindAttributeLocation("textureIn",T_VER);
        std::cout << "program->link() = " << program->link() << std::endl;

        glGenTextures(3, texs);//TODO: ERASE THIS WITH glDeleteTextures
    }
    program->bind();

    QMatrix4x4 transform;
    transform.setToIdentity();
    transform.scale(0.5, 0.5);
    //transform.translate(1,1);
    transform.translate(mapToMinus11(this->x, width),-1*mapToMinus11(this->y, height));
        std::cout << "width: " << mapToMinus11(this->x, width) << " height " << mapToMinus11(this->y, height) << std::endl;

    std::cout << "width: " << mapToMinus11(this->x, width) << " height " << mapToMinus11(this->y, height) << std::endl;
    std::cout << "x: " << x << " y: " << y << std::endl;
    //QMatrix4x4 translate;
    //translate.viewport(-width/2, -height/2, width, height);
    //translate.inverted();

    //transform = translate*transform;
    program->setUniformValue("u_transform", transform);

    //glViewport(50, 50, 50, 50);

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
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, 0);

    //U
    glBindTexture(GL_TEXTURE_2D, texs[1]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width/2, height / 2, 0, GL_RED, GL_UNSIGNED_BYTE, 0);

    //V
    glBindTexture(GL_TEXTURE_2D, texs[2]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width / 2, height / 2, 0, GL_RED, GL_UNSIGNED_BYTE, 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texs[0]);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RED, GL_UNSIGNED_BYTE, datas[0]);
    glUniform1i(unis[0], 0);


    glActiveTexture(GL_TEXTURE0+1);
    glBindTexture(GL_TEXTURE_2D, texs[1]); 
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width/2, height / 2, GL_RED, GL_UNSIGNED_BYTE, datas[1]);
    glUniform1i(unis[1],1);


    glActiveTexture(GL_TEXTURE0+2);
    glBindTexture(GL_TEXTURE_2D, texs[2]);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width / 2, height / 2, GL_RED, GL_UNSIGNED_BYTE, datas[2]);
    glUniform1i(unis[2], 2);

    glDrawArrays(GL_TRIANGLE_STRIP,0,4);

    program->disableAttributeArray(A_VER);
    program->disableAttributeArray(T_VER);
    program->release();

    // Not strictly needed for this example, but generally useful for when
    // mixing with raw OpenGL.
    //m_window->resetOpenGLState();//COMMENT OR NOT?
}