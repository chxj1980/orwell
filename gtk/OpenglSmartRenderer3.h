#ifndef OPENGLSMARTRENDERER3_H
#define OPENGLSMARTRENDERER3_H
#include "OpenGLArea2.h"
#include "VideoReceiver.h"
#include "Program.h"
#include "DecodedFrame.h"
#include "PixelFormats.h"
#include "Singleton.h"
#include "ThreadSafeDeque.h"
#include <mutex>
#include <condition_variable>
#include "Profiler.h"

class OpenglSmartRenderer3 : public OpenGLArea2, public Profiler //, public VideoReceiver
{
public:
	unsigned char *buffer[3] = {0};
	OpenglSmartRenderer3()
	{
		//set_size_request(640, 360);
		//Singleton::instance()->getStream("cam1").mediaStream->decoder->setVideoReceiver(this);
	}
	void init();
	//int receiveVideo(DecodedFrame& frame);
	bool render(const Glib::RefPtr<Gdk::GLContext> &context);
	void setDecodedFramesFifo(std::shared_ptr<ThreadSafeDeque<std::shared_ptr<DecodedFrame>>> decodedFramesFifo)
	{
		this->decodedFramesFifo = decodedFramesFifo;
	}

	//virtual void on_realize();
	//virtual bool on_draw (const Cairo::RefPtr<Cairo::Context> &);
	//virtual bool on_configure_event (GdkEventConfigure *event);

	void realize() {}

	void glInit();
	void glDraw();

	void run();

	const GLfloat vertices_textures[20] = {
		//vertices            //positions
		-1.0f, -1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, -1.0f, 0.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 0.0f, 1.0f, 0.0f};

	const int VERTEX_POINTER = 0;
	const int FRAGMENT_POINTER = 1;

protected:
	bool firstFrameReceived = false;
	std::unique_ptr<Program> program;
	std::shared_ptr<ThreadSafeDeque<std::shared_ptr<DecodedFrame>>> decodedFramesFifo;

private:
	//void on_glx_init ();
	std::shared_ptr<DecodedFfmpegFrame> decodedFfmpegFrame;
	int frameWidth = 0;
	int frameHeight = 0;
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
	std::mutex mutex;
	std::condition_variable conditionVariable;
	int fps = 0;
	//std::mutex mutex;
	//std::condition_variable conditionVariable;
	//PixelFormats pixelFormats;
	//GLuint program;
	//Display *xdisplay;
	//GLXDrawable drawable;
	//GLXContext context;
};

#endif //OPENGLSMARTRENDERER3_H
