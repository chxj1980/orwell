#ifndef OPENGLSMARTRENDERER3_H
#define OPENGLSMARTRENDERER3_H
#include "OpenGLArea2.h"
#include "VideoReceiver.h"
#include "Program.h"
#include "Frame.h"
#include "PixelFormats.h"
#include "Singleton.h"
#include "ThreadSafeDeque.h"
#include <mutex>
#include <condition_variable>

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
	void setDecodedFramesFifo(std::shared_ptr<ThreadSafeDeque<Frame>> decodedFramesFifo)
	{
		this->decodedFramesFifo = decodedFramesFifo;
	}

	//virtual void on_realize();
	//virtual bool on_draw (const Cairo::RefPtr<Cairo::Context> &);
	//virtual bool on_configure_event (GdkEventConfigure *event);

	void glInit();
	void glDraw();

	void run();

	const GLfloat vertices[12] = {
		-1.0f, -1.0f, 0.0f, //bottom left
		1.0f, -1.0f, 0.0f,  //bottom rigth
		-1.0f, 1.0f, 0.0f,  //top left
		1.0f, 1.0f, 0.0f};  //top rigth

	//not used
	const unsigned int indices[6] = {
		// note that we start from 0!
		0, 1, 2, // first triangle
		2, 3, 1  // second triangle
	};

	const GLfloat vertices_textures[20] = {
		//vertices            //positions
		-1.0f, -1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, -1.0f, 0.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 0.0f, 1.0f, 0.0f};

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
	std::shared_ptr<ThreadSafeDeque<Frame>> decodedFramesFifo;

private:
	//void on_glx_init ();
	Frame frame;
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
	GLint textureLocation[TEXTURE_NUMBER];
	GLuint pixelBufferObjects[TEXTURE_NUMBER];
	unsigned int vertexBufferObject, vertexArrayObject, TBO, EBO;
	GLint vextexInLocation;
	GLint textureInLocation;
	//std::mutex mutex;
	//std::condition_variable conditionVariable;
	//PixelFormats pixelFormats;
	//GLuint program;
	//Display *xdisplay;
	//GLXDrawable drawable;
	//GLXContext context;
};

#endif //OPENGLSMARTRENDERER3_H
