#include "NVidiaFrameBufferRenderer.h"
#include "SLog.h"

SLOG_CATEGORY("NVidiaFrameBufferRenderer")

const std::string vertexShaderSource =
#include "nvidia2.vert"
	;
const std::string fragmentShaderSource =
#include "nvidia2.frag"
	;

#define TEST_CONDITION(condition, message, errorCode) \
	if (condition)                                    \
	{                                                 \
		LOG << message;                               \
	}

static void assertOpenGLError(const std::string &msg)
{
	GLenum error = glGetError();

	if (error != GL_NO_ERROR)
	{
		std::stringstream s;
		s << "OpenGL error 0x" << std::hex << error << " at " << msg;
		throw std::runtime_error(s.str());
	}
}

static void assertEGLError(const std::string &msg)
{
	EGLint error = eglGetError();

	if (error != EGL_SUCCESS)
	{
		std::stringstream s;
		s << "EGL error 0x" << std::hex << error << " at " << msg;
		throw std::runtime_error(s.str());
	}
}

void NVidiaFrameBufferRenderer::init()
{
	LOG << "init!";
	eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
	assertEGLError("eglGetDisplay");

	eglInitialize(eglDisplay, nullptr, nullptr);
	assertEGLError("eglInitialize");

	eglChooseConfig(eglDisplay, nullptr, &eglConfig, 1, &numConfigs);
	assertEGLError("eglChooseConfig");

	eglBindAPI(EGL_OPENGL_API);
	assertEGLError("eglBindAPI");

	eglContext = eglCreateContext(eglDisplay, eglConfig, EGL_NO_CONTEXT, NULL);
	assertEGLError("eglCreateContext");

	//surface = eglCreatePbufferSurface(eglDisplay, eglConfig, nullptr);
	//assertEGLError("eglCreatePbufferSurface");

	eglMakeCurrent(eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, eglContext);
	assertEGLError("eglMakeCurrent");
}

PFNEGLCREATEIMAGEKHRPROC NVidiaFrameBufferRenderer::eglCreateImageKHR;
PFNEGLDESTROYIMAGEKHRPROC NVidiaFrameBufferRenderer::eglDestroyImageKHR;
PFNEGLCREATESYNCKHRPROC NVidiaFrameBufferRenderer::eglCreateSyncKHR;
PFNEGLDESTROYSYNCKHRPROC NVidiaFrameBufferRenderer::eglDestroySyncKHR;
PFNEGLCLIENTWAITSYNCKHRPROC NVidiaFrameBufferRenderer::eglClientWaitSyncKHR;
PFNEGLGETSYNCATTRIBKHRPROC NVidiaFrameBufferRenderer::eglGetSyncAttribKHR;
PFNGLEGLIMAGETARGETTEXTURE2DOESPROC NVidiaFrameBufferRenderer::glEGLImageTargetTexture2DOES;

void NVidiaFrameBufferRenderer::run()
{
	/*
		ThreadSafeDeque decodedFramesFifo is a nice object because it will only pop_front when it has data.
		This way, if decodedFramesFifo is empty, this rendering loop will wait, which is good, no CPU time is wasted.
	*/
	LOG << "NVidiaFrameBufferRenderer run called";
	init();
	//TODO: run every loop?
	while (true)
	{
		auto decodedNvFrame = std::dynamic_pointer_cast<DecodedNvFrame>(onAcquireNewDecodedFrame());

		std::unique_lock<std::mutex> lock{mutex};
		this->decodedNvFrame = decodedNvFrame;
		if (!firstFrameReceived)
			firstFrameReceived = true;
		lock.unlock();

		drawerDispatcher.emit();
	}
}

bool NVidiaFrameBufferRenderer::render(const Glib::RefPtr<Gdk::GLContext> &context)
{
	std::unique_lock<std::mutex> lock{mutex};
	glDraw();
	glFinish();
}

const GLfloat vertices_textures[20] = {
	//vertices            //positions
	-1.0f, -1.0f, 0.0f, 0.0f, 1.0f,
	1.0f, -1.0f, 0.0f, 1.0f, 1.0f,
	-1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
	1.0f, 1.0f, 0.0f, 1.0f, 0.0f};

static void glGetErr(std::string line)
{
	int e = glGetError();
	if (e != GL_NO_ERROR)
		std::cout << "got error " << e << " in line " << line << std::endl;
}

static unsigned char *d = new unsigned char[512 * 512];
static unsigned char *r = new unsigned char[512 * 512*4];

