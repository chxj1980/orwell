#ifndef VIDEOSURFACE_H
#define VIDEOSURFACE_H
#include "MediaStream.h"
#include <qabstractvideosurface.h>
#include <QList>
#include <QVideoFrame>

class VideoSurface : public QAbstractVideoSurface {
    MediaStream * mediaStream;
    public:
        VideoSurface(MediaStream* mediaStream);
        QList<QVideoFrame::PixelFormat> supportedPixelFormats(
            QAbstractVideoBuffer::HandleType handleType) const;
        bool present(const QVideoFrame &frame);

};

#endif // VIDEOSURFACE_H