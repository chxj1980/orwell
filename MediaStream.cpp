//   Copyright 2015-2016 Ansersion
//
//   Licensed under the Apache License, Version 2.0 (the "License");
//   you may not use this file except in compliance with the License.
//   You may obtain a copy of the License at
//
//       http://www.apache.org/licenses/LICENSE-2.0
//
//   Unless required by applicable law or agreed to in writing, software
//   distributed under the License is distributed on an "AS IS" BASIS,
//   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//   See the License for the specific language governing permissions and
//   limitations under the License.
//

#include <iostream>
#include "rtspClient.h"
#include "FfmpegDecoder.h"
#include "VideoConsumer.h"

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include "MediaStream.h"
#include "VideoConsumer.h"

using std::cout;
using std::endl;

bool ByeFromServerFlag2 = false;
void ByeFromServerClbk2()
{
	cout << "Server send BYE" << endl;
	ByeFromServerFlag2 = true;
}
//TODO: if uri is rtsp, do one thing. If it's a .mp4 file, do other thing
MediaStream::MediaStream (std::string uri):rtspClient(uri) {
	//rtspClient     = RtspClient(uri);//why the hell this line gives segmentation fault? TODO: make a minimal working example with it
	//ffmpegDecoder  = FfmpegDecoder();
}

void MediaStream::setFrameUpdater(FrameUpdater* frameUpdater) {
	this->frameUpdater = frameUpdater;
}

void MediaStream::run() {
	init();
	receiveFrame();
}

//int main(int argc, char *argv[])
int MediaStream::init()
{
	std::cout << "MediaStream::init() " << std::endl ;
	bool r = ffmpegDecoder.init();
	if(!r) std::cout << "problem with ffmpeg decoder init"  << std::endl;
    //ffmpegDecoder.setOpenGLWidget(&xVideoWidget);

	ffmpegDecoder.setFrameUpdater(frameUpdater);

	rtspClient.DoDESCRIBE();
	std::cout << "DoDESCRIBE() " << std::endl ;
	//printf("SDP: %s\n", rtspClient.GetSDP().c_str());

	/* Parse SDP message after sending DESCRIBE command */
	rtspClient.ParseSDP();
	std::cout << "ParseSDP() " << std::endl ;
	//printf("SDP: %s\n", rtspClient.GetSDP().c_str());


	/* Send SETUP command to set up all 'audio' and 'video' 
	 * sessions which SDP refers. */

    /* USE TCP for receving */
	rtspClient.DoSETUP("video", true);
	rtspClient.SetVideoByeFromServerClbk(ByeFromServerClbk2);
	std::cout << "DoSETUP() " << std::endl ;

	printf("start PLAY\n");
	//printf("SDP: %s\n", rtspClient.GetSDP().c_str());

	/* Send PLAY command to play only 'video' sessions.*/
	rtspClient.DoPLAY("video");
	std::cout << "doplay started" << std::endl;
}

int MediaStream::receiveFrame() {
	int try_times = 0;
	//const size_t BufSize = 398304;
	//uint8_t buf[/*4 +*/ BufSize];//4 bytes for 0x00000001 at beggining
	//uint8_t* paddedBuf = buf + 4;
	const size_t bufferSize = 398304;//TODO: make it variable according to video's size
    uint8_t frameBuffer[bufferSize];

    //size_t write_size = 0;
	//std::cout << "accessed char" << std::endl;

	/* Write h264 video data to file "test_packet_recv.h264" 
	 * Then it could be played by ffplay */
	// int fd = open("test_packet_recv.h264", O_CREAT | O_RDWR, 0);
	//int fd = open("test_packet_recv.h264", O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR | S_IXUSR);

    /* By default, myRtsprtspClient will write SPS, PPS, VPS(H265 only) for H264/H265 video
       periodly when you invoke 'GetMediaData'. It could bring your video data stability.
       However, if you want a high performance, you could turn down this function and get 
       SPS, PPS, VPS by yourself just at the begining of the video data */
    /* For convenience, you could refer to "simple_example.cpp" */
    /* Get SPS, PPS, VPS manually start */
	//std::cout << "writing vps" << std::endl;
	/*
    rtspClient.SetObtainVpsSpsPpsPeriodly(false);
    if(!rtspClient.GetVPSNalu(paddedBuf, &size)) {
		if(write(fd, paddedBuf, size) < 0) {
			perror("write");
		}
    } 
    if(!rtspClient.GetSPSNalu(paddedBuf, &size)) {
		if(write(fd, paddedBuf, size) < 0) {
			perror("write");
		}
    } 
    if(!rtspClient.GetPPSNalu(paddedBuf, &size)) {
		if(write(fd, paddedBuf, size) < 0) {
			perror("write");
		}
    }
	*/ 
    /* Get SPS, PPS, VPS manually end */
    while (true) {
		size_t size = 0;

		if(!rtspClient.GetMediaData("video", frameBuffer, &size, bufferSize)) {
			if(ByeFromServerFlag2) {
				printf("ByeFromServerFlag\n");
				return 0;
			}
			if(try_times > 5) {
				printf("try_times > 5\n");
				break;
				return 1;
			}
			try_times++;
		}
		
		ffmpegDecoder.decodeFrame(frameBuffer, size);
	}
	return 0;

	/* lower the 'write' times to improve performance */
	/* For convenience, you could refer to "simple_example.cpp" */
	/*
	if(write_size > 32768) {
		if(write(fd, buf, write_size) < 0) {
			perror("write");
		}
		write_size = 0;
	}
	*/
	//try_times = 0;
	//printf("recv %lu\n", size);
	
	/*
    if(write_size > 0) {
        if(write(fd, buf, write_size) < 0) {
            perror("write");
        }
    }

	printf("start TEARDOWN\n");
    int err = rtspClient.DoTEARDOWN();
	// Send TEARDOWN command to teardown all of the sessions 
	if(err != RTSP_NO_ERROR && err != RTSP_INVALID_MEDIA_SESSION) {
		printf("DoTEARDOWN error: %d\n", err);
		return 0;
	}

	printf("%s\n", rtspClient.GetResponse().c_str());
	// Check whether server return '200'(OK) //
	if(!rtspClient.IsResponse_200_OK()) {
		printf("DoTEARDOWN error\n");
		return 0;
	}
	return 0;
	*/
}
