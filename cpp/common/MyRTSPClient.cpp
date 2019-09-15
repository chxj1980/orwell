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
#include "MyRTSPClient.h"
#include "SLog.h"
#include <chrono>
#include <thread>
SLOG_CATEGORY("myRtspClient");

bool ByeFromServerFlag2 = false;
void ByeFromServerClbk2()
{
	//cout << "Server send BYE" << endl;
	ByeFromServerFlag2 = true;
}

void MyRTSPClient::run()
{
	while (shouldContinue())
	{
		while (init() != 0)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		}
		receivePacket();
		LOG << "RTSP connection LOST for " << this->uri;
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
}

int MyRTSPClient::init()
{
	if (firstConnection)
	{
		//std::cout << "RTSP connection for " << this->uri << std::endl;
		LOG << "RTSP connection for " << this->uri;
		firstConnection = false;
	}
	else
	{
		//LOG << "RTSP reconnection for " << this->uri;
	}
	if (!username.empty() || !password.empty())
	{
		myRtspClient.SetUsername(username);
		myRtspClient.SetPassword(password);
		LOG << "username: " << username;
		LOG << "password: " << password;
	}

	//TODO: init this only one time

	//decoder.setFrameUpdater(frameUpdater);
	if (transport == RTP_OVER_HTTP)
	{
		myRtspClient.SetHttpTunnelPort(8000);
		myRtspClient.SetPort(8000);
		if (myRtspClient.DoRtspOverHttpGet() == RTSP_NO_ERROR)
		{
			LOG << "DoGet OK for " << this->uri;
			;
		}
		LOG << myRtspClient.GetResource();
		if (myRtspClient.DoRtspOverHttpPost() == RTSP_NO_ERROR)
		{
			LOG << "DoPost OK for " << this->uri;
			;
		}
	}
	//printf("DoOPTIONS():\n");
	if (myRtspClient.DoOPTIONS() != RTSP_NO_ERROR)
	{
		//printf("DoOPTIONS error\n");
		LOG << "RTSP DoOPTIONS() error for " << this->uri;

		return 1;
	}
	//printf("%s\n", myRtspClient.GetResponse().c_str());
	/* Check whether server return '200'(OK) */
	if (!myRtspClient.IsResponse_200_OK())
	{
		//printf("DoOPTIONS error\n");
		//printf("%s\n", myRtspClient.GetSDP().c_str());
		LOG << "RTSP DoOPTIONS() not ok for " << this->uri;
		//init();
		return 2;
	}
	//printf("DoDESCRIBE():\n");
	/* Send DESCRIBE command to server */
	if (myRtspClient.DoDESCRIBE() != RTSP_NO_ERROR)
	{
		//printf("DoDESCRIBE error\n");
		//printf("%s\n", myRtspClient.GetSDP().c_str());
		LOG << "DoDESCRIBE() error for " << this->uri;
		//init();
		return 3;
	}
	//printf("%s\n", myRtspClient.GetResponse().c_str());
	/* Check whether server return '200'(OK) */
	if (!myRtspClient.IsResponse_200_OK())
	{
		//printf("DoDESCRIBE error\n");
		//init();
		//printf("%s\n", myRtspClient.GetSDP().c_str());
		LOG << "DoDESCRIBE() response error for " << this->uri;
		return 4;
	}

	/* Parse SDP message after sending DESCRIBE command */
	//printf("%s\n", myRtspClient.GetSDP().c_str());
	if (myRtspClient.ParseSDP() != RTSP_NO_ERROR)
	{
		//printf("ParseSDP error\n");
		return 5;
	}

	/* Send SETUP command to set up all 'audio' and 'video' 
	 * sessions which SDP refers. */
	//printf("DoSETUP():\n");

	if (myRtspClient.DoSETUP("video", true) != RTSP_NO_ERROR)
	{ //TODO: this DoSETUP actually works only for vstarcam cameras, change it to work with anything
		//printf("DoSETUP error\n");
		//printf("%s\n", myRtspClient.GetResponse().c_str());
		LOG << "DoSETUP() response error for " << this->uri;
		return 6;
	}
	myRtspClient.SetVideoByeFromServerClbk(ByeFromServerClbk2);
	//printf("%s\n", myRtspClient.GetResponse().c_str());

	/* Check whether server return '200'(OK) */
	if (!myRtspClient.IsResponse_200_OK())
	{
		//printf("DoSETUP error\n");
		return 7;
	}

	//printf("start PLAY\n");
	/* Send PLAY command to play only 'video' sessions.
	 * note(FIXME): 
	 * if there are several 'video' session 
	 * refered in SDP, only play the first 'video' 
	 * session, the same as 'audio'.*/
	if (myRtspClient.DoPLAY("video", NULL, NULL, NULL) != RTSP_NO_ERROR)
	{
		//printf("DoPLAY #include <stdio.h> error\n");
		//printf("%s\n", myRtspClient.GetResponse().c_str());
		return 8;
	}

	//printf("%s\n", myRtspClient.GetResponse().c_str());
	/* Check whether server return '200'(OK) */
	if (!myRtspClient.IsResponse_200_OK())
	{
		//printf("DoPLAY error\n");
		LOG << "DoPlay error";
		return 9;
	}
	else
	{
		LOG << "RTSP reconnection for " << this->uri;
	}
	return 0;
}

int MyRTSPClient::receivePacket()
{
	int try_times = 0;
	/* Get SPS, PPS, VPS manually end */
	unsigned int a = 0;
	size_t bufferSize = 408304;
	while (true)
	{
		//EncodedPacket frame(bufferSize);
		auto myRtspEncodedPacket = std::make_shared<MyRTSPEncodedPacket>(bufferSize);
		a++;

		if (!myRtspClient.GetMediaData("video", myRtspEncodedPacket->frame.get(), &myRtspEncodedPacket->size, bufferSize))
		{
			if (ByeFromServerFlag2)
			{
				printf("ByeFromServerFlag\n");
				return 0;
			}
			if (try_times > 5)
			{
				LOG << "RTSP connection for " << this->uri;
				break;
				return 1;
			}
			try_times++;
		}
		else
		{
			onNewPacket(myRtspEncodedPacket);
			//decoder->sendPacket(frameBuffer, size);
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
    int err = myRtspClient.DoTEARDOWN();
	// Send TEARDOWN command to teardown all of the sessions 
	if(err != RTSP_NO_ERROR && err != RTSP_INVALID_MEDIA_SESSION) {
		printf("DoTEARDOWN error: %d\n", err);
		return 0;
	}

	printf("%s\n", myRtspClient.GetResponse().c_str());
	// Check whether server return '200'(OK) //
	if(!myRtspClient.IsResponse_200_OK()) {
		printf("DoTEARDOWN error\n");
		return 0;
	}
	return 0;
	*/
}