void NVidiaFrameBufferRenderer::glDraw()
{
	/*
		By putting everythng inside a bit if (firstFrameReceived),
		we only create the resources after we know the size and pixel format
		of the received frame. 
	*/

	//TODO: discover why, in the beggining, frame has non setted components (0 for integer, for example)
	if (decodedNvFrame && this->firstFrameReceived && decodedNvFrame->getWidth() != 0)
	{
		printf("glDraw if\n"); 
		if (this->firstRun)
		{
			LOG << "firstRun of NVidiaFrameBufferRenderer";

			program = std::make_unique<Program>();
			Shader vertexShader(ShaderType::Vertex);
			vertexShader.load_from_string(vertexShaderSource);
			//std::cout << "vertexShaderSource: " << vertexShaderSource;
			//std::cout << "fragmentShaderSource: " << fragmentShaderSource;

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
			//TODO: https://stackoverflow.com/questions/15639957/glgetattriblocation-returns-1-when-retrieving-existing-shader-attribute
			vextexInLocation = glGetAttribLocation(program->get_id(), "aPos");
			//vextexInLocation = 0;
			textureInLocation = glGetAttribLocation(program->get_id(), "aTexCoord");

			//textureInLocation = 1;
			printf("vextexInLocation: %i\n", vextexInLocation);
			printf("textureInLocation: %i\n", textureInLocation);

			glGenVertexArrays(1, &vertexArrayObject);
			glGenBuffers(1, &vertexBufferObject);
			//glGenBuffers(3, pixelBufferObjects);

			glBindVertexArray(vertexArrayObject);

			glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_textures), vertices_textures, GL_STATIC_DRAW);

			glVertexAttribPointer(vextexInLocation, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void *)0);
			glEnableVertexAttribArray(vextexInLocation);

			glVertexAttribPointer(textureInLocation, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void *)(3 * sizeof(GLfloat)));
			glEnableVertexAttribArray(textureInLocation);

			texLocation = glGetUniformLocation(program->get_id(), "tex");
			printf("texLocation: %i\n", texLocation);

			glGetErr("294");

			//VERY IMPORTANT, use the progrm before glUniform1i
			program->use();
			glGetErr("300");
			
			glGenFramebuffers(1, &frameBuffer);
			glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
			assertOpenGLError("glBindFramebuffer");
			glGenTextures(1, &externalTexture);
			glGenTextures(1, &frameBufferTexture);
			glBindTexture(GL_TEXTURE_2D, frameBufferTexture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, decodedNvFrame->width, decodedNvFrame->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
			assertOpenGLError("198");
			glUniform1i(texLocation, 0);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			assertOpenGLError("204");
			
			firstRun = false;
		}
		assertOpenGLError("210");

		//TODO: put these back
		//glBindVertexArray(vertexArrayObject);
		program->use();
		//glActiveTexture(GL_TEXTURE0);
		//assertOpenGLError("216");
		glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
		glBindTexture(GL_TEXTURE_2D, frameBufferTexture);
	
		assertOpenGLError("220");
		//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, externalTexture, 0);
		EGLImageKHR hEglImage;

		//EGLSyncKHR eglSync;
		int iErr;
		hEglImage = NvEGLImageFromFd(eglDisplay, decodedNvFrame->nvBuffer->planes[0].fd);
		if (!hEglImage)
			printf("Could not get EglImage from fd. Not rendering\n");
																				assertOpenGLError("224");
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, frameBufferTexture, 0);
																				assertOpenGLError("228");
		
		//glBindTexture(GL_TEXTURE_2D, externalTexture);
		//glBindTexture(GL_TEXTURE_2D, externalTexture);
		glBindTexture(GL_TEXTURE_2D, externalTexture);
																				assertOpenGLError("232");
		glEGLImageTargetTexture2DOES(GL_TEXTURE_2D, hEglImage);
																				assertOpenGLError("235");
		//glBindTexture(GL_TEXTURE_2D, frameBufferTexture);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
																				assertOpenGLError("237");
    	//glClear(GL_COLOR_BUFFER_BIT);
																				assertOpenGLError("239");
		glUniform1i(texLocation, 0);
		glBindVertexArray(vertexArrayObject);
																				assertOpenGLError("241");
		//glActiveTexture(GL_TEXTURE0+1);
		//glBindTexture(GL_TEXTURE_2D, frameBufferTexture);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
																				assertOpenGLError("246");

																				printf("244\n");
		//glBindTexture(GL_TEXTURE_2D, frameBufferTexture);
		//-------------
		glReadBuffer(GL_COLOR_ATTACHMENT0);
																				assertOpenGLError("264");
																				printf("249\n");
		GLenum frameBufferStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    	if (frameBufferStatus!=GL_FRAMEBUFFER_COMPLETE) {
			printf("frameBufferStatus problem!\n");
			abort();
		}
		glReadPixels(0, 0, 512, 512, GL_RGBA, GL_UNSIGNED_BYTE, r);
																				printf("246\n");
																				assertOpenGLError("268");

		for (int i = 0; i < 100; ++i)
		{
			printf("%i ", r[i]);
		}
		printf("\n");

		NvDestroyEGLImage(eglDisplay, hEglImage);
		abort();
		//-------------
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);
		/*
		iErr = glGetError();
		if (iErr != GL_NO_ERROR)
			printf("glDrawArrays arrays failed: %i\n", iErr);
		
		eglSync = eglCreateSyncKHR(eglDisplay, EGL_SYNC_FENCE_KHR, NULL);
		if (eglSync == EGL_NO_SYNC_KHR)
			printf("eglCreateSyncKHR() failed\n");
		
		eglSwapBuffers(eglDisplay, eglSurface);
		if (eglGetError() != EGL_SUCCESS)
			printf("Got Error in eglSwapBuffers %i\n", eglGetError());

		if (eglClientWaitSyncKHR (eglDisplay, eglSync,
					EGL_SYNC_FLUSH_COMMANDS_BIT_KHR, EGL_FOREVER_KHR) == EGL_FALSE)
		{
			printf("eglClientWaitSyncKHR failed!\n");
		}
		*/
		//eglSwapBuffers(eglDisplay, eglSurface);
		/*
		glReadBuffer(GL_COLOR_ATTACHMENT0);
		assertOpenGLError("264");

		glReadPixels(0, 0, 512, 512, GL_RED, GL_UNSIGNED_BYTE, r);
		assertOpenGLError("268");

		for (int i = 0; i < 50; ++i)
		{
			printf("%i ", r[i]);
		}
		printf("\n");
		iErr = glGetError();
		if (iErr != GL_NO_ERROR)
			printf("glDrawArrays arrays failed:%i\n", iErr);
		*/
		//abort();
	}
}