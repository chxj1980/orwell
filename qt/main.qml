import QtQuick 2.0
import OpenGlVideoQtQuick 1.0
import OpenGlVideoQtQuick2 1.0
import OpenGlBufferQtQuick 1.0
import OpenGlMaterialQQuickItem 1.0

Grid {
    id: gridLayout
    columns: 2
    rows:2
    spacing: 0
    width: 1280
    height: 720
    
    property var cellWidth: gridLayout.width/gridLayout.columns;
    property var cellHeight: (gridLayout.height/gridLayout.rows);
    
    OpenGlBufferQtQuick {
        width: cellWidth
        height: cellHeight
        id: "cam1"
        //glue: Glue
        uri: "rtsp://admin:19929394@192.168.0.102:10554/tcp/av0_1"
    }
    /*
    OpenGlMaterialQQuickItem {
        width: cellWidth
        height: cellHeight
        uri: "rtsp://admin:19929394@192.168.0.101:10554/tcp/av0_1"
    }
    OpenGlMaterialQQuickItem {
        width: cellWidth
        height: cellHeight
        uri: "rtsp://admin:19929394@192.168.0.104:10554/tcp/av0_1"
    }
    OpenGlMaterialQQuickItem {
        width: cellWidth
        height: cellHeight
        uri: "rtsp://admin:19929394@192.168.0.103:10554/tcp/av0_1"
    }
    OpenGlMaterialQQuickItem {
        width: cellWidth
        height: cellHeight
        uri: "rtsp://admin:19929394@192.168.0.106:10554/tcp/av0_1"
    }
    OpenGlMaterialQQuickItem {
        width: cellWidth
        height: cellHeight
        uri: "rtsp://admin:19929394@192.168.0.107:10554/tcp/av0_1"
    }
    OpenGlMaterialQQuickItem {
        width: cellWidth
        height: cellHeight
        uri: "rtsp://admin:19929394@192.168.0.100:10554/tcp/av0_1"
    }
    */
    
}



/*
    OpenGlBufferQtQuick {
        width: cellWidth
        height: cellHeight
        uri: "rtsp://admin:19929394@192.168.1.178:10554/tcp/av0_0"
    }
    */
/*
    OpenGlVideoQtQuick {
        width: cellWidth
        height: cellHeight
        uri: "rtsp://admin:19929394@192.168.0.103:10554/tcp/av0_0"
    }
    OpenGlVideoQtQuick {
        width: cellWidth
        height: cellHeight
        uri: "rtsp://admin:19929394@192.168.0.104:10554/tcp/av0_0"
    }
    OpenGlVideoQtQuick {
        width: cellWidth
        height: cellHeight
        uri: "rtsp://admin:19929394@192.168.0.106:10554/tcp/av0_0"
    }
*/
/*
    OpenGlVideoQtQuick {
        width: cellWidth
        height: cellHeight
        uri: "rtsp://admin:19929394@192.168.1.198:10554/tcp/av0_0"
    }
    OpenGlVideoQtQuick {
        width: cellWidth
        height: cellHeight
        uri: "rtsp://admin:19929394@192.168.1.72:10554/tcp/av0_0"
    }
    OpenGlVideoQtQuick {
        width: cellWidth
        height: cellHeight
        uri: "rtsp://admin:19929394@192.168.1.178:10554/tcp/av0_0"
    }
    OpenGlVideoQtQuick {
        width: cellWidth
        height: cellHeight
        uri: "rtsp://admin:19929394@192.168.1.155:10554/tcp/av0_0"
    }
 */   
 /*
    OpenGlMaterialQQuickItem {
        width: cellWidth
        height: cellHeight
        uri: "rtsp://admin:19929394@192.168.1.178:10554/tcp/av0_0"
    }
    OpenGlMaterialQQuickItem {
        width: cellWidth
        height: cellHeight
        uri: "rtsp://admin:19929394@192.168.1.198:10554/tcp/av0_0"
    }
    OpenGlMaterialQQuickItem {
        width: cellWidth
        height: cellHeight
        uri: "rtsp://admin:19929394@192.168.1.72:10554/tcp/av0_0"
    }
    OpenGlMaterialQQuickItem {
        width: cellWidth
        height: cellHeight
        uri: "rtsp://admin:19929394@192.168.1.155:10554/tcp/av0_0"
    }
    */