#ifndef XVIDEOWIDGET_H
#define XVIDEOWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QVector3D>
#include <QMatrix4x4>
#include <QTime>
#include <QVector>
#include <QOpenGLTexture>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include "FrameUpdater.h"
//https://github.com/zhuyong006/QT/blob/cd42d0011717d4019764e6520cad739e9c534be8/opengl/qt-opengl-yuv/XVideoWidget.cpp

class XVideoWidget: public QOpenGLWidget, protected QOpenGLFunctions, public FrameUpdater
{
    Q_OBJECT
public:
    XVideoWidget(QWidget * parent = 0);
    ~XVideoWidget();
    void updateData(unsigned char**data);
protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
private:
    QOpenGLShaderProgram program;

    GLuint unis[3] = {0};
    GLuint texs[3] = { 0 };

    unsigned char *datas[3] = { 0 };
    //TODO: make this variable according to video data
    int width = 1920;
    int height = 1080;
private:

    QOpenGLFunctions *m_F;
};

#endif // XVIDEOWIDGET_H
