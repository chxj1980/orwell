#include <iostream>
#include "VideoSurface.h"

VideoSurface::VideoSurface(MediaStream* mediaStream) {
    this->mediaStream = mediaStream;
    //this->mediaStream->init();
}

QList<QVideoFrame::PixelFormat> VideoSurface::supportedPixelFormats(QAbstractVideoBuffer::HandleType handleType = QAbstractVideoBuffer::NoHandle) const
{
    Q_UNUSED(handleType);
    std:: cout << "VideoSurface supportedPixelFormats called " << std::endl; 

    // Return the formats you will support
    return QList<QVideoFrame::PixelFormat>() << QVideoFrame::Format_YUV420P;
}

bool VideoSurface::present(const QVideoFrame &frame)
{
    //Q_UNUSED(frame);
    std:: cout << "VideoSurface processing 1 frame " << std::endl; 

    QVideoFrame frametodraw(frame);

    if(!frametodraw.map(QAbstractVideoBuffer::ReadOnly))
    {
        setError(ResourceError);
        return false;
    } 
    // Handle the frame and do your processing
    const size_t bufferSize = 398304;
    uint8_t frameBuffer[bufferSize];
    this->mediaStream->receiveFrame();
    //frame.map(QAbstractVideoBuffer::ReadWrite);
    //https://github.com/bakhtiyarov/QCameraExample/blob/c399f8eb20a6fa4e88bd229335ea9c0fda394adb/CameraImageProcessor.cpp#L19
    //std::copy( frame.bits(), frame.bits() + frame.bytesPerLine()*frame.height(),frameBuffer->bits() );
    //Frame is now in frameBuffer, we must but into frame
    //frame.map(QAbstractVideoBuffer::ReadWrite);
    //Copy?????
    //frame.bits() = (uchar *) frameBuffer 
    //emit VideoFrameProbed(frame);
    //frame.unmap();
    frametodraw.unmap();
    return true;
}
