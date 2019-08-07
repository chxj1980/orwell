#ifndef OPENGLSMARTRENDERER2_H
#define OPENGLSMARTRENDERER2_H

#include "OpenGLArea.h"
#include "PixelFormats.h"
#include "Singleton.h"


class OpenglSmartRenderer2 : public OpenGLArea, public VideoReceiver
{
public:
	unsigned char *buffer[3] = {0};
	OpenglSmartRenderer2()
	{
		std::cout << "OpenglSmartRenderer2 constructor" << std::endl;

		set_size_request(640, 360);
		Singleton::instance()->getStream("cam1").mediaStream->decoder->setVideoReceiver(this);
		std::cout << "OpenglSmartRenderer2 constructor end" << std::endl;

	}
	void init();
	int receiveVideo(Frame *frame);

	//virtual void on_realize();
	//virtual bool on_draw (const Cairo::RefPtr<Cairo::Context> &);
	//virtual bool on_configure_event (GdkEventConfigure *event);

	void glInit();
	void glDraw();

	const GLfloat vertices[8] = {
		-1.0f, -1.0f,
		1.0f, -1.0f,
		-1.0f, 1.0f,
		1.0f, 1.0f};

	const GLfloat textureCoordinates[8] = {
		0.0f, 1.0f,
		1.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f};

	const int VERTEX_POINTER = 0;
	const int FRAGMENT_POINTER = 1;

private:
	//bool firstFrameReceived = false;
	//void on_glx_init ();
	Frame *frame;
	int frameWidth = 0;
	int frameHeight = 0;
	GLuint unis[3] = {0};
	GLuint texs[3] = {0};
	int format;
	bool firstRun = true;
	bool initiatedTextures = false;
	bool initiatedFrameBufferObjects = false;
	static const int TEXTURE_NUMBER = 3;
	GLuint textureId[TEXTURE_NUMBER];
	//int textureFormat;
	int textureLocation[TEXTURE_NUMBER];
	//PixelFormats pixelFormats;
	//GLuint program;
	//Display *xdisplay;
	//GLXDrawable drawable;
	//GLXContext context;
};

#endif //OPENGLSMARTRENDERER2_H
