#include <gtkmm/window.h>
#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/main.h>

#include "MediaStream.h"
#include "FfmpegHardwareDecoder.h"
#include "FfmpegSoftwareDecoder.h"
#include "Singleton.h"
#include "VideoReceiver.h"

#include <iostream>
#include <GL/glew.h>
#include <GL/glx.h>

#include "OpenglArea.h"

#define VERTEX_POINTER 0
#define FRAGMENT_POINTER 1

static const GLfloat vertices[] = {
    -1.0f,-1.0f,
     1.0f,-1.0f,
    -1.0f, 1.0f,
     1.0f, 1.0f
};

static const GLfloat textureCoordinates[] = {
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
	queue_draw();
	
	//glDraw();
}

void OpenGLArea::glInit()
{
	int frameWidth = 640;
	int frameHeight = 360;
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	Shader vertex_shader(ShaderType::Vertex, "vertex.shader");
	Shader fragment_shader(ShaderType::Fragment, "fragment.shader");

	program = new Program();
	program->attach_shader(vertex_shader);
	program->attach_shader(fragment_shader);

	program->link();
	unis[0] = glGetUniformLocation(program->get_id(), "tex_y");
	unis[1] = glGetUniformLocation(program->get_id(), "tex_u");
	unis[2] = glGetUniformLocation(program->get_id(), "tex_v");

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
}

class GLWindow : public Gtk::Window
{
  public:
	GLWindow()
	{
		vbox = new Gtk::VBox;
		drawing_area = new OpenGLArea();
		/*
			GtkWidget* Gtk::Widget::gobj() 	
			Provides access to the underlying C GObject.
		 */
		GtkDrawingArea* gDrawingArea = drawing_area->gobj();
		GtkWidget* gWidget = GTK_WIDGET(gDrawingArea);
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
	//singletonObject.mediaStream = std::make_shared<MediaStream>("rtsp://admin:19929394@192.168.0.103:10554/tcp/av0_1");
	FfmpegDecoder *ffmpegDecoder = new FfmpegDecoder(FfmpegDecoder::H264,FfmpegDecoder::HARDWARE);
	//std::cout << ffmpegDecoder->getSupportedDevices() << std::endl;
	for (auto i: FfmpegHardwareDecoder::getSupportedDevices())
  		std::cout << i << std::endl;
	std::cout << "finished" << std::endl;
	//singletonObject.mediaStream->setDecoder(ffmpegDecoder);
	//singletonObject.mediaThread = std::make_shared<boost::thread>(&MediaStream::run, singletonObject.mediaStream);
	//Singleton::instance()->addStream("cam1", singletonObject);

	GLWindow window;
	window.show_all();

	kit.run(window);
}
