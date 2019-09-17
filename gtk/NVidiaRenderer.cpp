#include "NVidiaRenderer.h"
#include "SLog.h"

SLOG_CATEGORY("NVidiaRenderer")

const std::string vertexShaderSource =
#include "nvidia2.vert"
	;
const std::string fragmentShaderSource =
#include "nvidia2.frag"
	;

#define TEST_CONDITION(condition, message, errorCode)      \
	if (condition)                                         \
	{                                                      \
		LOG << message;									   \
	}
static EGLint rgba8888[] = {
        EGL_RED_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_BLUE_SIZE, 8,
        EGL_ALPHA_SIZE, 8,
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_NONE,
    };
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

static const GLfloat ver[] = {
    -1.0f,-1.0f,
     1.0f,-1.0f,
    -1.0f, 1.0f,
     1.0f, 1.0f
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
	/*
	EGLBoolean eglStatus;
	GtkWidget *widget = glArea.Widget::gobj();
	EGLConfig eglConfig;
	EGLint n_config;

	static EGLint rgba8888[] = {
        EGL_RED_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_BLUE_SIZE, 8,
        EGL_ALPHA_SIZE, 8,
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_NONE,
    };
	//TODO: test these attributes
	EGLint context_attribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };

	eglDisplay = eglGetDisplay((EGLNativeDisplayType)gdk_x11_display_get_xdisplay(glArea.get_display()->gobj()));

	eglStatus = eglInitialize(eglDisplay, 0, 0);
	if (!eglStatus)
    {
        printf("Error at eglInitialize\n");
	}
	eglStatus = eglChooseConfig(eglDisplay, rgba8888, &eglConfig, 1, &numConfigs);
	if (!eglStatus)
    {
        printf("Error at eglChooseConfig\n");
    }
	eglBindAPI (EGL_OPENGL_API);
	eglContext = eglCreateContext(eglDisplay, eglConfig, EGL_NO_CONTEXT, context_attribs);
	if (eglGetError() != EGL_SUCCESS)
    {
        printf("Got Error in eglCreateContext \n");
    }
	eglSurface = eglCreateWindowSurface(eglDisplay, eglConfig, (EGLNativeWindowType) gdk_x11_window_get_xid(glArea.get_window()->gobj()), NULL);
	if (eglSurface == EGL_NO_SURFACE)
    {
        printf("Error in creating egl surface \n");
    }
	if (eglGetError() != EGL_SUCCESS)
    {
        printf("Got Error in eglCreateContext \n");
    }
	eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext);

    if (eglGetError() != EGL_SUCCESS)
    {
        printf("Error in eglMakeCurrent \n");
    }

    printf("Got numconfigs as %i\n", numConfigs);
	*/
	/*
    renderer->egl_context =
        eglCreateContext(renderer->egl_display, renderer->eglConfig,
                            EGL_NO_CONTEXT, context_attribs);
	*/
    /*
    renderer->egl_surface =
        eglCreateWindowSurface(renderer->egl_display, renderer->eglConfig,
                (EGLNativeWindowType) renderer->x_window, NULL);
    if (renderer->egl_surface == EGL_NO_SURFACE)
    {
        COMP_ERROR_MSG("Error in creating egl surface " << eglGetError());
        goto error;
    }
	*/
	/*
	#define CONTEXT_ES20

    #ifdef CONTEXT_ES20
        EGLint ai32ContextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2,
                                                     EGL_NONE };
    #endif

    // Step 1 - Get the default display.
    eglDisplay = eglGetDisplay((EGLNativeDisplayType)0);
	printf("162\n");

    // Step 2 - Initialize EGL.
    eglInitialize(eglDisplay, 0, 0);

    #ifdef CONTEXT_ES20
    // Step 3 - Make OpenGL ES the current API.
    eglBindAPI(EGL_OPENGL_ES_API);
	printf("170\n");

    // Step 4 - Specify the required configuration attributes.
    EGLint pi32ConfigAttribs[5];
    pi32ConfigAttribs[0] = EGL_SURFACE_TYPE;
    pi32ConfigAttribs[1] = EGL_WINDOW_BIT;
    pi32ConfigAttribs[2] = EGL_RENDERABLE_TYPE;
    pi32ConfigAttribs[3] = EGL_OPENGL_ES2_BIT;
    pi32ConfigAttribs[4] = EGL_NONE;
    #else
    EGLint pi32ConfigAttribs[3];
    pi32ConfigAttribs[0] = EGL_SURFACE_TYPE;
    pi32ConfigAttribs[1] = EGL_WINDOW_BIT;
    pi32ConfigAttribs[2] = EGL_NONE;
    #endif

    // Step 5 - Find a config that matches all requirements.
    int iConfigs;
    eglChooseConfig(eglDisplay, pi32ConfigAttribs, &eglConfig, 1,
                                                    &iConfigs);
	printf("190\n");

    if (iConfigs != 1) {
        printf("Error: eglChooseConfig(): config not found.\n");
        exit(-1);
    }

    // Step 6 - Create a surface to draw to.
    eglSurface = eglCreateWindowSurface(eglDisplay, eglConfig,
                                  (EGLNativeWindowType)NULL, NULL);
		printf("200\n");

    // Step 7 - Create a context.
    #ifdef CONTEXT_ES20
        eglContext = eglCreateContext(eglDisplay, eglConfig, NULL,
                                               ai32ContextAttribs);
    #else
        eglContext = eglCreateContext(eglDisplay, eglConfig, NULL, NULL);
    #endif
	printf("209\n");

    // Step 8 - Bind the context to the current thread
    eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext);
		printf("213\n");
	*/
}	


