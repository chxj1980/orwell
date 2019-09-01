#include "MediaCodecDecoder.h"
MediaCodecDecoder::MediaCodecDecoder(Codec codec) {
    this->codec = codec;
    if (codec == H264)
        aMediaCodec.reset(AMediaCodec_createDecoderByType("TYPE_HERE"));
    else if (codec == H265)
    {
       LOG(SLog::ERROR) << "H265 not yet supported";
    }
}

int MediaCodecDecoder::decodeFrame(EncodedUnit &encodedUnit) {

}
int MediaCodecDecoder::decodeFrame(EncodedUnit &encodedUnit, DecodedFrame &decodedFrame) {
    ssize_t bufidx = -1;
    bufidx = AMediaCodec_dequeueInputBuffer(aMediaCodec.get(), 2000);
    if (bufidx >= 0) {
        size_t bufsize;
        auto buf = AMediaCodec_getInputBuffer(aMediaCodec.get(), bufidx, &bufsize);


        AMediaCodec_queueInputBuffer(aMediaCodec.get(), bufidx, 0, encodedUnit.frameSize,
                                     presentationTimeUs,
                                     AMEDIACODEC_BUFFER_FLAG_PARTIAL_FRAME);
        //AMediaExtractor_advance(d->ex);
    }

    AMediaCodecBufferInfo info;
    auto status = AMediaCodec_dequeueOutputBuffer(aMediaCodec.get(), &info, 0);
    if (status >= 0) {
        if (info.flags & AMEDIACODEC_BUFFER_FLAG_END_OF_STREAM) {
            //LOGV("output EOS");
            //d->sawOutputEOS = true;
        }
        int64_t presentationNano = info.presentationTimeUs * 1000;
        //if (d->renderstart < 0) {
        //    d->renderstart = systemnanotime() - presentationNano;
        //}
        //int64_t delay = (d->renderstart + presentationNano) - systemnanotime();
        //if (delay > 0) {
        //    usleep(delay / 1000);
        //}
        AMediaCodec_releaseOutputBuffer(aMediaCodec.get(), status, info.size != 0);
        //if (d->renderonce) {
        //    d->renderonce = false;
        //    return;

    } else if (status == AMEDIACODEC_INFO_OUTPUT_BUFFERS_CHANGED) {
        //LOGV("output buffers changed");
    } else if (status == AMEDIACODEC_INFO_OUTPUT_FORMAT_CHANGED) {
        auto format = AMediaCodec_getOutputFormat(aMediaCodec.get());
        //LOGV("format changed to: %s", AMediaFormat_toString(format));
        AMediaFormat_delete(format);
    } else if (status == AMEDIACODEC_INFO_TRY_AGAIN_LATER) {
        //LOGV("no output buffer right now");
    } else {
        LOG(SLog::ERROR) << "unexpected info code: " << status;
    }
}
