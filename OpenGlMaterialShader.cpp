#include <qguiapplication.h>
#include <qsgmaterial.h>
#include <qsgnode.h>
#include <qquickitem.h>
#include <qquickview.h>
#include <qsgsimplerectnode.h>
#include <qsgsimplematerial.h>
#include <QOpenGLContext>
#include <QOpenGLFunctions>

#define GET_STR(x) #x

struct State
{
    QColor color;

    int compare(const State *other) const {
        uint rgb = color.rgba();
        uint otherRgb = other->color.rgba();

        if (rgb == otherRgb) {
            return 0;
        } else if (rgb < otherRgb) {
            return -1;
        } else {
            return 1;
        }
    }
};

class Shader : public QSGSimpleMaterialShader<State>
{
    QSG_DECLARE_SIMPLE_COMPARABLE_SHADER(Shader, State);
public:

    const char *vertexShader() const override {
        return GET_STR(
                    attribute vec4 vertexIn;
                    attribute vec2 textureIn;
                    varying vec2 textureOut;
                    void main(void)
                    {
                        gl_Position = vertexIn;
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
    }

    QList<QByteArray> attributes() const override
    {
        return QList<QByteArray>() << "aVertex" << "aTexCoord";
    }

    void initialize()
    {
        if (!program()->isLinked()) {
            return; //shader not linked, exit otherwise we crash, BUG: 336272
        }

        QSGSimpleMaterialShader<State>::initialize(); //What is this?
        glFuncs = QOpenGLContext::currentContext()->functions();
        program()->bind();
        
        datas[0] = new unsigned char[frameWidth*frameHeight];	//Y
        datas[1] = new unsigned char[frameWidth*frameHeight/4];	//U
        datas[2] = new unsigned char[frameWidth*frameHeight/4]; //V

        unis[0] = program()->uniformLocation("tex_y");
        unis[1] = program()->uniformLocation("tex_u");
        unis[2] = program()->uniformLocation("tex_v");

        glFuncs->glGenTextures(3, texs);
    }

    void updateState(const State *state, const State *) override
    {
        //TODO: do verification of old state and new state here. Don't know why but do it, I think it has to do with performance

        //Y
        glFuncs->glBindTexture(GL_TEXTURE_2D, texs[0]);
        glFuncs->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFuncs->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glFuncs->glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, frameWidth, frameHeight, 0, GL_RED, GL_UNSIGNED_BYTE, 0);

        //U
        glFuncs->glBindTexture(GL_TEXTURE_2D, texs[1]);
        glFuncs->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFuncs->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glFuncs->glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, frameWidth/2, frameHeight / 2, 0, GL_RED, GL_UNSIGNED_BYTE, 0);

        //V
        glFuncs->glBindTexture(GL_TEXTURE_2D, texs[2]);
        glFuncs->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFuncs->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glFuncs->glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, frameWidth / 2, frameHeight / 2, 0, GL_RED, GL_UNSIGNED_BYTE, 0);

        glFuncs->glActiveTexture(GL_TEXTURE0);
        glFuncs->glBindTexture(GL_TEXTURE_2D, texs[0]);
        glFuncs->glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, frameWidth, frameHeight, GL_RED, GL_UNSIGNED_BYTE, datas[0]);
        glFuncs->glUniform1i(unis[0], 0);

        glFuncs->glActiveTexture(GL_TEXTURE0+1);
        glFuncs->glBindTexture(GL_TEXTURE_2D, texs[1]); 
        glFuncs->glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, frameWidth/2, frameHeight / 2, GL_RED, GL_UNSIGNED_BYTE, datas[1]);
        glFuncs->glUniform1i(unis[1],1);

        glFuncs->glActiveTexture(GL_TEXTURE0+2);
        glFuncs->glBindTexture(GL_TEXTURE_2D, texs[2]);
        glFuncs->glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, frameWidth / 2, frameHeight / 2, GL_RED, GL_UNSIGNED_BYTE, datas[2]);
        glFuncs->glUniform1i(unis[2], 2);

        glFuncs->glDrawArrays(GL_TRIANGLE_STRIP,0,4);

        //release some things here
    }

    void resolveUniforms() override
    {
        //id_color = program()->uniformLocation("color");
    }

private:
    int id_color;
    QOpenGLFunctions *glFuncs = nullptr;
    GLuint unis[3] = {0};
    GLuint texs[3] = {0};
    QSize m_viewportSize;
    unsigned char *datas[3] = {0};
    bool firstRender = true;
    int frameWidth = 1920;
    int frameHeight = 1080;
};

class Node : public QSGGeometryNode
{
    public:
        Node(QSGTexture *source, QSGTexture *target):m_source(source),m_target(target)
        {
            QSGSimpleMaterial<Shader> *material = Shader::createMaterial();
            setMaterial(material);
            setFlag(OwnsMaterial, true);
            //setProgress(1.0);

            /*
            QSGGeometry *g = new QSGGeometry(QSGGeometry::defaultAttributes_TexturedPoint2D(), 4);
            QSGGeometry::updateTexturedRectGeometry(g, QRect(), QRect());
            setGeometry(g);
            setFlag(QSGNode::OwnsGeometry, true);
            */
        }
        void updateData(unsigned char**data, int frameWidth, int frameHeight)
        {
            material->frameWidth = frameWidth;
            material->frameHeight = frameHeight;

            //Before first render, datas pointer isn't even created yet
            firstRender = false; //temporary
            if (!firstRender) {
                memcpy(material->datas[0], data[0], frameWidth*frameHeight);
                memcpy(material->datas[1], data[1], frameWidth*frameHeight/4);
                memcpy(material->datas[2], data[2], frameWidth*frameHeight/4);
            }
        }

    private:
        QScopedPointer<QSGTexture> m_source;
        QScopedPointer<QSGTexture> m_target;
}


class Item : public QQuickItem
{
    Q_OBJECT

    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(QString uri WRITE setUri)// NOTIFY uriChanged)

    public:
        std::string uri;

        QSGNode *updatePaintNode(QSGNode *node, UpdatePaintNodeData *) override
        {
            Node *n = static_cast<Node *>(node);
            if (!node)
                n = new Node();

            //QSGGeometry::updateTexturedRectGeometry(n->geometry(), boundingRect(), QRectF(0, 0, 1, 1));
            //static_cast<QSGSimpleMaterial<State>*>(n->material())->state()->color = m_color;

            n->markDirty(QSGNode::DirtyGeometry | QSGNode::DirtyMaterial);//what is this?

            return n;
        }

        Item()
        {
            setFlag(ItemHasContents, true);
        }

        void setUri(const QString &a) {
            uri = a.toStdString();
        }

        void setColor(const QColor &color) {
            if (m_color != color) {
                m_color = color;
                emit colorChanged();
                update();
            }
        }

        QColor color() const {
            return m_color;
        }

    signals:
        void colorChanged();

    private:
        QColor m_color;
        
};