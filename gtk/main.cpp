#include <gtkmm/window.h>
#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/main.h>

#include "MediaStream.h"
#include "FfmpegDecoder.h"
#include "Singleton.h"
#include "VideoReceiver.h"

#include <iostream>
#include <GL/glew.h>
#include <GL/glx.h>

#include "OpenglArea.h"

#define A_VER 0
#define T_VER 1

static const GLfloat ver[] = {
	-1.0f, 1.0f,
	1.0f, 1.0f,
	-1.0f, -1.0f,
	1.0f, -1.0f,
};


static const GLfloat tex[] = {
	0.0f, 1.0f,
	1.0f, 1.0f,
	0.0f, 0.0f,
	1.0f, 0.0f
};

void OpenGLArea::init()
{
	std::cout << "OpenGLArea init" << std::endl;

	set_size_request(640, 360);
	Singleton::instance()->getStream("cam1").mediaStream->ffmpegDecoder->setVideoReceiver(this);
}
/*
~OpenGLArea::OpenGLArea()
{
	delete program;
}
*/
void OpenGLArea::receiveVideo(unsigned char **videoBuffer, int frameWidth, int frameHeight)
{
	this->frameWidth = frameWidth;
	this->frameHeight = frameHeight;
	//Before first render, datas pointer isn't even created yet
	if (!firstFrameReceived)
	{
		buffer[0] = new unsigned char[frameWidth * frameHeight];	 //Y
		buffer[1] = new unsigned char[frameWidth * frameHeight / 4]; //U
		buffer[2] = new unsigned char[frameWidth * frameHeight / 4]; //V
		firstFrameReceived = true;
	}
	else
	{
		memcpy(buffer[0], videoBuffer[0], frameWidth * frameHeight);
		memcpy(buffer[1], videoBuffer[1], frameWidth * frameHeight / 4);
		memcpy(buffer[2], videoBuffer[2], frameWidth * frameHeight / 4);
	}
	//glDraw();
}

void OpenGLArea::glInit()
{
	std::cout << "on GL init" << std::endl;

	int frameWidth = 640;
	int frameHeight = 360;
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	Shader vertex_shader(ShaderType::Vertex, "vertex.shader");
	Shader fragment_shader(ShaderType::Fragment, "fragment.shader");

	program = new Program();
	program->attach_shader(vertex_shader);
	program->attach_shader(fragment_shader);
	program->link();

	glGenTextures(3, texs);//TODO: delete texture

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

}

void OpenGLArea::glDraw()
{
	std::cout << "on draw" << std::endl;
	//glClear(GL_COLOR_BUFFER_BIT);

	program->use();

	//GLint originTextureUnit;
	//glGetIntegerv(GL_ACTIVE_TEXTURE, &originTextureUnit);

	glVertexAttribPointer(A_VER, 2, GL_FLOAT, 0, 0, ver);
	glEnableVertexAttribArray(A_VER);

	glVertexAttribPointer(T_VER, 2, GL_FLOAT, 0, 0, tex);
	glEnableVertexAttribArray(T_VER);

	unis[0] = glGetAttribLocation(program->get_id(), "tex_y");
	unis[1] = glGetAttribLocation(program->get_id(), "tex_u");
	unis[2] = glGetAttribLocation(program->get_id(), "tex_v");

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
}

class GLWindow : public Gtk::Window
{
  public:
	GLWindow()
	{
		std::cout << "GLWindow constructor" << std::endl;
		vbox = new Gtk::VBox;
		std::cout << "gonna create OpenGlArea" << std::endl;
		drawing_area = new OpenGLArea();

		vbox->pack_start(*drawing_area, true, true);
		add(*vbox);
	}

  private:
	Gtk::Button *button;
	Gtk::VBox *vbox;
	OpenGLArea *drawing_area;
};

int main()
{
	Gtk::Main kit;
	SingletonObject singletonObject;
	singletonObject.mediaStream = std::make_shared<MediaStream>("rtsp://admin:19929394@192.168.0.101:10554/tcp/av0_1");
	FfmpegDecoder *ffmpegDecoder = new FfmpegDecoder();
	singletonObject.mediaStream->setDecoder(ffmpegDecoder);
	singletonObject.mediaThread = std::make_shared<boost::thread>(&MediaStream::run, singletonObject.mediaStream);
	Singleton::instance()->addStream("cam1", singletonObject);

	GLWindow window;
	window.show_all();

	kit.run(window);
}
