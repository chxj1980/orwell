import QtQuick 2.0
import OpenGLComponents 1.0
Grid {
    columns: 2
    spacing: 2    
    width: 1280
    height: 720
    OpenGlVideoQtQuick {
        width: 640
        height: 360
        uri: "rtsp://admin:19929394@192.168.0.103:10554/tcp/av0_0"
    }
    OpenGlVideoQtQuick {
        width: 640
        height: 360
        uri: "rtsp://admin:19929394@192.168.0.101:10554/tcp/av0_0"
    }
    OpenGlVideoQtQuick {
        width: 640
        height: 360
        uri: "rtsp://admin:19929394@192.168.0.104:10554/tcp/av0_0"
    }
    OpenGlVideoQtQuick {
        width: 640
        height: 360
        uri: "rtsp://admin:19929394@192.168.1.43:10554/tcp/av0_0"
    }
    
}
