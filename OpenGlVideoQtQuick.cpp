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

OpenGlVideoQtQuick::OpenGlVideoQtQuick():openGlVideoQtQuickRenderer(nullptr)
{
    connect(this, &QQuickItem::windowChanged, this, &OpenGlVideoQtQuick::handleWindowChanged);
}

OpenGlVideoQtQuick::OpenGlVideoQtQuick(std::string uri):
    openGlVideoQtQuickRenderer(nullptr) {
        this->uri = uri;
        connect(this, &QQuickItem::windowChanged, this, &OpenGlVideoQtQuick::handleWindowChanged);
    }

void OpenGlVideoQtQuick::update()
{
    if (window())
        window()->update();
}


void OpenGlVideoQtQuick::handleWindowChanged(QQuickWindow *win)
{
    if (win) {
        connect(win, &QQuickWindow::beforeSynchronizing, this, &OpenGlVideoQtQuick::sync, Qt::DirectConnection);
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
    if (!openGlVideoQtQuickRenderer) {
        openGlVideoQtQuickRenderer = new OpenGlVideoQtQuickRenderer();
        connect(window(), &QQuickWindow::beforeRendering, openGlVideoQtQuickRenderer, &OpenGlVideoQtQuickRenderer::render, Qt::DirectConnection);
        connect(window(), &QQuickWindow::afterRendering, this, &OpenGlVideoQtQuick::update, Qt::DirectConnection);
        std::cout << "starting mediaStream with uri: " << this->uri << std::endl;
        MediaStream* camera1 = new MediaStream(this->uri);
        camera1->setFrameUpdater((FrameUpdater *) openGlVideoQtQuickRenderer);
        //TODO: put mutex on std::cout of this thread
        boost::thread mediaThread(&MediaStream::run, camera1);
        openGlVideoQtQuickRenderer->setDimensions(x(),y(),width(),height());
        std::cout <<  "x: " << x() << " y: " << y()<< std::endl;
    }   
}



void OpenGlVideoQtQuickRenderer::updateData(unsigned char**data, int frameWidth, int frameHeight)
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

//TODO: FIX THIS https://stackoverflow.com/a/54773889/6655884
void OpenGlVideoQtQuickRenderer::render()
{
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

            std::cout << "Fragment Shader compilation: " << program->addShaderFromSourceCode(QOpenGLShader::Fragment, tString2) << std::endl;
            std::cout << "Vertex Shader compilation: " << program->addShaderFromSourceCode(QOpenGLShader::Vertex, vString2) << std::endl;

            program->bindAttributeLocation("vertexIn",A_VER);
            program->bindAttributeLocation("textureIn",T_VER);
            std::cout << "program->link() = " << program->link() << std::endl;

            glGenTextures(3, texs);//TODO: ERASE THIS WITH glDeleteTextures
            /*
            GLint params;
            glGetIntegerv(GL_MAX_VIEWPORT_DIMS, &params);
            std::cout << "dimenson: " << params << std::endl;
            */

            GLint dims[2] = {0, 0};
            glGetIntegerv(GL_MAX_VIEWPORT_DIMS, dims);
            std::cout << "dimenson x: " << dims[0] << " dimenson y:" << dims[1] << std::endl;

            this->firstRender = false;
        }
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
    }
}