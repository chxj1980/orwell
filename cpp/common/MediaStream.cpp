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
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include "MediaStream.h"

using std::cout;
using std::endl;

bool ByeFromServerFlag2 = false;
void ByeFromServerClbk2()
{
	cout << "Server send BYE" << endl;
	ByeFromServerFlag2 = true;
}
//TODO: if uri is rtsp, do one thing. If it's a .mp4 file, do other thing
MediaStream::MediaStream (std::string uri):rtspClient(uri),uri(uri) {
}


void MediaStream::run() {
	//init();
	while (true) {
		while (init()!=0) {}
		receiveFrame();
		//break;
	}
}

int MediaStream::init()
{	
	if (firstConnection) {
		//std::cout << "RTSP connection for " << this->uri << std::endl;
	} else {
		//std::cout << "RTSP reconnection for " << this->uri << std::endl;
	}
	firstConnection = false;
	
	if (!ffmpegDecoder) {
		//ERROR, return
	}

	bool r = ffmpegDecoder->init();
	if(!r) std::cout << "problem with ffmpeg decoder init"  << std::endl;

	//ffmpegDecoder.setFrameUpdater(frameUpdater);
	
	//printf("DoOPTIONS():\n");
	if(rtspClient.DoOPTIONS() != RTSP_NO_ERROR) {
		//printf("DoOPTIONS error\n");
		return 1;
	}
	//printf("%s\n", rtspClient.GetResponse().c_str());
	/* Check whether server return '200'(OK) */
	if(!rtspClient.IsResponse_200_OK()) {
		printf("DoOPTIONS error\n");
		printf("%s\n", rtspClient.GetSDP().c_str());
		//init();
		return 2;
	}
	//printf("DoDESCRIBE():\n");
	/* Send DESCRIBE command to server */
	if(rtspClient.DoDESCRIBE() != RTSP_NO_ERROR) {
		printf("DoDESCRIBE error\n");
		printf("%s\n", rtspClient.GetSDP().c_str());
		//init();
		return 3;
	}
	//printf("%s\n", rtspClient.GetResponse().c_str());
	/* Check whether server return '200'(OK) */
	if(!rtspClient.IsResponse_200_OK()) {
		printf("DoDESCRIBE error\n");
		//init();
		printf("%s\n", rtspClient.GetSDP().c_str());
		return 4;
	}

	/* Parse SDP message after sending DESCRIBE command */
	//printf("%s\n", rtspClient.GetSDP().c_str());
	if(rtspClient.ParseSDP() != RTSP_NO_ERROR) {
		printf("ParseSDP error\n");
		return 5;
	}

	/* Send SETUP command to set up all 'audio' and 'video' 
	 * sessions which SDP refers. */
	//printf("DoSETUP():\n");

	if(rtspClient.DoSETUP("video", true) != RTSP_NO_ERROR) {//TODO: this DoSETUP actually works only for vstarcam cameras, change it to work with anything
		printf("DoSETUP error\n");
		printf("%s\n", rtspClient.GetResponse().c_str());
		return 6;
	}
	rtspClient.SetVideoByeFromServerClbk(ByeFromServerClbk2);
	//printf("%s\n", rtspClient.GetResponse().c_str());
	
	/* Check whether server return '200'(OK) */
	if(!rtspClient.IsResponse_200_OK()) {
		printf("DoSETUP error\n");
		return 7;
	}

	//printf("start PLAY\n");
	/* Send PLAY command to play only 'video' sessions.
	 * note(FIXME): 
	 * if there are several 'video' session 
	 * refered in SDP, only play the first 'video' 
	 * session, the same as 'audio'.*/
	if(rtspClient.DoPLAY("video", NULL, NULL, NULL) != RTSP_NO_ERROR) {
		printf("DoPLAY error\n");
		printf("%s\n", rtspClient.GetResponse().c_str());
		return 8;
	}
	
	//printf("%s\n", rtspClient.GetResponse().c_str());
	/* Check whether server return '200'(OK) */
	if(!rtspClient.IsResponse_200_OK()) {
		printf("DoPLAY error\n");
		return 9;
	}
	return 0;
}

int MediaStream::receiveFrame() {
	int try_times = 0;
	//const size_t BufSize = 398304;
	//uint8_t buf[/*4 +*/ BufSize];//4 bytes for 0x00000001 at beggining
	//uint8_t* paddedBuf = buf + 4;
	const size_t bufferSize = 408304;//TODO: make it variable according to video's size
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
	unsigned int a = 0;

    while (true) {
		size_t size = 0;
		a++;

		if(!rtspClient.GetMediaData("video", frameBuffer, &size, bufferSize)) {
			if(ByeFromServerFlag2) {
				printf("ByeFromServerFlag\n");
				return 0;
			}
			if(try_times > 5) {
				std::cout << "RTSP connection lost for " << this->uri << std::endl;
				break;
				return 1;
			}
			try_times++;
		} else {
			ffmpegDecoder->decodeFrame(frameBuffer, size);
		}
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
