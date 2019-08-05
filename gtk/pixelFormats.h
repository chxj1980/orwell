#ifndef PIXEL_FORMATS_H
#define PIXEL_FORMATS_H

#include <libavutil/pixfmt.h>
#include <vector>

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
    Fraction yuvSizes[3]; //Cast to AVRational if needed, but I don't think I will
    Fraction yuvWidths[3];
    Fraction yuvHeights[3];
    int yuvInternalFormat[3];
    int yuvGlFormat[3]; //cast to GLenum
    bool isPlanar;
    int dataType; //cast to GLenum
};

std::vector<PixelFormat> pixelFormats;
#endif //PIXEL_FORMATS_H