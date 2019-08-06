#ifndef OpenglYUV_H
#define OpenglYUV_H

#include "OpenGLArea.h"
#include "PixelFormats.h"

class OpenglSmartRenderer2 : public OpenGLArea
{
public:
	unsigned char *buffer[3] = {0};
	OpenglSmartRenderer2()
	{

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
	GLuint textureId[TEXTURE_NUMBER];
	//PixelFormats pixelFormats;
	//GLuint program;
	//Display *xdisplay;
	//GLXDrawable drawable;
	//GLXContext context;
};

#endif //OpenglYUV_H
