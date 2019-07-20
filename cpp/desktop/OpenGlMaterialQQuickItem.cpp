#include "OpenGlMaterialQQuickItem.h"

//https://github.com/KDE/plasma-framework/blob/e7329b95ed3e654e7b5edb4cf7c044b91f8d4d0a/src/declarativeimports/core/fadingnode.cpp

#define GET_STR(x) #x
#define VERTEX_ATTRIBUTE 3

struct State
{
    public:
        int frameWidth = 0;
        int frameHeight = 0;
        unsigned char *datas[3] = {0};
        unsigned int frameNumber = 0;
        //for debug only
        std::string uri;

    private:
        bool firstRender = true;
    public:
        void updateData(unsigned char**data, int frameWidth, int frameHeight)
        {
            if (this->frameWidth!=frameWidth | this->frameHeight!=frameHeight) {
                //Should trigger a recreation of everything here. But will it ever be needed? Do frames change size in videos?
            }
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
           this->frameNumber++;//No overflow occurs, because it's unsigned int  
        }

        int compare(const State *olderState) const {
            if (this->frameNumber == olderState->frameNumber) {
                return 0;
            } else {
                return 1;
            }
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
            //return -1;
        }
};

class Shader : public QSGSimpleMaterialShader<State>
{
    QSG_DECLARE_SIMPLE_COMPARABLE_SHADER(Shader, State);
    //QSG_DECLARE_SIMPLE_SHADER(Shader, State);
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
            //TODO: support packed YUV
            //https://github.com/yuan631111976/qtavplayer/blob/master/packed.frag 
            //This looks promising: https://github.com/yuan631111976/qtavplayer/blob/b4394d620076cedb05fdc006daee2918baeb73d9/AVOutput.h
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

        void updateState(const State *state, const State *oldState) override
        {
            /*
            if (oldState && (state->frameWidth != oldState->frameWidth | state->frameHeight != oldState->frameHeight)) {
                //Forces glTexImage2D to be redone
                firstRender = true;
            }
            */
           //TODO: only update if states change, migth be the solution to that decoding vertical lines
            if (state->frameWidth>0 && state->frameHeight>0) {
                if (firstRender) {
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
                    firstRender = false;
                }
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

                /*
                glFuncs->glActiveTexture(GL_TEXTURE0);
                glFuncs->glBindTexture(GL_TEXTURE_2D, 0);
                glFuncs->glActiveTexture(GL_TEXTURE0+1);
                glFuncs->glBindTexture(GL_TEXTURE_2D, 0);
                glFuncs->glActiveTexture(GL_TEXTURE0+2);
                glFuncs->glBindTexture(GL_TEXTURE_2D, 0);
                */
                //release some things here?
            }
        }

        void resolveUniforms() override
        {
            unis[0] = program()->uniformLocation("tex_y");
            unis[1] = program()->uniformLocation("tex_u");
            unis[2] = program()->uniformLocation("tex_v");
        }
};

class Node: public QSGGeometryNode, public VideoReceiver
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
            setFlag(OwnsGeometry, true);
        }

        ~Node() {
            if (this->item) {
                if (this->item->id!=nullptr) {
                    Glue::instance()->get(item->id).mediaStream->ffmpegDecoder->setVideoReceiver(nullptr);
                } else if (this->item->p_id!=nullptr) {
                    Glue::instance()->get(item->p_id).mediaStream->ffmpegDecoder->setVideoReceiver(nullptr);
                }else {
                    std::cout << "ERROR, id not set or not set yet " << std::endl;
                }
            } else {
                    std::cout << "ERROR, set item first" << std::endl;
            }
        }

        void setItem(OpenGlMaterialQQuickItem* item) {
            this->item = item;
        }

        void setUri(std::string uri) {
            this->uri = uri;
            material->state()->uri = uri;
        }

        void beginReceiving() {
            if (this->item) {
                if (this->item->id!=nullptr) {
                    Glue::instance()->get(item->id).mediaStream->ffmpegDecoder->setVideoReceiver(this);
                } else if (this->item->p_id!=nullptr) {
                    if(Glue::instance()->get(item->p_id).mediaStream==nullptr) {
                        //TODO (VERY IMPORTANT): retry every x millisseconds until we have a definition, or find a better solution
                        std::cout << "/1/1/1/1/1/1/1/1/11/1/1 ERROR: mediaStream is undefined for " << this->item->p_id.toStdString() << std::endl;
                    } else {
                        Glue::instance()->get(item->p_id).mediaStream->ffmpegDecoder->setVideoReceiver(this);
                    }
                }else {
                    std::cout << "ERROR, id not set or not set yet " << std::endl;
                }
            } else {
                    std::cout << "ERROR, set item first" << std::endl;
            }
        }

        void receiveVideo(unsigned char**videoBuffer, int frameWidth, int frameHeight) {
            material->state()->updateData(videoBuffer, frameWidth, frameHeight);
            markDirty(QSGNode::DirtyMaterial);//is this really needed?
            QMetaObject::invokeMethod(item, &QQuickItem::update, Qt::QueuedConnection);
        }

    private:
        QSGSimpleMaterial<State> *material;
        MediaStream* stream;
        OpenGlMaterialQQuickItem* item;
        std::string uri;

};

QSGNode * OpenGlMaterialQQuickItem::updatePaintNode(QSGNode *qsgNode, UpdatePaintNodeData *) //override
{
    node = static_cast<Node *>(qsgNode);
    if (!qsgNode) {
        node = new Node();
        node->setItem(this);
        /*
        if (!this->getUri().isEmpty()) 
            node->setUri(this->uri.toStdString()); //TODO: How do I know that when updatePaintNode is called, the object will already have a defined uri?
        else if (!this->getPUri().isEmpty())
            node->setUri(this->p_uri.toStdString());
        else 
            std::cout << "NO URI PASSED TO OBJECT" << std::endl;
        */
        node->beginReceiving();
    }
    QSGGeometry::updateTexturedRectGeometry(node->geometry(), boundingRect(), QRectF(0, 0, 1, 1));
    //static_cast<QSGSimpleMaterial<State>*>(n->material())->state()->color = m_color;
    

    node->markDirty(QSGNode::DirtyGeometry | QSGNode::DirtyMaterial);//what is this?
    return node;
}

