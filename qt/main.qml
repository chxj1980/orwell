import QtQuick 2.0
import OpenGlVideoQtQuick 1.0
import OpenGlVideoQtQuick2 1.0
import OpenGlBufferQtQuick 1.0

Grid {
    columns: 2
    spacing: 2    
    width: 1280
    height: 720
 /*
    OpenGlBufferQtQuick {
        width: 640
        height: 360
        uri: "rtsp://admin:19929394@192.168.1.178:10554/tcp/av0_0"
    }
    */
/*
    OpenGlVideoQtQuick {
        width: 640
        height: 360
        uri: "rtsp://admin:19929394@192.168.0.103:10554/tcp/av0_0"
    }
    OpenGlVideoQtQuick {
        width: 640
        height: 360
        uri: "rtsp://admin:19929394@192.168.0.104:10554/tcp/av0_0"
    }
    OpenGlVideoQtQuick {
        width: 640
        height: 360
        uri: "rtsp://admin:19929394@192.168.0.106:10554/tcp/av0_0"
    }
*/
    OpenGlVideoQtQuick {
        width: 640
        height: 360
        uri: "rtsp://admin:19929394@192.168.1.198:10554/tcp/av0_0"
    }
    OpenGlVideoQtQuick {
        width: 640
        height: 360
        uri: "rtsp://admin:19929394@192.168.1.72:10554/tcp/av0_0"
    }
        OpenGlVideoQtQuick {
        width: 640
        height: 360
        uri: "rtsp://admin:19929394@192.168.1.178:10554/tcp/av0_0"
    }
    OpenGlVideoQtQuick {
        width: 640
        height: 360
        uri: "rtsp://admin:19929394@192.168.1.155:10554/tcp/av0_0"
    }
    
    
}
