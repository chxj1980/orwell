#include "FfmpegSoftwareDecoder.h"

int FfmpegSoftwareDecoder::init()
{
    //avcodec_register_all(); //Deprecated
	avFrame = av_frame_alloc();

	avCodec = avcodec_find_decoder(AV_CODEC_ID_H264);

	if (!avCodec) {
		std::cout << "avcodec_find_decoder problem" << std::endl;
		return 1;
	}

	avCodecContext = avcodec_alloc_context3(avCodec);

	if (avcodec_open2(avCodecContext, avCodec, NULL) < 0) {
		std::cout << "avcodec_open2 problem" << std::endl;
		return 2;
	}

	return 0;
}

//https://stackoverflow.com/questions/30784549/best-simplest-way-to-display-ffmpeg-frames-in-qt5
void FfmpegSoftwareDecoder::decodeFrame(uint8_t* frameBuffer, int frameLength)
{	
	decodeFrame(frameBuffer, frameLength, frame);
	this->videoReceiver->receiveVideo(frame);
}

void FfmpegSoftwareDecoder::decodeFrame(uint8_t* frameBuffer, int frameLength, Frame* frame)
{
	if (frameLength <= 0) return;

	int frameFinished = 0;
	
	AVPacket* avPacket = av_packet_alloc();
	if (!avPacket) std::cout << "av packet error" << std::endl;
	//AVRational tb = avCodecContext->time_base; //For fixed-fps content, timebase should be 1/framerate and timestamp increments should be identically 1. This often, but not always is the inverse of the frame rate or field rate for video. 1/time_base is not the average frame rate if the frame rate is not constant.
    //AVRational frameRate = av_guess_frame_rate(avFormatContext, avStream, NULL); //Guess the frame rate, based on both the container and codec information. 

	avPacket->size = frameLength;
	avPacket->data = frameBuffer;

    //https://github.com/saki4510t/pupilClient/blob/0e9f7bdcfe9f5fcb197b1c2408a6fffb90345f8d/src/media/h264_decoder.cpp#L119
	//Disable ffmpeg annoying output
	av_log_set_level(AV_LOG_QUIET); 

	int sendPacketResult = avcodec_send_packet(avCodecContext, avPacket);

	if (!sendPacketResult) {
		int receiveFrameResult = avcodec_receive_frame(avCodecContext, avFrame);
		if (!receiveFrameResult) {
			/*
				Most important code line. If we ended here, it means avFrame has the decoded frame.
				We just need to convert our avFrame to a generic Frame object. 
				Now caller has a video frame and can render it.
			*/
			FfmpegDecoder::avFrameToFrame(avFrame, frame);
		} else if ((receiveFrameResult < 0) && (receiveFrameResult != AVERROR(EAGAIN)) && (receiveFrameResult != AVERROR_EOF)) {
			std::cout << "avcodec_receive_frame returned error " << receiveFrameResult << std::endl;
		} else {
			switch (receiveFrameResult) {
				//Not exactly an error, we just have to wait for more data
				case AVERROR(EAGAIN):
					break;
				//To be done: what does this error mean? I think it's literally the end of an mp4 file
				case AVERROR_EOF:
					std::cout << "avcodec_receive_frame AVERROR_EOF" << std::endl;
					break;
				//To be done: describe what error is this in std cout before stopping
				default:
					std::cout << "avcodec_receive_frame returned error, stopping..." << receiveFrameResult /*<< av_err2str(result).c_str()*/ << std::endl;
					break;
				//Error happened, should break anyway
				break;
			}
		}
	} else {
		switch (sendPacketResult) {
			case AVERROR(EAGAIN):
				std::cout << "avcodec_send_packet EAGAIN" << std::endl;
				break;
			case AVERROR_EOF:
				std::cout << "avcodec_send_packet AVERROR_EOF" << std::endl;
				break;
			//To be done: debug which erros are these. Last time I checked it was a bunch of -1094995529 errors
			default:
				//std::cout << "ffmpeg default unknown error number " << result << " for " << this->uri << std::endl;
				break;
		}
	}

}
	//if (frameFinished) {
		/*
		double dpts = NAN;

		if (avFrame->pts != AV_NOPTS_VALUE)
			dpts = av_q2d(avStream->time_base) * avFrame->pts;

		avFrame->sample_aspect_ratio = av_guess_sample_aspect_ratio(avFormatContext, avStream, avFrame);

		AVRational avRational;
		avRational.num = frameRate.den;
		avRational.den = frameRate.num;
		duration = (frameRate.num && frameRate.den ? av_q2d(avRational) : 0);
		pts = (frame->pts == AV_NOPTS_VALUE) ? NAN : frame->pts * av_q2d(tb);
		*/

		//int width = avFrame->width;
		//int height= avFrame->height;
		/*
		AvPicture* picture;
		picture = avcodec_alloc_frame();
		int ret = avpicture_alloc(picture, AV_PIX_FMT_YUV420P, width, height);
		//mVideoRender->setSize(width,height);
		//mVideoRender->start();
		//if(ret==0)
		//	picInit=true;

		
		
		//avpicture_fill(&picture, video_buf, AV_PIX_FMT_YUV420P,width, height);

		//Context for scaling
		convertContext = sws_getCachedContext(convertContext, width, height, avCodecContext->pix_fmt,
												width,height,AV_PIX_FMT_YUV420P, SWS_FAST_BILINEAR,
												NULL, NULL, NULL);

		if (!convertContext) qDebug("Cannot initialize the conversion context\n");

		//Lets scale our frame
		int rr = sws_scale(convertContext, avFrame->data, avFrame->linesize,
						   0, frame->height, picture->data, picture->linesize);
		*/
		/*
		if(dpts > mClock->getClockTime())
		{
			QThread::msleep((dpts-mClock->getClockTime())*1000);
		}
		*/
		
		//av_frame_unref(avFrame);
	//}
