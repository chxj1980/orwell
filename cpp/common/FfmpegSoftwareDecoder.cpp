#include "FfmpegSoftwareDecoder.h"

int FfmpegSoftwareDecoder::init()
{
	if (codec==H264)
		avCodec = avcodec_find_decoder(AV_CODEC_ID_H264);
	else if(codec==H265){
		//avCodec = avcodec_find_decoder(AV_CODEC_ID_H265);
		std::cout << "H265 not yet supported" << std::endl;
		return -1;
	}

	if (!avCodec)
	{
		std::cout << "avcodec_find_decoder problem" << std::endl;
		return -2;
	}

	avCodecContext.reset(avcodec_alloc_context3(avCodec));

	int ret = avcodec_open2(avCodecContext.get(), avCodec, NULL);
	if (ret < 0)
	{
		std::cout << "avcodec_open2 problem: " << ret << std::endl;
		return -3;
	}

	return 0;
}

//https://stackoverflow.com/questions/30784549/best-simplest-way-to-display-ffmpeg-frames-in-qt5
int FfmpegSoftwareDecoder::decodeFrame(uint8_t *frameBuffer, int frameLength)
{
	Frame frame;
	frame.decodedFrom = Frame::FFMPEG;
	int r = decodeFrame(frameBuffer, frameLength, frame);
	if (!decodedFramesFifo) {
		std::cerr << "No decodedFramesFifo setted in FfmpegSoftwareDecoder" << std::endl;
	}
	if (r == 0)
		this->decodedFramesFifo->emplace_back(std::move(frame));
	return r;
	
}

int FfmpegSoftwareDecoder::decodeFrame(uint8_t *frameBuffer, int frameLength, Frame &frame)
{
	//Disable ffmpeg annoying output
	av_log_set_level(AV_LOG_QUIET);
	if (frameLength <= 0)
		return -1;

	int frameFinished = 0;

	std::unique_ptr<AVPacket, AVPacketDeleter> avPacket(av_packet_alloc());
	if (!avPacket.get())
		std::cout << "av packet error" << std::endl;
	
	avPacket.get()->size = frameLength;
	avPacket.get()->data = frameBuffer;

	//https://github.com/saki4510t/pupilClient/blob/0e9f7bdcfe9f5fcb197b1c2408a6fffb90345f8d/src/media/h264_decoder.cpp#L119

	int sendPacketResult = avcodec_send_packet(avCodecContext.get(), avPacket.get());

	std::unique_ptr<AVFrame, AVFrameDeleter> avFrame(av_frame_alloc());
	if (!sendPacketResult)
	{
		int receiveFrameResult = avcodec_receive_frame(avCodecContext.get(), avFrame.get());
		if (!receiveFrameResult)
		{
			/*
				Most important code line. If we ended here, it means avFrame has the decoded frame.
				We just need to move our avFrame to a generic Frame object. 
				Now caller has a video frame and can render it.
			*/
			frame.avFrame = std::move(avFrame);
			return 0;
		}
		else if ((receiveFrameResult < 0) && (receiveFrameResult != AVERROR(EAGAIN)) && (receiveFrameResult != AVERROR_EOF))
		{
			std::cout << "avcodec_receive_frame returned error " << receiveFrameResult << std::endl;
			return -2;
		}
		else
		{
			//TODO: return numbers here indicating errors
			switch (receiveFrameResult)
			{
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
	}
	else
	{
		switch (sendPacketResult)
		{
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
