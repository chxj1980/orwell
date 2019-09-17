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
#include "ZLRTSPClient.h"

SLOG_CATEGORY("main");

int main(int argc, char **argv)
{
	LOG.logImmediately(true);
	LOG << "------------ Orwell initiated!";
	//Gtk::Main kit;
	SLOG_ENABLE_CATEGORIES("main", "NVDecoder", "Decoder", 
						  "NVidiaRenderer", "NvidiaRendererEGL", "myRtspClient", 
						  "ZLRTSPClient", "OpenglSmartRenderer3", "Profiler");
	auto app = Gtk::Application::create(argc, argv, "");
	LOG << "Supported ffmpeg hardware decoders: ";
	for (auto i : FfmpegHardwareDecoder::getSupportedDevices())
		LOG << i;
	//auto ffmpegHardwareDecoder = std::make_shared<FfmpegHardwareDecoder>(Decoder::H264, FfmpegHardwareDecoder::HARDWARE, std::string("cuda"));
    std::string username;
	std::string password;
	std::string rtspUrl;
	username = "admin";
	password = "19929394";
	rtspUrl = "rtsp://admin:19929394@192.168.0.101:10554/tcp/av0_0";
	//username = "admin";
	//password = "ljspqk1.618.@";
	//rtspUrl = "rtsp://192.168.0.118:554/cam/realmonitor?channel=1&subtype=0&unicast=true&proto=Onvif";
	//auto rtspClient = std::make_shared<MyRTSPClient>(rtspUrl, RTSPClient::RTP_OVER_TCP, username, password);
	auto rtspClient = std::make_shared<ZLRTSPClient>(rtspUrl, RTSPClient::RTP_OVER_TCP);
	auto renderer = std::make_shared<OpenglSmartRenderer3>();
	auto decoder = std::make_shared<FfmpegSoftwareDecoder>(Decoder::H264);
	auto orwell = std::make_shared<Orwell>(rtspClient, decoder, renderer);
	Singleton::instance()->addStream("cam1", orwell);
	ProfilingThread profilingThread;
	//TODO (VERY IMPORTANT): when Windows is created, it searches for "cam1" in Singleton.
	//It must be already setted. TODO: I must find a way to not cause problems if it's not setted yet.

	
	//auto openglSmartRenderer3Thread = std::make_shared<std::thread>(&OpenglSmartRenderer3::run, &openglSmartRenderer3);
	//return app->run();
	return app->run(*renderer.get());
}
