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

int main(int argc, char **argv)
{
	//Gtk::Main kit;
	auto app = Gtk::Application::create(argc, argv, "");

	SingletonObject singletonObject;
	singletonObject.mediaStream = std::make_shared<MediaStream>("rtsp://admin:19929394@192.168.0.103:10554/tcp/av0_1");
	std::cout << "supported hardware: " << std::endl;
	for (auto i : FfmpegHardwareDecoder::getSupportedDevices())
		std::cout << i << std::endl;
	//FIFO encoded and decoder
	singletonObject.encodedFramesFifo = std::make_shared<ThreadSafeDeque<DecodedFrame>>();
	singletonObject.decodedFramesFifo = std::make_shared<ThreadSafeDeque<DecodedFrame>>();
	//Decoders
	auto ffmpegHardwareDecoder = std::make_shared<FfmpegHardwareDecoder>(Decoder::H264, FfmpegHardwareDecoder::HARDWARE, std::string("cuda"));
	auto ffmpegSoftwareDecoder = std::make_shared<FfmpegSoftwareDecoder>(Decoder::H264);
	//Decoder specific configuration
	singletonObject.decoder = ffmpegSoftwareDecoder;
	singletonObject.decoder.setEncodedFramesFifo(singletonObject.encodedFramesFifo);
	singletonObject.decoder->setDecodedFramesFifo(singletonObject.decodedFramesFifo);
	singletonObject.decoderThread = std::make_shared<std::thread>(&Decoder::run, singletonObject.decoder);
	//RTSP client
	singletonObject.mediaStream->setDecoder(singletonObject.decoder);
	singletonObject.mediaThread = std::make_shared<std::thread>(&MediaStream::run, singletonObject.mediaStream);

	Singleton::instance()->addStream("cam1", singletonObject);

	//TODO (VERY IMPORTANT): when Windows is created, it searches for "cam1" in Singleton.
	//It must be already setted. I must find a way to not cause problems if it's not setted yet.

	OpenglSmartRenderer3 openglSmartRenderer3;
	openglSmartRenderer3.setDecodedFramesFifo(singletonObject.decodedFramesFifo);
	auto openglSmartRenderer3Thread = std::make_shared<std::thread>(&OpenglSmartRenderer3::run, &openglSmartRenderer3);
	//return app->run();
	return app->run(openglSmartRenderer3);
}
