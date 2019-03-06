import QtQuick 2.0
import OpenGLComponents 1.0
Item {
    width: 1280
    height: 720
    OpenGlBufferItem {
        x:0
        y: 0
        width: 640
        height: 360
        uri: "rtsp://admin:19929394@192.168.0.103:10554/tcp/av0_0"
    }
    OpenGlBufferItem {
        x:640
        y: 0
        width: 640
        height: 360
        uri: "rtsp://admin:19929394@192.168.0.104:10554/tcp/av0_0"
    }
}
