import QtQuick 2.0
import OpenGlVideoQtQuick 1.0
import OpenGlBufferQtQuick 1.0

Grid {
    columns: 2
    spacing: 2    
    width: 1280
    height: 720
    OpenGlBufferQtQuick {
        width: 640
        height: 360
        uri: "rtsp://admin:19929394@192.168.0.103:10554/tcp/av0_0"
    }
    OpenGlBufferQtQuick {
        width: 640
        height: 360
        uri: "rtsp://admin:19929394@192.168.0.101:10554/tcp/av0_0"
    }
    OpenGlBufferQtQuick {
        width: 640
        height: 360
        uri: "rtsp://admin:19929394@192.168.0.104:10554/tcp/av0_0"
    }
    OpenGlBufferQtQuick {
        width: 640
        height: 360
        uri: "rtsp://admin:19929394@192.168.1.43:10554/tcp/av0_0"
    }
    
}
