#include "OpenglSmartRenderer2.h"

void OpenglSmartRenderer2::init()
{
	
}
/*
~OpenGLArea::OpenGLArea()
{
	delete program;
}
*/
//void OpenGLArea::receiveVideo(unsigned char **videoBuffer, int frameWidth, int frameHeight)
int OpenglSmartRenderer2::receiveVideo(Frame *frame)
{
	this->frame = frame;
	firstFrameReceived = true;
	queue_draw();
}

void OpenglSmartRenderer2::glInit()
{
}

void OpenglSmartRenderer2::glDraw()
{
	std::cout << "glDraw()" << std::endl;
	/*
		By putting everythng inside a bit if (firstFrameReceived),
		we only create the resources after we know the size and pixel format
		of the received frame. 
	*/
	int textureFormat;
	int alpha;
	PixelFormat* pixelFormat;
	//TODO: discover why, in the beggining, frame has non setted components (0 for integer, for example)
	if (this->firstFrameReceived && frame->width!=0)
	{
		//std::cout << "getting pixelformat for " << frame->format << std::endl;
		pixelFormat = PixelFormats::get((int) frame->format);
		//In the first run we create everything needed
		if (this->firstRun)
		{
			std::cout << "firstRun of OpenglSmartRenderer2" << std::endl;
			//This is a pointer to an object that is created with this renderer, so it never goes away
			if (!pixelFormat) {
				std::cout << "ERROR, format of decoded frame is not supported" << std::endl;
				return;
			}
			glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

			//Shader vertex_shader(ShaderType::Vertex, "video.vert");
			//Shader fragment_shader(ShaderType::Fragment, "planar.frag");

			program = std::make_unique<Program>();
			
			program->attach_shader(Shader(ShaderType::Vertex, "video.vert"));

			/*
				Todo: be careful if renderers will be reused for other video formats. 
				I think it shouldn't, but if it's done, make everything be redone, including
				planar vs packed shader options. Maybe create a reset method?
			*/
			if (pixelFormat->isPlanar)
				program->attach_shader(Shader(ShaderType::Fragment, "planar.frag"));
			else
				program->attach_shader(Shader(ShaderType::Fragment, "packed.frag"));

			program->link();

			//vextexInLocation = program->attributeLocation("vertexIn");
			//textureInLocaltion = program->attributeLocation("textureIn");

			textureLocation[0] = glGetUniformLocation(program->get_id(), "tex_y");
			textureLocation[1] = glGetUniformLocation(program->get_id(), "tex_u");
			textureLocation[2] = glGetUniformLocation(program->get_id(), "tex_v");

			//alpha = program->uniformLocation("alpha");
			alpha =  glGetUniformLocation(program->get_id(), "alpha");
			//program->setUniformValue(mAlpha, (GLfloat)1.0);
			glUniform1f(alpha, (GLfloat)1.0);
			//textureFormat = program->uniformLocation("tex_format");
			textureFormat =  glGetUniformLocation(program->get_id(), "tex_format");

			//mTextureOffset = program->uniformLocation("tex_offset");
			//mImageWidthId = program->uniformLocation("imageWidth");
			//mImageHeightId = program->uniformLocation("imageHeight");
			//mEnableHDRId = program->uniformLocation("enableHDR");
			//enableGaussianBlurId = program->uniformLocation("enableGaussianBlur");

			//glGenTextures(TEXTURE_NUMBER, texs); //TODO: delete texture

			//-------
			if (!initiatedTextures)
			{
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
								 NULL);
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

		//https://community.khronos.org/t/i-passed-a-pointer-to-glvertexattribpointer-without-binded-buffer-and-it-works/104367
		//TODO: to be changed to buffer object on CORE version, because only yhis version works (read link above)
		glVertexAttribPointer(VERTEX_POINTER, 2, GL_FLOAT, 0, 0, vertices);
		glEnableVertexAttribArray(VERTEX_POINTER);

		glVertexAttribPointer(FRAGMENT_POINTER, 2, GL_FLOAT, 0, 0, textureCoordinates);
		glEnableVertexAttribArray(FRAGMENT_POINTER);

		for (int j = 0; j < TEXTURE_NUMBER; j++)
		{
			glActiveTexture(GL_TEXTURE0 + j);
			glBindTexture(GL_TEXTURE_2D, textureId[j]);

			int linesize = frame->linesize[j];
			uint8_t *buffer = frame->buffer[j];

			if (buffer != NULL && linesize != 0)
			{
				int textureSize = linesize * frame->height;
				//std::cout << "gonna read pixelFormat->yuvSizes[" << j << "]" << std::endl;
				//std::cout << "yuvSizes[" << j << "].numerator is " << pixelFormat->yuvSizes[j].numerator << std::endl;
				//std::cout << "yuvSizes[" << j << "].denominator is " << pixelFormat->yuvSizes[j].denominator << std::endl;
				//textureSize = textureSize * pixelFormat->yuvSizes[j].numerator / pixelFormat->yuvSizes[j].denominator;

				//if (m_pbo[pboIndex][j].size() != textureSize)
				//	m_pbo[pboIndex][j].allocate(textureSize);

				//TODO: why he used frame->linesize[i] instead of frame->width? frame->width worked perfectly for me
				int width = frame->width * pixelFormat->yuvWidths[j].numerator / pixelFormat->yuvWidths[j].denominator;
				int height = frame->height * pixelFormat->yuvHeights[j].numerator / pixelFormat->yuvHeights[j].denominator;

				//Copy frame here!
				glTexSubImage2D(GL_TEXTURE_2D, 
								0, 
								0, 
								0, 
								width, 
								height, 
								pixelFormat->yuvGlFormat[j], 
								pixelFormat->dataType, 
								frame->buffer[j]);//NULL);
			}
			//program->setUniformValue(textureLocation[j], j);
			glUniform1i(textureLocation[j],j);
			//m_pbo[pboIndex][j].release();
		}
		//program->setUniformValue(textureFormat, (GLfloat) frame->format);
		//std::cout << "setting textureFormat to " << pixelFormat->textureFormat << std::endl;
		//std::cout << "isPlanar? " << pixelFormat->isPlanar << std::endl;

		glUniform1f(textureFormat, (GLfloat) pixelFormat->textureFormat);

		//GLint originTextureUnit;
		//glGetIntegerv(GL_ACTIVE_TEXTURE, &originTextureUnit);

		//glVertexAttribPointer(VERTEX_POINTER, 2, GL_FLOAT, 0, 0, vertices);
		//glEnableVertexAttribArray(VERTEX_POINTER);

		//glVertexAttribPointer(FRAGMENT_POINTER, 2, GL_FLOAT, 0, 0, textureCoordinates);
		//glEnableVertexAttribArray(FRAGMENT_POINTER);

		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, texs[0]);
		//glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, frameWidth, frameHeight, GL_RED, GL_UNSIGNED_BYTE, buffer[0]);
		//glUniform1i(unis[0], 0);

		//glActiveTexture(GL_TEXTURE0 + 1);
		//glBindTexture(GL_TEXTURE_2D, texs[1]);
		//glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, frameWidth / 2, frameHeight / 2, GL_RED, GL_UNSIGNED_BYTE, buffer[1]);
		//glUniform1i(unis[1], 1);

		//glActiveTexture(GL_TEXTURE0 + 2);
		//glBindTexture(GL_TEXTURE_2D, texs[2]);
		//glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, frameWidth / 2, frameHeight / 2, GL_RED, GL_UNSIGNED_BYTE, buffer[2]);
		//glUniform1i(unis[2], 2);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		//program->disableAttributeArray(A_VER);
		//program->disableAttributeArray(T_VER);
		//program->release();

		//glActiveTexture(originTextureUnit);

		//triangle->draw ();
		firstFrameReceived = false;
	}
}