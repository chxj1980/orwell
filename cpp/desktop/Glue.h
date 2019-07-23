#ifndef GLUE_H
#define GLUE_H
#include <iostream>
#include <QDebug>
#include <QFileInfo>
#include <QDir>
#include <QVariant>
#include <QByteArray>
#include <memory>
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>
#include "VideoReceiver.h"
#include "VideoRecorder.h"
#include "MediaStream.h"


struct GlueObject {
    std::shared_ptr<MediaStream> mediaStream;
    std::shared_ptr<boost::thread> mediaThread;
    std::shared_ptr<VideoReceiver> videoReceiver;
    std::shared_ptr<VideoRecorder> videoRecorder;
    //std::shared_ptr<MovementTracker> movementTracker
};

//https://stackoverflow.com/questions/1008019/c-singleton-design-pattern/40337728#40337728
class Glue
{
public:
    Glue(Glue const&) = delete;
    Glue& operator=(Glue const&) = delete;

    static void addStream(QString id, GlueObject glueObject) {
        mutex.lock();
        streamList.insert(id, glueObject);
        mutex.unlock();
    }

    static GlueObject getStream(QString id) {
        mutex.lock();
        GlueObject glueObject = streamList.value(id);
        mutex.unlock();
        return glueObject;
    }

    //Very important. Must be called from React Native when view is being changed, 
    //so the Media Player gets destroyed and there's no place for ffmpegDecoder to send
    //video data. Otherwise it tries to send data do invalid object.
    static void DisableStreamReceiver(QString id) {
        mutex.lock();
        GlueObject glueObject = streamList.value(id);
        glueObject.mediaStream->ffmpegDecoder->disableVideoReceiver();
        mutex.unlock();
    }

    static std::shared_ptr<Glue> instance()
    {
        static std::shared_ptr<Glue> s{new Glue};
        return s;
    }


private:
    Glue() {}
    static QMap<QString,GlueObject> streamList;
    static boost::mutex mutex;
};

#endif // GLUE_H
