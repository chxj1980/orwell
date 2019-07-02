#include "OpenGlMaterialQQuickItem.h"

//https://github.com/KDE/plasma-framework/blob/e7329b95ed3e654e7b5edb4cf7c044b91f8d4d0a/src/declarativeimports/core/fadingnode.cpp

#define GET_STR(x) #x
#define VERTEX_ATTRIBUTE 3

struct State
{
    public://TEMPORARY
        int frameWidth = 1920;
        int frameHeight = 1080;
        unsigned char *datas[3] = {0};

    private:
        bool firstRender = true;

    public:
        void updateData(unsigned char**data, int frameWidth, int frameHeight)
        {
            this->frameWidth = frameWidth;
            this->frameHeight = frameHeight;

            if (firstRender) {
                datas[0] = new unsigned char[frameWidth*frameHeight];  //Y
                datas[1] = new unsigned char[frameWidth*frameHeight/4];//U
                datas[2] = new unsigned char[frameWidth*frameHeight/4];//V
                firstRender = false;
            }

            memcpy(datas[0], data[0], frameWidth*frameHeight);
            memcpy(datas[1], data[1], frameWidth*frameHeight/4);
            memcpy(datas[2], data[2], frameWidth*frameHeight/4);
        }

        

        int compare(const State *other) const {
            std::cout << "compare called " << std::endl;
            /*
            uint rgb = color.rgba();
            uint otherRgb = other->color.rgba();

            if (rgb == otherRgb) {
                return 0;
            } else if (rgb < otherRgb) {
                return -1;
            } else {
                return 1;
            }
            */
            return -1;
        }
};

class Shader : public QSGSimpleMaterialShader<State>
{
    //QSG_DECLARE_SIMPLE_COMPARABLE_SHADER(Shader, State);
    QSG_DECLARE_SIMPLE_SHADER(Shader, State);
    private:
        QOpenGLFunctions *glFuncs = nullptr;
        GLuint unis[3] = {0};
        GLuint texs[3] = {0};
        QSize m_viewportSize;
        bool firstRender = true;
    public:
        const char *vertexShader() const override {
            return GET_STR(
                        uniform highp mat4 qt_Matrix;
                        attribute vec4 vertexIn;
                        attribute vec2 textureIn;
                        varying vec2 textureOut;
                        void main(void)
                        {
                            gl_Position = qt_Matrix * vertexIn;
                            textureOut = textureIn;
                        }
                );
        }

        const char *fragmentShader() const override {
            return GET_STR(
                        varying vec2 textureOut;
                        uniform sampler2D tex_y;
                        uniform sampler2D tex_u;
                        uniform sampler2D tex_v;
                        uniform lowp float qt_Opacity;
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
                            gl_FragColor = vec4(rgb, 1.0) * qt_Opacity;
                        }
                    );
        }

        QList<QByteArray> attributes() const override
        {
            return {QByteArrayLiteral("vertexIn"), QByteArrayLiteral("textureIn")};
        }

        void initialize()
        {
            if (!program()->isLinked()) {
                return; //shader not linked, exit otherwise we crash, BUG: 336272
            }

            QSGSimpleMaterialShader<State>::initialize();
            glFuncs = QOpenGLContext::currentContext()->functions();
            program()->bind();

            glFuncs->glGenTextures(3, texs);//TODO: delete these textures on exit
        }

        void updateState(const State *state, const State *) override
        {
            //Y
            glFuncs->glBindTexture(GL_TEXTURE_2D, texs[0]);
            glFuncs->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glFuncs->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glFuncs->glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, state->frameWidth, state->frameHeight, 0, GL_RED, GL_UNSIGNED_BYTE, 0);

            //U
            glFuncs->glBindTexture(GL_TEXTURE_2D, texs[1]);
            glFuncs->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glFuncs->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glFuncs->glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, state->frameWidth/2, state->frameHeight / 2, 0, GL_RED, GL_UNSIGNED_BYTE, 0);

            //V
            glFuncs->glBindTexture(GL_TEXTURE_2D, texs[2]);
            glFuncs->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glFuncs->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glFuncs->glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, state->frameWidth / 2, state->frameHeight / 2, 0, GL_RED, GL_UNSIGNED_BYTE, 0);

            glFuncs->glActiveTexture(GL_TEXTURE0);
            glFuncs->glBindTexture(GL_TEXTURE_2D, texs[0]);
            glFuncs->glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, state->frameWidth, state->frameHeight, GL_RED, GL_UNSIGNED_BYTE, state->datas[0]);
            glFuncs->glUniform1i(unis[0], 0);

            glFuncs->glActiveTexture(GL_TEXTURE0+1);
            glFuncs->glBindTexture(GL_TEXTURE_2D, texs[1]); 
            glFuncs->glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, state->frameWidth/2, state->frameHeight / 2, GL_RED, GL_UNSIGNED_BYTE, state->datas[1]);
            glFuncs->glUniform1i(unis[1],1);

            glFuncs->glActiveTexture(GL_TEXTURE0+2);
            glFuncs->glBindTexture(GL_TEXTURE_2D, texs[2]);
            glFuncs->glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, state->frameWidth / 2, state->frameHeight / 2, GL_RED, GL_UNSIGNED_BYTE, state->datas[2]);
            glFuncs->glUniform1i(unis[2], 2);

            glFuncs->glDrawArrays(GL_TRIANGLE_STRIP,0,4);

            //release some things here?
        }

        void resolveUniforms() override
        {
            unis[0] = program()->uniformLocation("tex_y");
            unis[1] = program()->uniformLocation("tex_u");
            unis[2] = program()->uniformLocation("tex_v");
        }
};

class Node: public QSGGeometryNode, public FrameUpdater
{
    public:
        State state;
        Node()
        {            
            material = Shader::createMaterial();
            setMaterial(material);
            setFlag(OwnsMaterial, true);

            QSGGeometry *g = new QSGGeometry(QSGGeometry::defaultAttributes_TexturedPoint2D(), 4);
            QSGGeometry::updateTexturedRectGeometry(g, QRect(), QRect());
            setGeometry(g);
            setFlag(QSGNode::OwnsGeometry, true);

            stream = new MediaStream("rtsp://admin:19929394@192.168.1.178:10554/tcp/av0_0");
            stream->setFrameUpdater((FrameUpdater *) this);
            boost::thread mediaThread(&MediaStream::run, stream);
        }

        void setItem(QQuickItem* item) {
            this->item = item;
        }

        void updateData(unsigned char**data, int frameWidth, int frameHeight)
        {
            
            material->state()->updateData(data, frameWidth, frameHeight);
            markDirty(QSGNode::DirtyMaterial);//is this really needed?
            QMetaObject::invokeMethod(item, &QQuickItem::update, Qt::QueuedConnection);
        }

    private:
        QSGSimpleMaterial<State> *material;
        MediaStream* stream;
        QQuickItem* item;

};

QSGNode * OpenGlMaterialQQuickItem::updatePaintNode(QSGNode *node, UpdatePaintNodeData *) //override
{
    Node *n = static_cast<Node *>(node);
    if (!node)
        n = new Node();
    n->setItem(this);
    QSGGeometry::updateTexturedRectGeometry(n->geometry(), boundingRect(), QRectF(0, 0, 1, 1));
    //This is how we change things, because updatePaintNode is the safe place to do it 
    //static_cast<QSGSimpleMaterial<State>*>(n->material())->state()->color = m_color;
    

    n->markDirty(QSGNode::DirtyGeometry | QSGNode::DirtyMaterial);//what is this?
    return n;
}

//#include "OpenGlMaterialQtQuick.moc"