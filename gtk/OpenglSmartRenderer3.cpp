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
int OpenglSmartRenderer3::receiveVideo(Frame *frame)
{
	this->frame = frame;
	firstFrameReceived = true;
	queue_draw();
}

void OpenglSmartRenderer3::glInit()
{
}

void err(std::string a) {
	std::cout << a << std::endl;
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
		
		glFlush();
	}
	catch (const Gdk::GLError &gle)
	{
		std::cerr << "An error occurred in the render callback of the GLArea" << std::endl;
		std::cerr << gle.domain() << "-" << gle.code() << "-" << gle.what() << std::endl;
		return false;
	}
}

void OpenglSmartRenderer3::glDraw()
{
	std::cout << "glDraw()" << std::endl;
	/*
		By putting everythng inside a bit if (firstFrameReceived),
		we only create the resources after we know the size and pixel format
		of the received frame. 
	*/
	int textureFormat;
	int alpha;
	PixelFormat *pixelFormat;
	//TODO: discover why, in the beggining, frame has non setted components (0 for integer, for example)
	if (this->firstFrameReceived && frame->width != 0)
	{
		std::cout << "Received frame with width: " << frame->width << " and height: " << frame->height << std::endl;
		std::cout << "getting pixelformat for " << (int)frame->format << std::endl;
		pixelFormat = PixelFormats::get((int)frame->format);
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

			//Shader vertex_shader(ShaderType::Vertex, "video.vert");
			//Shader fragment_shader(ShaderType::Fragment, "planar.frag");

			program = std::make_unique<Program>();
			program->attach_shader(Shader(ShaderType::Vertex, "video.vert"));
			err("86");
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
			err("97");

			program->link();
			err("100");

			vextexInLocation = glGetAttribLocation(program->get_id(), "aPos");
			textureInLocation = glGetAttribLocation(program->get_id(), "aTexCoord");
			err("104");

			glGenVertexArrays(1, &VAO);
			glGenBuffers(1, &VBO);
			//glGenBuffers(1, &TBO);

			//glGenBuffers(1, &EBO);
			// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
			glBindVertexArray(VAO);

			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_textures), vertices_textures, GL_STATIC_DRAW);

			glVertexAttribPointer(vextexInLocation, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void *)0);
			glEnableVertexAttribArray(vextexInLocation);

			glVertexAttribPointer(textureInLocation, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void *)(3 * sizeof(GLfloat)));
			glEnableVertexAttribArray(textureInLocation);
			err("122");

			//glBindBuffer(GL_TEXTURE_BUFFER, TBO);
			//glBufferData(GL_TEXTURE_BUFFER, sizeof(textureCoordinates), textureCoordinates, GL_STATIC_DRAW);//TODO: static or stream?
			//glVertexAttribPointer(textureInLocation, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
			//glEnableVertexAttribArray(textureInLocation);

			// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
			//glBindBuffer(GL_ARRAY_BUFFER, 0);
			//glBindBuffer(GL_TEXTURE_BUFFER, 0);

			// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
			//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

			// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
			// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
			//glBindVertexArray(0);

			//vextexInLocation = program->attributeLocation("vertexIn");
			//textureInLocaltion = program->attributeLocation("textureIn");

			textureLocation[0] = glGetUniformLocation(program->get_id(), "tex_y");
			textureLocation[1] = glGetUniformLocation(program->get_id(), "tex_u");
			textureLocation[2] = glGetUniformLocation(program->get_id(), "tex_v");
			err("146");

			//alpha = program->uniformLocation("alpha");
			alpha = glGetUniformLocation(program->get_id(), "alpha");
			//program->setUniformValue(mAlpha, (GLfloat)1.0);
			err("151");
			//glUniform1f(alpha, (GLfloat)1.0);
			err("152");
			//textureFormat = program->uniformLocation("tex_format");
			textureFormat = glGetUniformLocation(program->get_id(), "tex_format");
			err("154");

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
				err("168");

				for (int i = 0; i < TEXTURE_NUMBER; i++)
				{
					//std::cout << "initiating texture " << i << std::endl;
					glBindTexture(GL_TEXTURE_2D, textureId[i]);
					std::cout << i << std::endl;
					err("176");

					/*
						Our Frame called `frame` has a PixelFormat (example: AV_PIX_FMT_YUV420P). 
						We're gonna get, in the list of PixelFormats, for parameters for this format.
						The parameters are things like the ratio of U and V components of the YUV 
						component, in the case of an YUV frame, or the details about RGB in the
						case of an RGB frame.
					*/
					Fraction widthRatio = pixelFormat->yuvWidths[i];
					Fraction heightRatio = pixelFormat->yuvHeights[i];
					/*
						The width and height of our texture is determined by our widthRation and heightRatio.
						TODO: explain it more.
					 */
					//TODO: why he used frame->linesize[i] instead of frame->width? frame->width worked perfectly for me
					int width = frame->width * widthRatio.numerator / heightRatio.denominator;
					int height = frame->height * heightRatio.numerator / heightRatio.denominator;
					//std::cout << "yuv plane number: " << i << " has width: " << width << " and height: " << height << std::endl;

					glTexImage2D(GL_TEXTURE_2D,
								 0,
								 pixelFormat->yuvInternalFormat[i],
								 width,
								 height,
								 0,
								 pixelFormat->yuvGlFormat[i],
								 pixelFormat->dataType,
								 NULL); //frame->buffer[i]);
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
			err("213");

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

		//https://community.khronos.org/t/i-passed-a-pointer-to-glvertexattribpointer-without-binded-buffer-and-it-works/104367
		//TODO: to be changed to buffer object on CORE version, because only yhis version works (read link above)

		//glVertexAttribPointer(VERTEX_POINTER, 2, GL_FLOAT, 0, 0, vertices);
		//glEnableVertexAttribArray(VERTEX_POINTER);

		//glVertexAttribPointer(FRAGMENT_POINTER, 2, GL_FLOAT, 0, 0, textureCoordinates);
		//glEnableVertexAttribArray(FRAGMENT_POINTER);

		for (int j = 0; j < TEXTURE_NUMBER; j++)
		{
			glActiveTexture(GL_TEXTURE0 + j);
			glBindTexture(GL_TEXTURE_2D, textureId[j]);
			//glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32F, TBO);

			int linesize = frame->linesize[j];
			uint8_t *buffer = frame->buffer[j];

			if (buffer != NULL && linesize != 0)
			{
				int textureSize = linesize * frame->height;

				//if (m_pbo[pboIndex][j].size() != textureSize)
				//	m_pbo[pboIndex][j].allocate(textureSize);

				//TODO: why he used frame->linesize[i] instead of frame->width? frame->width worked perfectly for me
				int width = frame->width * pixelFormat->yuvWidths[j].numerator / pixelFormat->yuvWidths[j].denominator;
				int height = frame->height * pixelFormat->yuvHeights[j].numerator / pixelFormat->yuvHeights[j].denominator;
				//std::cout << "width: " << width << " height: " << height << std::endl;
				//Copy frame here!
				glTexSubImage2D(GL_TEXTURE_2D,
								0,
								0,
								0,
								width,
								height,
								pixelFormat->yuvGlFormat[j],
								pixelFormat->dataType,
								frame->buffer[j]); //NULL);
			}
			glUniform1i(textureLocation[j], j);
			//m_pbo[pboIndex][j].release();
		}
		glUniform1f(textureFormat, (GLfloat)pixelFormat->textureFormat);

		glBindVertexArray(VAO);
		//glBindBuffer(GL_TEXTURE_BUFFER, TBO);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		firstFrameReceived = false;
	}
}