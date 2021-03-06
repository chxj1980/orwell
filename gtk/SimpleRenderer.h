#ifndef SIMPLERENDERER_H
#define SIMPLERENDERER_H
#include "OpenGLArea2.h"
#include "VideoReceiver.h"
#include "Program.h"
#include "DecodedFrame.h"
#include "PixelFormats.h"
#include "Singleton.h"

class SimpleRenderer : public OpenGLArea2
{
public:
	unsigned char *buffer[3] = {0};
	SimpleRenderer()
	{
		//set_size_request(640, 360);
		//Singleton::instance()->getStream("cam1").mediaStream->decoder->setVideoReceiver(this);

	}
	void init();
	//int receiveVideo(Frame *frame);
	bool render(const Glib::RefPtr<Gdk::GLContext> &context);

	//virtual void on_realize();
	//virtual bool on_draw (const Cairo::RefPtr<Cairo::Context> &);
	//virtual bool on_configure_event (GdkEventConfigure *event);

	void glInit();
	void glDraw();

	const GLfloat vertices_textures[20] = {
		//vertices            //positions
		-1.0f, -1.0f, 0.0f,    0.0f, 1.0f,
		1.0f, -1.0f, 0.0f,     1.0f, 1.0f,
		-1.0f, 1.0f, 0.0f,     0.0f, 0.0f,
		1.0f, 1.0f, 0.0f,      1.0f, 0.0f};


	const int VERTEX_POINTER = 0;
	const int FRAGMENT_POINTER = 1;
protected:
	bool firstFrameReceived = false;
	std::unique_ptr<Program> program;

private:
	//void on_glx_init ();
	DecodedFrame *frame;
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

#endif //SIMPLERENDERER_H
