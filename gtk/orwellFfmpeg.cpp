//#include <gtkmm/window.h>
//#include <gtkmm/box.h>
//#include <gtkmm/button.h>
//#include <gtkmm/main.h>
//These two includes must be the very first thing on the program
#include <gtkmm.h>
#include <epoxy/gl.h>

#include "MyRTSPClient.h"
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
#include "SLog.h"
SLOG_CATEGORY("main");

int main(int argc, char **argv)
{
	LOG.printImmediately(true);
	//Gtk::Main kit;
	SLOG_ENABLE_CATEGORIES("main", "NVDecoder", "Decoder", "NVidiaRenderer", "NvidiaRendererEGL", "myRtspClient");
	auto app = Gtk::Application::create(argc, argv, "");
	std::cout << "supported hardware: " << std::endl;
	for (auto i : FfmpegHardwareDecoder::getSupportedDevices())
		std::cout << i << std::endl;
	//auto ffmpegHardwareDecoder = std::make_shared<FfmpegHardwareDecoder>(Decoder::H264, FfmpegHardwareDecoder::HARDWARE, std::string("cuda"));
    std::string rtspUrl("rtsp://admin:19929394@192.168.0.103:10554/tcp/av0_1");
	LOG << "41";
	auto rtspClient = std::make_shared<MyRTSPClient>(rtspUrl);
	LOG << "43";
	auto decoder = std::make_shared<FfmpegSoftwareDecoder>(Decoder::H264);
	LOG << "45";
	Orwell orwell(rtspClient, decoder);
	LOG << "47";
	Singleton::instance()->addStream("cam1", orwell);

	//TODO (VERY IMPORTANT): when Windows is created, it searches for "cam1" in Singleton.
	//It must be already setted. TODO: I must find a way to not cause problems if it's not setted yet.

	OpenglSmartRenderer3 openglSmartRenderer3;
	openglSmartRenderer3.setDecodedFramesFifo(orwell.decodedFramesFifo);
	auto openglSmartRenderer3Thread = std::make_shared<std::thread>(&OpenglSmartRenderer3::run, &openglSmartRenderer3);
	//return app->run();
	return app->run(openglSmartRenderer3);
}
