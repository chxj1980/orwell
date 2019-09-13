#include "OpenglSmartRenderer3.h"
#include "SLog.h"
SLOG_CATEGORY("OpenglSmartRenderer3");

const std::string vertexShaderSource =
#include "video.vert"
;
const std::string fragmentShaderSourcePlanar =
#include "planar.frag"
;
const std::string fragmentShaderSourcePacked =
#include "packed.frag"
;

void OpenglSmartRenderer3::init()
{
}

void OpenglSmartRenderer3::run()
{
	/*
		ThreadSafeDeque decodedFramesFifo is a nice object because it will only pop_front when it has data.
		This way, if decodedFramesFifo is empty, this rendering loop will wait, which is good, no CPU time is wasted.
	*/
	std::cout << "OpenglSmartRenderer3 run called" << std::endl;
	if (!decodedFramesFifo)
	{
		std::cerr << "No decodedFramesFifo setted for this renderer" << std::endl;
		return;
	}
	//int i = 0;
	while (true)
	{
		//std::unique_lock<std::mutex> lock{mutex};
		//TODO: certify that the operation below is MOVING the frame to here, not copying it
		auto decodedFfmpegFrame =  std::dynamic_pointer_cast<DecodedFfmpegFrame>(decodedFramesFifo->pop_front());
		/* 
		    Since the frame is gone from the fifo, it only exists here. We move it to the renderer and then we 
		    don't need to worry with its lifetime. When another frame arrives, it automatically deletes this one
		    TODO: verify if this indeed happens
		*/
		std::unique_lock<std::mutex> lk{mutex};
		this->decodedFfmpegFrame = decodedFfmpegFrame;
		lk.unlock();
		if (!firstFrameReceived)
			firstFrameReceived = true;
		//i++;
		queue_draw();
		profile(0, 1000, [this]{fps++;}, [this]{fps=0;});
		//LOG << fps;
		//std::cout << "waiting" << std::endl;
		//conditiconditionVariable.wait(lock);
		//std::cout << "waited" << std::endl;
	}
}

void OpenglSmartRenderer3::glInit()
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

bool OpenglSmartRenderer3::render(const Glib::RefPtr<Gdk::GLContext> &context)
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

