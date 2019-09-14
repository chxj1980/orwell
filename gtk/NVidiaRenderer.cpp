#include "NVidiaRenderer.h"
#include "SLog.h"

SLOG_CATEGORY("NVidiaRenderer")

const std::string vertexShaderSource =
#include "nvidia.vert"
	;
const std::string fragmentShaderSource =
#include "nvidia.frag"
	;

#define TEST_CONDITION(condition, message, errorCode)      \
	if (condition)                                         \
	{                                                      \
		LOG << message;									   \
	}

static const float kVertices[] = {
	-1.f,
	-1.f,
	1.f,
	-1.f,
	-1.f,
	1.f,
	1.f,
	1.f,
};
static const float kTextureCoords[] = {
	0.0f,
	1.0f,
	1.0f,
	1.0f,
	0.0f,
	0.0f,
	1.0f,
	0.0f,
};

void NVidiaRenderer::init()
{
}

PFNEGLCREATEIMAGEKHRPROC NVidiaRenderer::eglCreateImageKHR;
PFNEGLDESTROYIMAGEKHRPROC NVidiaRenderer::eglDestroyImageKHR;
PFNEGLCREATESYNCKHRPROC NVidiaRenderer::eglCreateSyncKHR;
PFNEGLDESTROYSYNCKHRPROC NVidiaRenderer::eglDestroySyncKHR;
PFNEGLCLIENTWAITSYNCKHRPROC NVidiaRenderer::eglClientWaitSyncKHR;
PFNEGLGETSYNCATTRIBKHRPROC NVidiaRenderer::eglGetSyncAttribKHR;
PFNGLEGLIMAGETARGETTEXTURE2DOESPROC NVidiaRenderer::glEGLImageTargetTexture2DOES;

void NVidiaRenderer::realize()
{
	std::cout << "NVidiaRenderer::realize" << std::endl;
	GtkWidget *widget = glArea.Widget::gobj();
	EGLConfig egl_config;
	EGLint n_config;
	EGLint attributes[] = {EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT,
						   EGL_NONE};

	eglDisplay = eglGetDisplay((EGLNativeDisplayType)gdk_x11_display_get_xdisplay(glArea.get_display()->gobj()));

	eglInitialize(&eglDisplay, NULL, NULL);
	eglChooseConfig(&eglDisplay, attributes, &egl_config, 1, &n_config);
	eglBindAPI(EGL_OPENGL_API);
	eglSurface = eglCreateWindowSurface(&eglDisplay, egl_config, gdk_x11_window_get_xid(glArea.get_window()->gobj()), NULL);
	eglContext = eglCreateContext(&eglDisplay, egl_config, EGL_NO_CONTEXT, NULL);


}

void NVidiaRenderer::run()
{
	/*
		ThreadSafeDeque decodedFramesFifo is a nice object because it will only pop_front when it has data.
		This way, if decodedFramesFifo is empty, this rendering loop will wait, which is good, no CPU time is wasted.
	*/
	LOG << "NVidiaRenderer run called";
	if (!decodedFramesFifo)
	{
		std::cerr << "No decodedFramesFifo setted for this renderer" << std::endl;
		return;
	}
	int i = 0;
	//TODO: run every loop?
	eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext);
	while (true)
	{
		//std::unique_lock<std::mutex> lock{mutex};
		//TODO: certify that the operation below is MOVING the frame to here, not copying it
		auto decodedNvFrame =  std::dynamic_pointer_cast<DecodedNvFrame>(decodedFramesFifo->pop_front());
		/* 
		    Since the frame is gone from the fifo, it only exists here. We move it to the renderer and then we 
		    don't need to worry with its lifetime. When another frame arrives, it automatically deletes this one
		    TODO: verify if this indeed happens
		*/
		std::unique_lock<std::mutex> lk{mutex};
		this->decodedNvFrame = decodedNvFrame;

		//lk.unlock();
		if (!firstFrameReceived)
			firstFrameReceived = true;
		i++;
		//std::cout << i << std::endl;
		queue_draw();
		//std::cout << "waiting" << std::endl;
		//conditiconditionVariable.wait(lock);
		//std::cout << "waited" << std::endl;
	}
}

void NVidiaRenderer::glInit()
{
}

static void err(std::string lineInformation)
{
	std::cout << lineInformation << std::endl;
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR)
	{
		std::cerr << ">>>>>>>>>>>>>>>>OpenGL error: " << err << std::endl;
	}
}

