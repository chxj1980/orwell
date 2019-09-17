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
#include "NVidiaRenderer.h"
#include "NvidiaRendererEGL.h"
#include "SimpleRenderer.h"
#include "Orwell.h"
#include "SLog.h"
#include "NVDecoder.h"
#include "ZLRTSPClient.h"
SLOG_CATEGORY("main");

int main(int argc, char **argv)
{
	LOG.logImmediately(true);
	LOG << "------------ Orwell initiated!";
	SLOG_ENABLE_CATEGORIES("main", "NVDecoder", "Decoder", 
						   "NVidiaRenderer", "NvidiaRendererEGL", 
						   "myRtspClient", "ZLRTSPClient", "Profiler", "FileWriter");
	Gtk::Main kit;
	//"NaluUtils"
	auto app = Gtk::Application::create(argc, argv, "");
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
	std::string cameraAlias("cam1");
	std::string fileWritePath("/mnt/external");
	auto rtspClient = std::make_shared<ZLRTSPClient>(rtspUrl, RTSPClient::RTP_OVER_TCP);
	std::shared_ptr<Decoder> decoder = std::make_shared<NVDecoder>(NVDecoder::NALU, Decoder::H264);
	auto renderer = std::make_shared<NvidiaRendererEGL>(640, 360, 100, 30);
	//auto renderer = std::make_shared<NVidiaRenderer>();
	auto orwell = std::make_shared<Orwell>(cameraAlias,rtspClient, decoder, renderer);
	Singleton::instance()->addStream(cameraAlias, orwell);
	bool fileOpenResult = orwell->fileWriter->setPath(fileWritePath);
	if (!fileOpenResult)
		LOG << "error opening file: " << orwell->fileWriter->getCurrentPath();
	else {
		LOG << "writing to file: " << orwell->fileWriter->getCurrentPath();
		orwell->fileWriter->startThreadMode();
	}
	ProfilingThread profilingThread;
	//return app->run(*renderer.get());
	getchar();

	//nVidiaRendererEGL.setDecodedFramesFifo(orwell->decodedFramesFifo);
	//auto nVidiaRendererEGLThread = std::make_shared<std::thread>(&NvidiaRendererEGL::run, &nVidiaRendererEGL);
	//TODO (VERY IMPORTANT): when Windows is created, it searches for "cam1" in Singleton.
	//It must be already setted. I must find a way to not cause problems if it's not setted yet.

	//TODO (VERY IMPORTANT): when Windows is created, it searches for "cam1" in Singleton.
	//It must be already setted. I must find a way to not cause problems if it's not setted yet.

	//NVidiaRenderer nVidiaRenderer;
	//nVidiaRenderer.setDecodedFramesFifo(orwell.decodedFramesFifo);
	//auto nVidiaRendererThread = std::make_shared<std::thread>(&NVidiaRenderer::run, &nVidiaRenderer);
	//return app->run();
	//return app->run(nVidiaRenderer);

	//Gtk::Main kit;
	//"NaluUtils"
	/*
	std::string rtspUrl("rtsp://admin:19929394@192.168.0.103:10554/tcp/av0_1");
	auto rtspClient = std::make_shared<MyRTSPClient>(rtspUrl);
	auto decoder = std::make_shared<NVDecoder>(NVDecoder::NALU, Decoder::H264);
	Orwell orwell(rtspClient, decoder);
	Singleton::instance()->addStream("cam1", orwell);
	

	NvidiaRendererEGL nVidiaRendererEGL(1920, 1080, 0, 0);
	nVidiaRendererEGL.setDecodedFramesFifo(orwell.decodedFramesFifo);
	auto nVidiaRendererEGLThread = std::make_shared<std::thread>(&NvidiaRendererEGL::run, &nVidiaRendererEGL);
	//TODO (VERY IMPORTANT): when Windows is created, it searches for "cam1" in Singleton.
	//It must be already setted. I must find a way to not cause problems if it's not setted yet.
	*/
}
