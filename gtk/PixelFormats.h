#ifndef PIXEL_FORMATS_H
#define PIXEL_FORMATS_H

//TODO: include both pixftm.h and glx.h myself (only the structs needed here)
#include <libavutil/pixfmt.h>
#include <GL/glx.h>

enum TextureFormat{
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

Fraction zero  = {0,1};
Fraction one   = {1,1};
Fraction two   = {1,2};
Fraction three = {1,3};
Fraction four  = {1,4};

struct PixelFormat{
    Fraction yuvSizes[3];
    Fraction yuvWidths[3];
    Fraction yuvHeights[3];
    int yuvInternalFormat[3];
    GLenum yuvGlFormat[3]; //int or GLenum
    int textureFormat;
    bool isPlanar;
    GLenum dataType; //int ot GLenum
};

/*
    Let's put 100, don't know how many and don't want to auto generate because
    I also want to edit that file by hand
*/
class PixelFormats{
    public:
        static PixelFormat pixelFormats[100];
        PixelFormats();
        PixelFormat* get(int pixelFormat) {
            return &(this->pixelFormats[pixelFormat]);
        }

};
#endif //PIXEL_FORMATS_H