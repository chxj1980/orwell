import QtQuick 2.0
import OpenGLComponents 1.0
Item {
    width: 1280
    height: 720
    OpenGlVideoQtQuick {
        width: 640
        height: 480
        uri: "rtsp://admin:19929394@192.168.0.103:10554/tcp/av0_0"
    }
}