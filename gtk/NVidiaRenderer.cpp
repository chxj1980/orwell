#include "NVidiaRenderer.h"
#include "SLog.h"

SLOG_DECLARE_CATEGORY("NVidiaRenderer")

const std::string vertexShaderSource =
#include "nvidia.vert"
	;
const std::string fragmentShaderSource =
#include "nvidia.frag"
	;

void NVidiaRenderer::init()
{
}

void NVidiaRenderer::run()
{
	/*
		ThreadSafeDeque decodedFramesFifo is a nice object because it will only pop_front when it has data.
		This way, if decodedFramesFifo is empty, this rendering loop will wait, which is good, no CPU time is wasted.
	*/
	std::cout << "NVidiaRenderer run called" << std::endl;
	if (!decodedFramesFifo)
	{
		std::cerr << "No decodedFramesFifo setted for this renderer" << std::endl;
		return;
	}
	int i = 0;
	while (true)
	{
		//std::unique_lock<std::mutex> lock{mutex};
		//TODO: certify that the operation below is MOVING the frame to here, not copying it
		DecodedFrame frame = std::move(decodedFramesFifo->pop_front());
		/* 
		    Since the frame is gone from the fifo, it only exists here. We move it to the renderer and then we 
		    don't need to worry with its lifetime. When another frame arrives, it automatically deletes this one
		    TODO: verify if this indeed happens
		*/
		std::unique_lock<std::mutex> lk{mutex};
		this->frame = std::move(frame);
		
		//lk.unlock();
		if (!firstFrameReceived)
			firstFrameReceived = true;
		i++;
		std::cout << i << std::endl;
		queue_draw();
		//std::cout << "waiting" << std::endl;
		//conditiconditionVariable.wait(lock);
		//std::cout << "waited" << std::endl;
	}
}

void NVidiaRenderer::glInit()
{
}

void err(std::string lineInformation)
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
	if (this->firstFrameReceived && frame.width() != 0)
	{
		if (this->firstRun)
		{
			std::cout << "firstRun of NVidiaRenderer" << std::endl;

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
			pos_location = glGetAttribLocation(program, "in_pos");

			glEnableVertexAttribArray(pos_location);
			glVertexAttribPointer(pos_location, 2, GL_FLOAT, GL_FALSE, 0, kVertices);

			tc_location = glGetAttribLocation(program, "in_tc");

			glEnableVertexAttribArray(tc_location);
			glVertexAttribPointer(tc_location, 2, GL_FLOAT, GL_FALSE, 0,
								  kTextureCoords);

			glActiveTexture(GL_TEXTURE0);
			glUniform1i(glGetUniformLocation(program, "texSampler"), 0);

			//To be done
			if (!initiatedFrameBufferObjects)
			{
				initiatedFrameBufferObjects = true;
			}

			firstRun = false;
		}

		program->use();

		EGLImageKHR hEglImage;
		bool frame_is_late = false;

		EGLSyncKHR egl_sync;
		int iErr;
		hEglImage = NvEGLImageFromFd(NULL, render_fd);
		if (!hEglImage)
		{
			LOG(SLog::ERROR) << "Could not get EglImage from fd. Not rendering";
			//return -1;
		}

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_EXTERNAL_OES, texture_id);
		glEGLImageTargetTexture2DOES(GL_TEXTURE_EXTERNAL_OES, hEglImage);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		iErr = glGetError();
		if (iErr != GL_NO_ERROR)
		{
			COMP_ERROR_MSG("glDrawArrays arrays failed:" << iErr);
			return -1;
		}
		egl_sync = eglCreateSyncKHR(egl_display, EGL_SYNC_FENCE_KHR, NULL);
		if (egl_sync == EGL_NO_SYNC_KHR)
		{
			COMP_ERROR_MSG("eglCreateSyncKHR() failed");
			return -1;
		}
		/*
		if (last_render_time.tv_sec != 0)
		{
			pthread_mutex_lock(&render_lock);
			last_render_time.tv_sec += render_time_sec;
			last_render_time.tv_nsec += render_time_nsec;
			last_render_time.tv_sec += last_render_time.tv_nsec / 1000000000UL;
			last_render_time.tv_nsec %= 1000000000UL;

			if (isProfilingEnabled())
			{
				struct timeval cur_time;
				gettimeofday(&cur_time, NULL);
				if ((cur_time.tv_sec * 1000000.0 + cur_time.tv_usec) >
					(last_render_time.tv_sec * 1000000.0 +
					 last_render_time.tv_nsec / 1000.0))
				{
					frame_is_late = true;
				}
			}

			pthread_cond_timedwait(&render_cond, &render_lock,
								   &last_render_time);

			pthread_mutex_unlock(&render_lock);
		}
		else
		{
			struct timeval now;

			gettimeofday(&now, NULL);
			last_render_time.tv_sec = now.tv_sec;
			last_render_time.tv_nsec = now.tv_usec * 1000L;
		}
		*/
		eglSwapBuffers(egl_display, egl_surface);
		if (eglGetError() != EGL_SUCCESS)
		{
			COMP_ERROR_MSG("Got Error in eglSwapBuffers " << eglGetError());
			return -1;
		}
		if (eglClientWaitSyncKHR(egl_display, egl_sync,
								 EGL_SYNC_FLUSH_COMMANDS_BIT_KHR, EGL_FOREVER_KHR) == EGL_FALSE)
		{
			COMP_ERROR_MSG("eglClientWaitSyncKHR failed!");
		}

		if (eglDestroySyncKHR(egl_display, egl_sync) != EGL_TRUE)
		{
			COMP_ERROR_MSG("eglDestroySyncKHR failed!");
		}
		NvDestroyEGLImage(egl_display, hEglImage);

		//profiler.finishProcessing(0, frame_is_late);
	}
}