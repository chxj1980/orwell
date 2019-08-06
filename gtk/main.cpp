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
#include <thread>
#include <mutex>

#include "OpenGLArea.h"
#include "OpenglSmartRenderer.h"

class GLWindow : public Gtk::Window
{
  public:
	GLWindow()
	{
		vbox = new Gtk::VBox;
		drawing_area = new OpenglSmartRenderer();
		/*
			GtkWidget* Gtk::Widget::gobj() 	
			Provides access to the underlying C GObject.
		*/
		GtkDrawingArea *gDrawingArea = drawing_area->gobj();
		GtkWidget *gWidget = GTK_WIDGET(gDrawingArea);
		vbox->pack_start(*drawing_area, true, true);
		add(*vbox);
	}

  private:
	Gtk::Button *button;
	Gtk::VBox *vbox;
	OpenglSmartRenderer *drawing_area;
};

int main()
{
	Gtk::Main kit;
	SingletonObject singletonObject;
	singletonObject.mediaStream = std::make_shared<MediaStream>("rtsp://admin:19929394@192.168.0.103:10554/tcp/av0_1");
	std::cout << "supported hardware: " << std::endl;
	for (auto i : FfmpegHardwareDecoder::getSupportedDevices())
		std::cout << i << std::endl;

	auto ffmpegHardwareDecoder = std::make_shared<FfmpegHardwareDecoder>(Decoder::H264,FfmpegHardwareDecoder::HARDWARE,std::string("cuda"));
	auto ffmpegSoftwareDecoder = std::make_shared<FfmpegSoftwareDecoder>(Decoder::H264);

	singletonObject.mediaStream->setDecoder(ffmpegSoftwareDecoder);
	singletonObject.mediaThread = std::make_shared<std::thread>(&MediaStream::run, singletonObject.mediaStream);
	Singleton::instance()->addStream("cam1", singletonObject);

	GLWindow window;
	window.show_all();

	kit.run(window);
}
