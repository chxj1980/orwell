#include "FfmpegSoftwareDecoder.h"
#include "SLog.h"
SLOG_CATEGORY("FfmpegSoftwareDecoder");

FfmpegSoftwareDecoder::FfmpegSoftwareDecoder(Codec codec)
{
	this->codec = codec;
	if (codec == H264)
		avCodec = avcodec_find_decoder(AV_CODEC_ID_H264);
	else if (codec == H265)
	{
		//avCodec = avcodec_find_decoder(AV_CODEC_ID_H265);
		LOG << "H265 not yet supported";
		//return -1;
	}

	if (!avCodec)
	{
		LOG << "avcodec_find_decoder problem";
		//return -2;
	}

	avCodecContext.reset(avcodec_alloc_context3(avCodec));

	int ret = avcodec_open2(avCodecContext.get(), avCodec, NULL);
	if (ret < 0)
	{
		LOG << "avcodec_open2 problem: " << ret;
		//return -3;
	}
}

//https://stackoverflow.com/questions/30784549/best-simplest-way-to-display-ffmpeg-frames-in-qt5
int FfmpegSoftwareDecoder::decodeFrame(std::shared_ptr<EncodedPacket> encodedPacket)
{
	auto decodedFfmpegFrame = std::dynamic_pointer_cast<DecodedFfmpegFrame>(decodedFramesFifo->pop_front());
	decodedFfmpegFrame->decodedFrom = DecodedFrame::FFMPEG;
	int r = decodeFrame(encodedPacket, decodedFfmpegFrame);
	if (!decodedFramesFifo)
	{
		LOG << "No decodedFramesFifo setted in FfmpegSoftwareDecoder";
	}
	if (r == 0)
		this->decodedFramesFifo->emplace_back(decodedFfmpegFrame);
	return r;
}

int FfmpegSoftwareDecoder::decodeFrame(std::shared_ptr<EncodedPacket> encodedPacket, std::shared_ptr<DecodedFrame> decodedFrame)
{
	std::shared_ptr<DecodedFfmpegFrame> decodedFfmpegFrame = std::dynamic_pointer_cast<DecodedFfmpegFrame>(decodedFrame);
	//Disable ffmpeg annoying output
	av_log_set_level(AV_LOG_QUIET);
	if (encodedPacket->getSize() <= 0)
		return -1;

	int frameFinished = 0;

	std::unique_ptr<AVPacket, AVPacketDeleter> avPacket(av_packet_alloc());
	if (!avPacket.get())
		LOG << "av packet error";

	avPacket.get()->size = encodedPacket->getSize();
	avPacket.get()->data = encodedPacket->getFramePointer();

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
			decodedFfmpegFrame->avFrame->width = avFrame->width;
			decodedFfmpegFrame->avFrame->height = avFrame->height;
			decodedFfmpegFrame->avFrame->format = avFrame->format;
			for (int i=0; i<AV_NUM_DATA_POINTERS; i++) {
				decodedFfmpegFrame->avFrame->linesize[i] = avFrame->linesize[i];
				decodedFfmpegFrame->avFrame->data[i] = avFrame->data[i];
			}
			decodedFfmpegFrame->avFrame = std::move(avFrame);
			return 0;
		}
		else if ((receiveFrameResult < 0) && (receiveFrameResult != AVERROR(EAGAIN)) && (receiveFrameResult != AVERROR_EOF))
		{
			LOG << "avcodec_receive_frame returned error " << receiveFrameResult;
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
				LOG << "avcodec_receive_frame AVERROR_EOF";
				break;
			//To be done: describe what error is this in std cout before stopping
			default:
				LOG << "avcodec_receive_frame returned error, stopping..." << receiveFrameResult /*<< av_err2str(result).c_str()*/;
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
			LOG << "avcodec_send_packet EAGAIN";
			break;
		case AVERROR_EOF:
			LOG << "avcodec_send_packet AVERROR_EOF";
			break;
		//To be done: debug which erros are these. Last time I checked it was a bunch of -1094995529 errors
		default:
			//LOG << "ffmpeg default unknown error number " << result << " for " << this->uri;
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
