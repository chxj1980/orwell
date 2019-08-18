//#include <gtkmm/window.h>
//#include <gtkmm/box.h>
//#include <gtkmm/button.h>
//#include <gtkmm/main.h>

#include "MediaStream.h"
#include "FfmpegHardwareDecoder.h"
#include "FfmpegSoftwareDecoder.h"
#include "Singleton.h"
#include "VideoReceiver.h"

#include <iostream>
//#include <GL/glew.h>
//#include <GL/glx.h>
#include <thread>
#include <mutex>

//#include "OpenGLArea.h"
//#include "OpenglSmartRenderer.h"
//#include "OpenglSmartRenderer2.h"
#include "OpenglSmartRenderer3.h"
#include "SimpleRenderer.h"

/* 
class GLWindow : public Gtk::Window
{
  public:
	GLWindow()
	{
		vbox = new Gtk::VBox;
		drawing_area = new OpenglSmartRenderer2();

		GtkDrawingArea *gDrawingArea = drawing_area->gobj();
		GtkWidget *gWidget = GTK_WIDGET(gDrawingArea);
		vbox->pack_start(*drawing_area, true, true);
		add(*vbox);
	}

  private:
	Gtk::Button *button;
	Gtk::VBox *vbox;
	OpenglSmartRenderer2 *drawing_area;
};
*/
int main(int argc,  char** argv)
{
	//Gtk::Main kit;
	auto app = Gtk::Application::create(argc, argv, "");
    
	SingletonObject singletonObject;
	singletonObject.mediaStream = std::make_shared<MediaStream>("rtsp://admin:19929394@192.168.0.103:10554/tcp/av0_1");
	std::cout << "supported hardware: " << std::endl;
	for (auto i : FfmpegHardwareDecoder::getSupportedDevices())
		std::cout << i << std::endl;

	//singletonObject.uncodedFramesFifo = std::make_shared<ThreadSafeDeque<Frame>>();
	singletonObject.decodedFramesFifo = std::make_shared<ThreadSafeDeque<Frame>>();
	auto ffmpegHardwareDecoder = std::make_shared<FfmpegHardwareDecoder>(Decoder::H264,FfmpegHardwareDecoder::HARDWARE,std::string("cuda"));
	auto ffmpegSoftwareDecoder = std::make_shared<FfmpegSoftwareDecoder>(Decoder::H264);
	singletonObject.decoder = ffmpegSoftwareDecoder;
	singletonObject.mediaStream->setDecoder(singletonObject.decoder);
	//singletonObject.decoder->setUncodedFramesFifo(singletonObject.uncodedFramesFifo);
	singletonObject.decoder->setDecodedFramesFifo(singletonObject.decodedFramesFifo);

	singletonObject.mediaThread = std::make_shared<std::thread>(&MediaStream::run, singletonObject.mediaStream);
	Singleton::instance()->addStream("cam1", singletonObject);

	//TODO (VERY IMPORTANT): when Windows is created, it searches for "cam1" in Singleton. 
	//It must be already setted. I must find a way to not cause problems if it's not setted yet.
	//GLWindow window;
	//window.show_all();
	OpenglSmartRenderer3 openglSmartRenderer3;
	auto openglSmartRenderer3Thread = std::make_shared<std::thread>(&OpenglSmartRenderer3::run, &openglSmartRenderer3);
	//SimpleRenderer simpleRenderer;
	//return app->run();
	return app->run(openglSmartRenderer3);
	//return app->run(simpleRenderer);
	//kit.run(window);
}
