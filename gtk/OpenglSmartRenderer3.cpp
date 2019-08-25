#include "OpenglSmartRenderer3.h"

void OpenglSmartRenderer3::init()
{
}
/*
~OpenGLArea::OpenGLArea()
{
	delete program;
}
*/
//void OpenGLArea::receiveVideo(unsigned char **videoBuffer, int frameWidth, int frameHeight)
int OpenglSmartRenderer3::receiveVideo(DecodedFrame &frame)
{
	this->frame = std::move(frame);
	firstFrameReceived = true;
	queue_draw();
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
	int i = 0;
	while (true)
	{
		std::unique_lock<std::mutex> lock{mutex};
		//TODO: certify that the operation below is MOVING the frame to here, not copying it
		DecodedFrame frame = decodedFramesFifo->pop_front();
		/* 
		    Since the frame is gone from the fifo, it only exists here. We move it to the renderer and then we 
		    don't need to worry with its lifetime. When another frame arrives, it automatically deletes this one
		    TODO: verify if this indeed happens
		*/
		this->frame = std::move(frame);
		if (!firstFrameReceived)
			firstFrameReceived = true;
		i++;
		std::cout << i << std::endl;
		queue_draw();
		std::cout << "waiting" << std::endl;
		conditionVariable.wait(lock);
		std::cout << "waited" << std::endl;
	}
}

void OpenglSmartRenderer3::glInit()
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

bool OpenglSmartRenderer3::render(const Glib::RefPtr<Gdk::GLContext> &context)
{
	try
	{
		glArea.throw_if_error();

		glDraw();

		glFinish();
		std::cout << "gonna notify" << std::endl;
		conditionVariable.notify_one();
		std::cout << "did notify" << std::endl;
	}
	catch (const Gdk::GLError &gle)
	{
		std::cerr << "An error occurred in the render callback of the GLArea" << std::endl;
		std::cerr << gle.domain() << "-" << gle.code() << "-" << gle.what() << std::endl;
		conditionVariable.notify_one();
		return false;
	}
	conditionVariable.notify_one();
}