bool NVidiaRenderer::render(const Glib::RefPtr<Gdk::GLContext> &context)
{
	std::unique_lock<std::mutex> lk{mutex};
	try
	{
		//std::cout << "gonna render" << std::endl;

		glArea.throw_if_error();

		glDraw();

		glFinish();
		//std::cout << "gonna notify" << std::endl;
		//conditionVariable.notify_one();
		//std::cout << "did notify" << std::endl;
		//std::cout << "did render" << std::endl;

		return true;
	}
	catch (const Gdk::GLError &gle)
	{
		std::cerr << "An error occurred in the render callback of the GLArea" << std::endl;
		std::cerr << gle.domain() << "-" << gle.code() << "-" << gle.what() << std::endl;
		//conditionVariable.notify_one();
		return false;
	}
}

void NVidiaRenderer::glDraw()
{
	/*
		By putting everythng inside a bit if (firstFrameReceived),
		we only create the resources after we know the size and pixel format
		of the received frame. 
	*/

	//TODO: discover why, in the beggining, frame has non setted components (0 for integer, for example)
	if (this->firstFrameReceived && decodedNvFrame->getWidth() != 0)
	{
		if (this->firstRun)
		{
			std::cout << "firstRun of NVidiaRenderer" << std::endl;

			uint32_t pos_location = 0;
			uint32_t tc_location = 0;
			glEnable(GL_SCISSOR_TEST);
			program = std::make_unique<Program>();
			Shader vertexShader(ShaderType::Vertex);
			vertexShader.load_from_string(vertexShaderSource);
			//) Shader(ShaderType::Vertex, "video.vert")
			program->attach_shader(vertexShader);
			/*
					TODO: be careful if renderers will be reused for other video formats. 
					I think it shouldn't, but if it's done, make everything be redone, including
					planar vs packed shader options. Maybe create a reset method?
				*/

			Shader fragmentShader(ShaderType::Fragment);
			fragmentShader.load_from_string(fragmentShaderSource);
			program->attach_shader(fragmentShader);

			program->link();
			pos_location = glGetAttribLocation(program->get_id(), "in_pos");

			glEnableVertexAttribArray(pos_location);
			glVertexAttribPointer(pos_location, 2, GL_FLOAT, GL_FALSE, 0, kVertices);

			tc_location = glGetAttribLocation(program->get_id(), "in_tc");

			glEnableVertexAttribArray(tc_location);
			glVertexAttribPointer(tc_location, 2, GL_FLOAT, GL_FALSE, 0,
								  kTextureCoords);

			glActiveTexture(GL_TEXTURE0);
			glUniform1i(glGetUniformLocation(program->get_id(), "texSampler"), 0);

			//To be done
			if (!initiatedFrameBufferObjects)
			{
				int viewport[4];

				glGetIntegerv(GL_VIEWPORT, viewport);
				glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
				glScissor(viewport[0], viewport[1], viewport[2], viewport[3]);

				glGenTextures(1, &texture_id);

				glBindTexture(GL_TEXTURE_EXTERNAL_OES, texture_id);
				initiatedFrameBufferObjects = true;
			}

			firstRun = false;
		}

		program->use();
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		/*
		EGLImageKHR hEglImage;
		bool frame_is_late = false;

		EGLSyncKHR egl_sync;
		int iErr;
		//NVDecoderReusableBuffer *nVDecoderReusableBuffer = dynamic_cast<NVDecoderReusableBuffer *>(frame.reusableBuffer.get());
		hEglImage = NvEGLImageFromFd(NULL, decodedNvFrame->nvBuffer->planes[0].fd);
		TEST_CONDITION(!hEglImage, "Could not get EglImage from fd. Not rendering", 0)
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_EXTERNAL_OES, texture_id);
		glEGLImageTargetTexture2DOES(GL_TEXTURE_EXTERNAL_OES, hEglImage);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		iErr = glGetError();
		TEST_CONDITION(iErr != GL_NO_ERROR, "glDrawArrays arrays failed:", iErr)
		egl_sync = eglCreateSyncKHR(&eglDisplay, EGL_SYNC_FENCE_KHR, NULL);
		TEST_CONDITION(egl_sync == EGL_NO_SYNC_KHR, "eglCreateSyncKHR() failed", 0)
		
		
		eglSwapBuffers(&eglDisplay, &eglSurface);
		TEST_CONDITION(eglGetError() != EGL_SUCCESS, "Got Error in eglSwapBuffers ", eglGetError())

		iErr = eglClientWaitSyncKHR(&eglDisplay, egl_sync,
									EGL_SYNC_FLUSH_COMMANDS_BIT_KHR, EGL_FOREVER_KHR);
		TEST_CONDITION(iErr == EGL_FALSE, "eglClientWaitSyncKHR failed!", 0)

		iErr = eglDestroySyncKHR(&eglDisplay, egl_sync);
		TEST_CONDITION(iErr != EGL_TRUE, "eglDestroySyncKHR failed!", 0)

		NvDestroyEGLImage(&eglDisplay, hEglImage);

		if(iErr != EGL_TRUE)
			abort();
		*/
		
		//profiler.finishProcessing(0, frame_is_late);
	}
}