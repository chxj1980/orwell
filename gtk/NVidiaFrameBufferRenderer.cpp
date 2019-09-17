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

void assertOpenGLError(const std::string &msg)
{
	GLenum error = glGetError();

	if (error != GL_NO_ERROR)
	{
		std::stringstream s;
		s << "OpenGL error 0x" << std::hex << error << " at " << msg;
		throw std::runtime_error(s.str());
	}
}

void assertEGLError(const std::string &msg)
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
		//std::unique_lock<std::mutex> lock{mutex};
		//TODO: certify that the operation below is MOVING the frame to here, not copying it
		auto decodedNvFrame = std::dynamic_pointer_cast<DecodedNvFrame>(onAcquireNewDecodedFrame());
		this->decodedNvFrame = decodedNvFrame;
		/* 
		    Since the frame is gone from the fifo, it only exists here. We move it to the renderer and then we 
		    don't need to worry with its lifetime. When another frame arrives, it automatically deletes this one
		    TODO: verify if this indeed happens
		*/
		glDraw();
		glFinish();
		if (!firstFrameReceived)
			firstFrameReceived = true;
	}
}

void NVidiaFrameBufferRenderer::glDraw2(){
	GLuint frameBuffer;
	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	assertOpenGLError("glBindFramebuffer");

	
	/*
	 * Create a texture as color attachment.
	 */
	GLuint t;
	glGenTextures(1, &t);

	glBindTexture(GL_TEXTURE_2D, t);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 500, 500, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	assertOpenGLError("glTexImage2D");
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
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
static unsigned char *r = new unsigned char[512 * 512];

void NVidiaFrameBufferRenderer::glDraw()
{
	/*
		By putting everythng inside a bit if (firstFrameReceived),
		we only create the resources after we know the size and pixel format
		of the received frame. 
	*/
	LOG << "glDraw";

	//TODO: discover why, in the beggining, frame has non setted components (0 for integer, for example)
	if (decodedNvFrame && this->firstFrameReceived && decodedNvFrame->getWidth() != 0)
	{
		LOG << "before first run";

		if (this->firstRun)
		{
			for (int i = 0; i < 512 * 512; ++i)
			{
				d[i] = 255;
			}
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

			glActiveTexture(GL_TEXTURE0);
			texLocation = glGetUniformLocation(program->get_id(), "tex");
			glGetErr("294");

			//VERY IMPORTANT, use the progrm before glUniform1i
			program->use();
			glGetErr("300");

			glGenFramebuffers(1, &frameBuffer);
			glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
			assertOpenGLError("glBindFramebuffer");

			glGenTextures(1, &texture_id);
			glGetErr("316");
			glBindTexture(GL_TEXTURE_2D, texture_id);
			glGetErr("318");
			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 512, 512, 0, GL_RED, GL_UNSIGNED_BYTE, nullptr);
			//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 500, 500, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
			firstRun = false;
		}
		printf("229\n");
		glUniform1i(texLocation, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_id, 0);
		assertOpenGLError("glFramebufferTexture2D");
		printf("237\n");
		//EGLImageKHR hEglImage;

		//EGLSyncKHR eglSync;
		int iErr;
		//hEglImage = NvEGLImageFromFd(NULL, decodedNvFrame->nvBuffer->planes[0].fd);
		//if (!hEglImage)
		//	printf("Could not get EglImage from fd. Not rendering\n");

		//glBindTexture(GL_TEXTURE_EXTERNAL_OES, texture_id);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture_id);
		glBindVertexArray(vertexArrayObject);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		printf("251\n");

		glReadBuffer(GL_COLOR_ATTACHMENT0);
		glReadPixels(0, 0, 512, 512, GL_BGR, GL_UNSIGNED_BYTE, r);
		for (int i = 0; i < 512 * 512; ++i)
		{
			printf("%i ", r[i]);
		}
		printf("\n");
		iErr = glGetError();
		if (iErr != GL_NO_ERROR)
			printf("glDrawArrays arrays failed:%i\n", iErr);
		abort();
	}
}