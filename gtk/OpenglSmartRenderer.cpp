#include "OpenglSmartRenderer.h"
#include "Singleton.h"

void OpenglSmartRenderer::init()
{
	std::cout << "OpenGLArea init" << std::endl;

	set_size_request(640, 360);
	Singleton::instance()->getStream("cam1").mediaStream->decoder->setVideoReceiver(this);
}
/*
~OpenGLArea::OpenGLArea()
{
	delete program;
}
*/
//void OpenGLArea::receiveVideo(unsigned char **videoBuffer, int frameWidth, int frameHeight)
int OpenglSmartRenderer::receiveVideo(Frame *frame)
{
	this->frameWidth = frame->width;
	this->frameHeight = frame->height;
	this->format = frame->format;
	if (format != -1 && this->format != frame->format)
	{
		//ERROR: format changed from one frame to another, this renderer does not support this
		return 1;
	}
	//We don't use switch here because I want to return if problem happens
	if (frame->format == AV_PIX_FMT_YUV420P)
	{
		if (!firstFrameReceived)
		{
			buffer[0] = new unsigned char[frameWidth * frameHeight];	 //Y
			buffer[1] = new unsigned char[frameWidth * frameHeight / 4]; //U
			buffer[2] = new unsigned char[frameWidth * frameHeight / 4]; //V
			firstFrameReceived = true;
		}
		else
		{
			memcpy(buffer[0], frame->buffer[0], frameWidth * frameHeight);
			memcpy(buffer[1], frame->buffer[1], frameWidth * frameHeight / 4);
			memcpy(buffer[2], frame->buffer[2], frameWidth * frameHeight / 4);
		}
	}
	else if (frame->format == AV_PIX_FMT_ABGR)
	{
	}
	queue_draw();
}

void OpenglSmartRenderer::glInit()
{
}

void OpenglSmartRenderer::glDraw()
{
	//Check to see if we received the first frame, so we have frameWidth
	//and frameHeight values setted
	if (this->firstFrameReceived)
	{
		if (this->firstRun)
		{
			glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

			Shader vertex_shader(ShaderType::Vertex, "vertex_yuv.shader");
			Shader fragment_shader(ShaderType::Fragment, "fragment_yuv.shader");

			program = std::make_unique<Program>();
			program->attach_shader(vertex_shader);
			program->attach_shader(fragment_shader);

			program->link();
			unis[0] = glGetUniformLocation(program->get_id(), "tex_y");
			unis[1] = glGetUniformLocation(program->get_id(), "tex_u");
			unis[2] = glGetUniformLocation(program->get_id(), "tex_v");

			glGenTextures(3, texs); //TODO: delete texture

			//Y
			glBindTexture(GL_TEXTURE_2D, texs[0]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, frameWidth, frameHeight, 0, GL_RED, GL_UNSIGNED_BYTE, 0);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			//U
			glBindTexture(GL_TEXTURE_2D, texs[1]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, frameWidth / 2, frameHeight / 2, 0, GL_RED, GL_UNSIGNED_BYTE, 0);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			//V
			glBindTexture(GL_TEXTURE_2D, texs[2]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, frameWidth / 2, frameHeight / 2, 0, GL_RED, GL_UNSIGNED_BYTE, 0);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			firstRun = false;
		}

		//glClear(GL_COLOR_BUFFER_BIT);
		program->use();

		//GLint originTextureUnit;
		//glGetIntegerv(GL_ACTIVE_TEXTURE, &originTextureUnit);

		glVertexAttribPointer(VERTEX_POINTER, 2, GL_FLOAT, 0, 0, vertices);
		glEnableVertexAttribArray(VERTEX_POINTER);

		glVertexAttribPointer(FRAGMENT_POINTER, 2, GL_FLOAT, 0, 0, textureCoordinates);
		glEnableVertexAttribArray(FRAGMENT_POINTER);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texs[0]);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, frameWidth, frameHeight, GL_RED, GL_UNSIGNED_BYTE, buffer[0]);
		glUniform1i(unis[0], 0);

		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_2D, texs[1]);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, frameWidth / 2, frameHeight / 2, GL_RED, GL_UNSIGNED_BYTE, buffer[1]);
		glUniform1i(unis[1], 1);

		glActiveTexture(GL_TEXTURE0 + 2);
		glBindTexture(GL_TEXTURE_2D, texs[2]);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, frameWidth / 2, frameHeight / 2, GL_RED, GL_UNSIGNED_BYTE, buffer[2]);
		glUniform1i(unis[2], 2);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		//program->disableAttributeArray(A_VER);
		//program->disableAttributeArray(T_VER);
		//program->release();

		//glActiveTexture(originTextureUnit);

		//triangle->draw ();
		firstFrameReceived = false;
	}
}