void OpenglSmartRenderer3::glDraw()
{
	/*
		By putting everythng inside a bit if (firstFrameReceived),
		we only create the resources after we know the size and pixel format
		of the received decodedFrame-> 
	*/
	int textureFormat;
	int alpha;
	PixelFormat *pixelFormat;
	//TODO: discover why, in the beggining, frame has non setted components (0 for integer, for example)
	if (this->firstFrameReceived && decodedFfmpegFrame->getWidth() != 0)
	{
		//std::cout << "Received frame with width: " << decodedFrame->width << " and height: " << decodedFrame->height << std::endl;
		//std::cout << "getting pixelformat for " << (int)decodedFrame->format << std::endl;
		std::cout << "begin drawing" << std::endl;
		//--------------
		//Width of each plane Y,U,V or R,G,B
		int width[3];
		//Height of each plane
		int height[3];
		//Stride of each plane (explained below)
		int linesize[3];
		//Planesize of each plane (explained below)
		int planeSize[3];
		//Texture size of each OpenGL texture
		int textureSize[3];
		/*
			Our Frame called `frame` has a PixelFormat (example: AV_PIX_FMT_YUV420P). 
			We're gonna get, in the list of PixelFormats, for parameters for this format.
			The parameters are things like the ratio of U and V components of the YUV 
			component, in the case of an YUV frame, or the details about RGB in the
			case of an RGB decodedFrame->
		*/
		pixelFormat = PixelFormats::get(static_cast<int>(decodedFfmpegFrame->getFormat()));
		for (int i = 0; i <= 2; i++)
		{
			/*
				Video width and height are from the first plane
				of the YUV plane, for example.In YUV420P, for example, 
				U and V are smaller.
				Given the video width and height, which comes from the first
				plane for YUV (TODO: write how it comes from RGB), we can calculate,
				based on the pixel format, the width and height of the other planes (U,V) 
				or G,B. We use widthRatio and heightRatio for that purpose.
			*/
			Fraction widthRatio = pixelFormat->yuvWidths[i];
			Fraction heightRatio = pixelFormat->yuvHeights[i];
			/*
				linesize[i] is the stride (https://docs.microsoft.com/en-us/windows/win32/medfound/image-stride)
				for each plane.
				Basically, the stride must be greater or equal to the width of the plane,
				and is there for performance purposes. 
				We need to render to a texture. Each plane has a planeSize[i],
				which is basically linesize[i]*height[i]. We need to take linesize in consideration
				when copying to the texture, which is, of course, of size width[i]*height[i].
				So, basically, we need to copy the plane to the texture ignoring the padding bytes 
				that come after the stride.
			*/
			//TODO: can linesize be <0?
			linesize[i] = decodedFfmpegFrame->getLineSize(i);
			width[i] = decodedFfmpegFrame->getWidth() * widthRatio.numerator / heightRatio.denominator;
			height[i] = decodedFfmpegFrame->getHeight() * heightRatio.numerator / heightRatio.denominator;
			planeSize[i] = linesize[i] * height[i];
			textureSize[i] = width[i] * height[i];
			/*
				Now that we setted width, height, linesize and planarSize, our renderer can
				render the image by doing the correct copy taking the stride into account.
			*/
		}
		if (this->firstRun)
		{
			std::cout << "firstRun of OpenglSmartRenderer3" << std::endl;
			//This is a pointer to an object that is created with this renderer, so it never goes away
			if (!pixelFormat)
			{
				std::cout << "ERROR, format of decoded frame is not supported" << std::endl;
				return;
			}
			//glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

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
			if (pixelFormat->isPlanar)
			{
				Shader fragmentShader(ShaderType::Fragment);
				fragmentShader.load_from_string(fragmentShaderSourcePlanar);
				program->attach_shader(fragmentShader);
				//program->attach_shader(Shader(ShaderType::Fragment, "yuv420p.frag"));
			}
			else
			{
				Shader fragmentShader(ShaderType::Fragment);
				fragmentShader.load_from_string(fragmentShaderSourcePacked);
				program->attach_shader(fragmentShader);
			}
			program->link();

			vextexInLocation = glGetAttribLocation(program->get_id(), "aPos");
			textureInLocation = glGetAttribLocation(program->get_id(), "aTexCoord");

			glGenVertexArrays(1, &vertexArrayObject);
			glGenBuffers(1, &vertexBufferObject);
			glGenBuffers(3, pixelBufferObjects);

			glBindVertexArray(vertexArrayObject);

			glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_textures), vertices_textures, GL_STATIC_DRAW);

			glVertexAttribPointer(vextexInLocation, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void *)0);
			glEnableVertexAttribArray(vextexInLocation);

			glVertexAttribPointer(textureInLocation, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void *)(3 * sizeof(GLfloat)));
			glEnableVertexAttribArray(textureInLocation);

			textureLocation[0] = glGetUniformLocation(program->get_id(), "tex_y");
			textureLocation[1] = glGetUniformLocation(program->get_id(), "tex_u");
			textureLocation[2] = glGetUniformLocation(program->get_id(), "tex_v");

			//alpha = program->uniformLocation("alpha");
			alpha = glGetUniformLocation(program->get_id(), "alpha");
			glUniform1f(alpha, (GLfloat)1.0);
			textureFormat = glGetUniformLocation(program->get_id(), "tex_format");

			//mTextureOffset = program->uniformLocation("tex_offset");
			//mImageWidthId = program->uniformLocation("imageWidth");
			//mImageHeightId = program->uniformLocation("imageHeight");
			//mEnableHDRId = program->uniformLocation("enableHDR");
			//enableGaussianBlurId = program->uniformLocation("enableGaussianBlur");

			//-------
			if (!initiatedTextures)
			{
				std::cout << "initiatedTextures" << std::endl;
				//TODO: delete these textures
				glGenTextures(TEXTURE_NUMBER, textureId);

				for (int i = 0; i < TEXTURE_NUMBER; i++)
				{
					glBindTexture(GL_TEXTURE_2D, textureId[i]);
					//We need to call glBufferData at least once, so then we can use glBufferSubData
					glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pixelBufferObjects[i]);
					glBufferData(GL_PIXEL_UNPACK_BUFFER, textureSize[i], 0, GL_STREAM_DRAW);
					glTexImage2D(GL_TEXTURE_2D,
								 0,
								 pixelFormat->yuvInternalFormat[i],
								 width[i],
								 height[i],
								 0,
								 pixelFormat->yuvGlFormat[i],
								 pixelFormat->dataType,
								 NULL); //decodedFrame->buffer[i]);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
					//glTexParameteri(GL_TEXTURE_2D, GL_UNPACK_SWAP_BYTES, GL_TRUE);
					//glPixelStorei(GL_UNPACK_SWAP_BYTES,GL_TRUE);
				}
				initiatedTextures = true;
			}

			//To be done
			if (!initiatedFrameBufferObjects)
			{
				for (int i = 0; i < 1; i++)
				{
					for (int j = 0; j < TEXTURE_NUMBER; j++)
					{
						//m_pbo[i][j] = QOpenGLBuffer(QOpenGLBuffer::PixelUnpackBuffer);
						//m_pbo[i][j].setUsagePattern(QOpenGLBuffer::StreamDraw);
						//m_pbo[i][j].create();
					}
				}
				initiatedFrameBufferObjects = true;
			}

			firstRun = false;
		}

		program->use();
		//std::cout << "------------------" << std::endl;

		for (int j = 0; j < TEXTURE_NUMBER; j++)
		{
			glActiveTexture(GL_TEXTURE0 + j);
			glBindTexture(GL_TEXTURE_2D, textureId[j]);
			glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pixelBufferObjects[j]);

			//glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, TBO);

			if (decodedFfmpegFrame->getPointer(j) != NULL) // && linesize[j] != 0)
			{
				/*
					We're gonna write to our Pixel Buffer Object line by line ignoring the stride.
					There are height[j] lines for the current plane j, and linesize[j] is the stride
					for plane j. 
				*/
				for (int i = 0; i <= height[j] - 1; i++)
				{
					GLintptr offset = i * linesize[j];
					glBufferSubData(GL_PIXEL_UNPACK_BUFFER, offset, width[j], decodedFfmpegFrame->getPointer(j) + offset);
				}
				glTexSubImage2D(GL_TEXTURE_2D,
								0,
								0,
								0,
								width[j],
								height[j],
								pixelFormat->yuvGlFormat[j],
								pixelFormat->dataType,
								NULL); //decodedFrame->buffer[j]); //NULL);
			}
			glUniform1i(textureLocation[j], j);
		}
		glUniform1f(textureFormat, (GLfloat)pixelFormat->textureFormat);

		glBindVertexArray(vertexArrayObject);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}
}