void NVidiaRenderer::run()
{
	/*
		ThreadSafeDeque decodedFramesFifo is a nice object because it will only pop_front when it has data.
		This way, if decodedFramesFifo is empty, this rendering loop will wait, which is good, no CPU time is wasted.
	*/
	LOG << "NVidiaRenderer run called";
	
	//TODO: run every loop?
	while (true)
	{
		//std::unique_lock<std::mutex> lock{mutex};
		//TODO: certify that the operation below is MOVING the frame to here, not copying it
		auto decodedNvFrame =  std::dynamic_pointer_cast<DecodedNvFrame>(onAcquireNewDecodedFrame());
		/* 
		    Since the frame is gone from the fifo, it only exists here. We move it to the renderer and then we 
		    don't need to worry with its lifetime. When another frame arrives, it automatically deletes this one
		    TODO: verify if this indeed happens
		*/
		std::unique_lock<std::mutex> lock{mutex};
		this->decodedNvFrame = decodedNvFrame;

		lock.unlock();
		if (!firstFrameReceived)
			firstFrameReceived = true;

		drawerDispatcher.emit();
		
	}
}

const GLfloat vertices_textures[20] = {
		//vertices            //positions
		-1.0f, -1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, -1.0f, 0.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 0.0f, 1.0f, 0.0f};

void NVidiaRenderer::glInit()
{
}



bool NVidiaRenderer::render(const Glib::RefPtr<Gdk::GLContext> &context)
{
	std::unique_lock<std::mutex> lock{mutex};
	glDraw();
	glFinish();
}

static const GLfloat g_vertex_buffer_data[] = {
   -1.0f, -1.0f, 0.0f,
   1.0f, -1.0f, 0.0f,
   0.0f,  1.0f, 0.0f,
};
static float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
};

static void glGetErr(std::string line) {
	int e = glGetError();
	if (e != GL_NO_ERROR)
		std::cout << "got error " << e << " in line " << line << std::endl;
}
//static unsigned char d[230400] = {0};
static unsigned char * d = new unsigned char[640*360*4];
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
			int i;
			for (i = 0; i < 640*360*4; ++i)
			{
				d[i] = 255;
			}
			LOG << "firstRun of NVidiaRenderer";
			
			//glEnable(GL_SCISSOR_TEST);
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
			

			//try tex instead of texSampler????
			//GLuint texLocation = glGetUniformLocation(program->get_id(), "tex");
			//printf("texLocation: %i\n", texLocation);
    		//glUniform1i(texLocation, 0);
			
			//int viewport[4];

			//glGetIntegerv(GL_VIEWPORT, viewport);
			//glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
			//glScissor(viewport[0], viewport[1], viewport[2], viewport[3]);
			glActiveTexture(GL_TEXTURE0);
			texLocation = glGetUniformLocation(program->get_id(), "tex");
			glGetErr("294");
			
			//VERY IMPORTANT, use the progrm before glUniform1i
			program->use();
			glGetErr("300");
			
			/*
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
			*/
	
			glGenTextures(1, &texture_id);
			glGetErr("316");
			glBindTexture(GL_TEXTURE_2D, texture_id);
			glGetErr("318");
		
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 640, 360, 0,  GL_RED, GL_UNSIGNED_BYTE, d);
			
			firstRun = false;
		}

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
		program->use();
		glUniform1i(texLocation, 0);

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

		iErr = glGetError();
		if (iErr != GL_NO_ERROR)
			printf("glDrawArrays arrays failed:%i\n", iErr);

		/*
		
		eglSync = eglCreateSyncKHR(eglDisplay, EGL_SYNC_FENCE_KHR, NULL);
		if (eglSync == EGL_NO_SYNC_KHR)
		{
			printf("eglCreateSyncKHR() failed\n");
		}
		eglSwapBuffers(eglDisplay, eglSurface);
		*/
		//NvDestroyEGLImage(NULL, hEglImage);

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