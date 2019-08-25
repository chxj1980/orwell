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
#include "Orwell.h"

int main(int argc, char **argv)
{
	//Gtk::Main kit;
	auto app = Gtk::Application::create(argc, argv, "");
	std::cout << "supported hardware: " << std::endl;
	for (auto i : FfmpegHardwareDecoder::getSupportedDevices())
		std::cout << i << std::endl;
	Orwell orwell(RTSPUrl("rtsp://admin:19929394@192.168.0.103:10554/tcp/av0_1"));
	Singleton::instance()->addStream("cam1", orwell);

	//TODO (VERY IMPORTANT): when Windows is created, it searches for "cam1" in Singleton.
	//It must be already setted. I must find a way to not cause problems if it's not setted yet.

	OpenglSmartRenderer3 openglSmartRenderer3;
	openglSmartRenderer3.setDecodedFramesFifo(orwell.decodedFramesFifo);
	auto openglSmartRenderer3Thread = std::make_shared<std::thread>(&OpenglSmartRenderer3::run, &openglSmartRenderer3);
	//return app->run();
	return app->run(openglSmartRenderer3);
}
