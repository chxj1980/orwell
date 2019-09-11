#ifndef ZLRTSPClient_H
#define ZLRTSPClient_H
#include "RTSPClient.h"
#include "Player/MediaPlayer.h"
#include "SLog.h"
class ZLRTSPClient : public RTSPClient
{
public:
    using RTSPClient::RTSPClient;

    ZLRTSPClient(std::string uri, Transport transport) : RTSPClient(uri, transport), player(new MediaPlayer())
    {
    }
    ZLRTSPClient(std::string uri, Transport transport, std::string user,
                 std::string password) : RTSPClient(uri, transport, user, password), player(new MediaPlayer())
    {
    }

    int init();
    void run(){};
    int receivePacket(){};
    /*
        ZLRTSP library already has thread support, so we simply play,
        no need to do it in a while loop, neither implement void run
    */
    void startThreadMode()
    {
        init();
        if (transport == RTP_OVER_TCP)
        {
            (*player)[Client::kRtpType] = Rtsp::RTP_TCP;
            player->play(uri);
        }
    }

protected:
    MediaPlayer::Ptr player;
    //We have to create a non static log here in order to pass inside the lambda functions
    SLog::SLog LOG{SLog::Category{"ZLRTSPClient"}};
};

#endif //ZLRTSPClient_H