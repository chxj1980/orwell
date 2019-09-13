#include "ZLRTSPClient.h"
#include "SLog.h"
#include "EncodedPacket.h"

int ZLRTSPClient::init()
{
    LOG << "ZRTSPClient init for " << this->uri;
    weak_ptr<MediaPlayer> weakPlayer = player;
    player->setOnPlayResult([weakPlayer, this](const SockException &ex) {
        this->LOG << "OnPlayResult: " << ex.what() << " for " << this->uri;
        auto strongPlayer = weakPlayer.lock();
        if (ex || !strongPlayer)
        {
            return;
        }

        auto viedoTrack = strongPlayer->getTrack(TrackVideo);
        if (!viedoTrack)
        {
            this->LOG << "No video track for " << uri;
            return;
        }
        viedoTrack->addDelegate(std::make_shared<FrameWriterInterfaceHelper>([this](const Frame::Ptr &frame) {
            if (!this->encodedPacketsFifo)
            {
                LOG(SLog::CRITICAL_ERROR) << "ZLRTSPClient: no encodedPacketsFifo setted, nowhere to send RTSP data for " << uri;
            }
            else
            {
                auto zLRTSPEncodedPacket = std::make_shared<ZLRTSPEncodedPacket>(frame);
                bytesPerSecond->profile(
                        [&frame](int& counter) {
                            counter += frame->size();
                        },
                        [](int& counter) {
                            counter = 0;
                        });
                encodedPacketsFifo->emplace_back(zLRTSPEncodedPacket);
                //LOG << "size: " << frame->size();
                LOG << bytesPerSecond->getSampleString() << "kB/s";
            }
        }));
    });

    player->setOnResume([this]() {
        LOG << "onResume for " << uri;
    });

    player->setOnShutdown([this](const SockException &ex) {
        LOG << "OnShutdown:" << ex.what();
    });
    return 0;
}