void OpenglSmartRenderer3::glDraw()
{
	/*
		By putting everythng inside a bit if (firstFrameReceived),
		we only create the resources after we know the size and pixel format
		of the received frame. 
	*/
	int textureFormat;
	int alpha;
	PixelFormat *pixelFormat;
	//TODO: discover why, in the beggining, frame has non setted components (0 for integer, for example)
	if (this->firstFrameReceived && frame.width() != 0)
	{
		std::cout << "Received frame with width: " << frame.width() << " and height: " << frame.height() << std::endl;
		std::cout << "getting pixelformat for " << (int)frame.format() << std::endl;
		pixelFormat = PixelFormats::get((int)frame.format());
		//--------------
		int width[3];
		int height[3];
		int linesize[3];
		int textureSize[3];
		for (int i = 0; i <= 2; i++)
		{
			Fraction widthRatio = pixelFormat->yuvWidths[i];
			Fraction heightRatio = pixelFormat->yuvHeights[i];
			linesize[i] = frame.linesize(i);
			width[i] = linesize[i] * widthRatio.numerator / heightRatio.denominator;
			width[i] = frame.width() * widthRatio.numerator / heightRatio.denominator;
			//width = linesize[i];
			height[i] = frame.height() * heightRatio.numerator / heightRatio.denominator;
			std::cout << "linesize: " << linesize[i] << std::endl;
			textureSize[i] = width[i] * height[i];
		}
		//--------------
		//In the first run we create everything needed
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
			program->attach_shader(Shader(ShaderType::Vertex, "video.vert"));
			/*
				Todo: be careful if renderers will be reused for other video formats. 
				I think it shouldn't, but if it's done, make everything be redone, including
				planar vs packed shader options. Maybe create a reset method?
			*/
			if (pixelFormat->isPlanar)
				program->attach_shader(Shader(ShaderType::Fragment, "planar.frag"));
			//program->attach_shader(Shader(ShaderType::Fragment, "yuv420p.frag"));
			else
				program->attach_shader(Shader(ShaderType::Fragment, "packed.frag"));

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

					/*
						Our Frame called `frame` has a PixelFormat (example: AV_PIX_FMT_YUV420P). 
						We're gonna get, in the list of PixelFormats, for parameters for this format.
						The parameters are things like the ratio of U and V components of the YUV 
						component, in the case of an YUV frame, or the details about RGB in the
						case of an RGB frame.
					*/
					//Fraction widthRatio = pixelFormat->yuvWidths[i];
					//Fraction heightRatio = pixelFormat->yuvHeights[i];
					/*
						The width and height of our texture is determined by our widthRation and heightRatio.
						TODO: explain it more.
					 */
					//TODO: why he used frame.linesize[i] instead of frame.width? frame.width worked perfectly for me
					//int width = frame.width() * widthRatio.numerator / heightRatio.denominator;
					//width = frame.linesize(i);
					//int height = frame.height() * heightRatio.numerator / heightRatio.denominator;
					//std::cout << "yuv plane number: " << i << " has width: " << width << " and height: " << height << std::endl;
					//Fraction widthRatio = pixelFormat->yuvWidths[i];
					//Fraction heightRatio = pixelFormat->yuvHeights[i];
					//int linesize = frame.linesize(i);
					//int width = linesize * widthRatio.numerator / heightRatio.denominator;
					//width = linesize;
					//int height = frame.height() * heightRatio.numerator / heightRatio.denominator;
					glTexImage2D(GL_TEXTURE_2D,
								 0,
								 pixelFormat->yuvInternalFormat[i],
								 width[i],
								 height[i],
								 0,
								 pixelFormat->yuvGlFormat[i],
								 pixelFormat->dataType,
								 NULL); //frame.buffer[i]);
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

			//int linesize = frame.linesize(j);
			uint8_t *buffer = frame.buffer(j);
			int linesize = frame.linesize(j);

			if (buffer != NULL && linesize != 0)
			{
				//Fraction widthRatio = pixelFormat->yuvWidths[j];
				//Fraction heightRatio = pixelFormat->yuvHeights[j];
				//int width = linesize * widthRatio.numerator / heightRatio.denominator;
				//width = linesize;
				//int height = frame.height() * heightRatio.numerator / heightRatio.denominator;
				//std::cout << "linesize: " << linesize << std::endl;
				//int textureSize = width * height;
				//textureSize =  frame.width()* frame.height();
				//std::cout << "frame.width: " << frame.width() << " frame.height: " << frame.height() << std::endl;
				//std::cout << "width: " << width << " height: " << height << std::endl;

				//textureSize = frame.width * frame.height;
				//if (m_pbo[pboIndex][j].size() != textureSize)
				//	m_pbo[pboIndex][j].allocate(textureSize);
				//TODO: readi this http://www.ffmpeg-archive.org/Decoder-AVFrame-data-linesize-confusion-td2965985.html
				//std::cout << "gonna buffer data " << j << std::endl;
				glBufferData(GL_PIXEL_UNPACK_BUFFER, textureSize[j], frame.buffer(j), GL_STREAM_DRAW);
				//std::cout << "buffered data " << std::endl;

				//TODO: why he used frame.linesize[i] instead of frame.width? frame.width worked perfectly for me
				//int width = linesize * pixelFormat->yuvWidths[j].numerator / pixelFormat->yuvWidths[j].denominator;
				//width = linesize;
				//int height = frame.height() * pixelFormat->yuvHeights[j].numerator / pixelFormat->yuvHeights[j].denominator;
				//std::cout << "width: " << width << " height: " << height << std::endl;
				//Copy frame here!
				//std::cout << "gonna glTexSubImage2D " << std::endl;
				glTexSubImage2D(GL_TEXTURE_2D,
								0,
								0,
								0,
								width[j],
								height[j],
								pixelFormat->yuvGlFormat[j],
								pixelFormat->dataType,
								NULL); //frame.buffer(j)); //NULL);
									   //std::cout << "did glTexSubImage2D " << std::endl;
			}
			glUniform1i(textureLocation[j], j);
			//m_pbo[pboIndex][j].release();
		}
		glUniform1f(textureFormat, (GLfloat)pixelFormat->textureFormat);

		glBindVertexArray(vertexArrayObject);
		//glBindBuffer(GL_TEXTURE_BUFFER, TBO);
		//std::cout << "gonna glDrawArrays " << std::endl;

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		std::cout << "did glDrawArrays " << std::endl;
	}
}