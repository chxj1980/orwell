#ifndef PIXEL_FORMATS_H
#define PIXEL_FORMATS_H

//TODO: include both pixftm.h and glx.h myself (only the structs needed here)
#include <libavutil/pixfmt.h>
#include <GL/glx.h>

/* 
    These types are used in the shader. There's an if section that deals with
    each one of them.
*/
enum TextureFormat {
    YUV = 0,
    YUVJ = 1,
    RGB = 2,
    GRAY = 3,
    YUYV = 4,
    UYVY = 5 ,
    BGR = 6,
    UYYVYY = 7,
    NV12 = 8 ,
    NV21 = 9 ,
    YUV420P10LE = 10,
    BGR8 = 11,
    RGBA = 12 ,
    ARGB = 13 ,
    ABGR = 14 ,
    BGRA = 15 ,
    YUV16 = 16,
    YUVJ16 = 17,
    BGR4 = 18,
    BGGR = 19,
    RGGB = 20 ,
    GRBG = 21 ,
    GBRG = 22 ,
    BGR565 = 23 ,
    RGB444 = 24,
    BGR444 = 25,
};

struct Fraction {
    int numerator;
    int denominator;
};

struct PixelFormat{
    Fraction yuvSizes[3];
    Fraction yuvWidths[3];
    Fraction yuvHeights[3];
    int yuvInternalFormat[3];
    GLenum yuvGlFormat[3]; //int or GLenum?
    int textureFormat;
    bool isPlanar;
    GLenum dataType; //int ot GLenum?
};

/*
    Just a class to initiate all the objects when constructed.
    Single instance of it to be stored on `PixelFormats` class.
    Let's put 100, don't know how many and don't want to auto generate because
    I also want to edit that file by hand for each new addition.
*/
class StaticPixelFormats {
    public:
        //TODO: change it to a container format that supports searching
        PixelFormat pixelFormats[100];
        StaticPixelFormats();
};
class PixelFormats {
    public:
        static StaticPixelFormats staticPixelFormats;
        static PixelFormat* get(int format) {
            return &staticPixelFormats.pixelFormats[format];
        }
};
#endif //PIXEL_FORMATS_H