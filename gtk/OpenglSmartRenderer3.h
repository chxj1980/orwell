#ifndef OPENGLSMARTRENDERER3_H
#define OPENGLSMARTRENDERER3_H
#include "OpenGLArea2.h"
#include "VideoReceiver.h"
#include "Program.h"
#include "Frame.h"
#include "PixelFormats.h"
#include "Singleton.h"

class OpenglSmartRenderer3 : public OpenGLArea2, public VideoReceiver
{
public:
	unsigned char *buffer[3] = {0};
	OpenglSmartRenderer3()
	{
		//set_size_request(640, 360);
		Singleton::instance()->getStream("cam1").mediaStream->decoder->setVideoReceiver(this);

	}
	void init();
	int receiveVideo(Frame *frame);
	bool render(const Glib::RefPtr<Gdk::GLContext> &context);

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
protected:
	bool firstFrameReceived = false;
	std::unique_ptr<Program> program;

private:
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

#endif //OPENGLSMARTRENDERER3_H
