#ifndef Ffmpeg_Deleters_H
#define Ffmpeg_Deleters_H
extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavutil/frame.h>
#include <libavutil/hwcontext.h>
}

struct AVPacketDeleter
{
	void operator()(AVPacket *avPacket) { av_packet_unref(avPacket); }
};

struct AVFrameDeleter
{
	void operator()(AVFrame *avFrame) { av_frame_unref(avFrame); }
};

struct AVBufferRefDeleter
{
	void operator()(AVBufferRef *avBufferRef) { av_buffer_unref(&avBufferRef); }
};

struct AVCodecContextDeleter
{
	void operator()(AVCodecContext *avCodecContext) { avcodec_free_context(&avCodecContext); }
};
#endif //Ffmpeg_Deleters_H