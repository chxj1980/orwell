#include "VideoProducer.h"
#include <QObject>

/*
QAbstractVideoSurface* MyVideoProducer::videoSurface() const { return abstractVideoSurface; }

void MyVideoProducer::setVideoSurface(QAbstractVideoSurface *surface)
{
    if (abstractVideoSurface != surface && abstractVideoSurface && abstractVideoSurface->isActive()) {
        abstractVideoSurface->stop();
    }
    abstractVideoSurface = surface;
    if (abstractVideoSurface)
        abstractVideoSurface->start(m_format);
}


void MyVideoProducer::onNewVideoContentReceived(const QVideoFrame &frame)
{
    if (abstractVideoSurface)
        abstractVideoSurface->present(frame);
}

*/