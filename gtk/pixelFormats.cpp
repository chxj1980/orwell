#include "pixelFormats.h"

pixelFormats[AV_PIX_FMT_YUV420P] = {
    {one,one,one},  //AVRational yuvSizes[3];
    {one,two,two},  //AVRational yuvWidths[3];
    {one,two,two},  //AVRational yuvHeights[3];
    {GL_LUMINANCE,GL_LUMINANCE,GL_LUMINANCE}, //GLint yuvInternalFormat[3];
    {GL_LUMINANCE,GL_LUMINANCE,GL_LUMINANCE}, //GLenum yuvGlFormat[3];
    textureFormat=YUV,
    isPlanar=true,
    dataType=GL_UNSIGNED_BYTE
}
pixelFormats[AV_PIX_FMT_YUVJ420P] = {
    {one,one,one},  //AVRational yuvSizes[3];
    {one,two,two},  //AVRational yuvWidths[3];
    {one,two,two},  //AVRational yuvHeights[3];
    {GL_LUMINANCE,GL_LUMINANCE,GL_LUMINANCE}, //GLint yuvInternalFormat[3];
    {GL_LUMINANCE,GL_LUMINANCE,GL_LUMINANCE}, //GLenum yuvGlFormat[3];
    textureFormat=YUVJ,
    isPlanar=true,
    dataType=GL_UNSIGNED_BYTE
}
pixelFormats[AV_PIX_FMT_YUV422P] = {
    {one,one,one},  //AVRational yuvSizes[3];
    {one,one,one},  //AVRational yuvWidths[3];
    {one,one,one},  //AVRational yuvHeights[3];
    {GL_LUMINANCE,GL_LUMINANCE,GL_LUMINANCE}, //GLint yuvInternalFormat[3];
    {GL_LUMINANCE,GL_LUMINANCE,GL_LUMINANCE}, //GLenum yuvGlFormat[3];
    textureFormat=YUV,
    isPlanar=true,
    dataType=GL_UNSIGNED_BYTE
}
pixelFormats[AV_PIX_FMT_YUVJ422P] = {
    {one,one,one},  //AVRational yuvSizes[3];
    {one,one,one},  //AVRational yuvWidths[3];
    {one,one,one},  //AVRational yuvHeights[3];
    {GL_LUMINANCE,GL_LUMINANCE,GL_LUMINANCE}, //GLint yuvInternalFormat[3];
    {GL_LUMINANCE,GL_LUMINANCE,GL_LUMINANCE}, //GLenum yuvGlFormat[3];
    textureFormat=YUVJ,
    isPlanar=true,
    dataType=GL_UNSIGNED_BYTE
}
pixelFormats[AV_PIX_FMT_YUV444P] = {
    {one,one,one},  //AVRational yuvSizes[3];
    {one,one,one},  //AVRational yuvWidths[3];
    {one,one,one},  //AVRational yuvHeights[3];
    {GL_LUMINANCE,GL_LUMINANCE,GL_LUMINANCE}, //GLint yuvInternalFormat[3];
    {GL_LUMINANCE,GL_LUMINANCE,GL_LUMINANCE}, //GLenum yuvGlFormat[3];
    textureFormat=YUV,
    isPlanar=true,
    dataType=GL_UNSIGNED_BYTE
}
pixelFormats[AV_PIX_FMT_YUVJ444P] = {
    {one,one,one},  //AVRational yuvSizes[3];
    {one,one,one},  //AVRational yuvWidths[3];
    {one,one,one},  //AVRational yuvHeights[3];
    {GL_LUMINANCE,GL_LUMINANCE,GL_LUMINANCE}, //GLint yuvInternalFormat[3];
    {GL_LUMINANCE,GL_LUMINANCE,GL_LUMINANCE}, //GLenum yuvGlFormat[3];
    textureFormat=YUVJ,
    isPlanar=true,
    dataType=GL_UNSIGNED_BYTE
}
pixelFormats[AV_PIX_FMT_GRAY8] = {
    {one,one,one},  //AVRational yuvSizes[3];
    {one,one,one},  //AVRational yuvWidths[3];
    {one,one,one},  //AVRational yuvHeights[3];
    {GL_LUMINANCE,GL_LUMINANCE,GL_LUMINANCE}, //GLint yuvInternalFormat[3];
    {GL_LUMINANCE,GL_LUMINANCE,GL_LUMINANCE}, //GLenum yuvGlFormat[3];
    textureFormat=GRAY,
    isPlanar=true,
    dataType=GL_UNSIGNED_BYTE
}
pixelFormats[AV_PIX_FMT_UYVY422] = {
    {four,zero,zero},  //AVRational yuvSizes[3];
    {one,zero,zero},  //AVRational yuvWidths[3];
    {one,zero,zero},  //AVRational yuvHeights[3];
    {GL_RGBA,GL_RGBA,GL_RGBA}, //GLint yuvInternalFormat[3];
    {GL_RGBA,GL_RGBA,GL_RGBA}, //GLenum yuvGlFormat[3];
    textureFormat=UYVY,
    isPlanar=false,
    dataType=GL_UNSIGNED_BYTE
}
pixelFormats[AV_PIX_FMT_YUYV422] = {
    {four,zero,zero},  //AVRational yuvSizes[3];
    {one,zero,zero},  //AVRational yuvWidths[3];
    {one,zero,zero},  //AVRational yuvHeights[3];
    {GL_RGBA,GL_RGBA,GL_RGBA}, //GLint yuvInternalFormat[3];
    {GL_RGBA,GL_RGBA,GL_RGBA}, //GLenum yuvGlFormat[3];
    textureFormat=YUYV,
    isPlanar=false,
    dataType=GL_UNSIGNED_BYTE
}
pixelFormats[AV_PIX_FMT_BGR24] = {
    {three,one,one},  //AVRational yuvSizes[3];
    {one,one,one},  //AVRational yuvWidths[3];
    {one,one,one},  //AVRational yuvHeights[3];
    {GL_RGB8,GL_RGB8,GL_RGB8}, //GLint yuvInternalFormat[3];
    {GL_BGR,GL_BGR,GL_BGR}, //GLenum yuvGlFormat[3];
    textureFormat=BGR,
    isPlanar=true,
    dataType=GL_UNSIGNED_BYTE
}
pixelFormats[AV_PIX_FMT_RGB24] = {
    {three,one,one},  //AVRational yuvSizes[3];
    {one,one,one},  //AVRational yuvWidths[3];
    {one,one,one},  //AVRational yuvHeights[3];
    {GL_RGB8,GL_RGB8,GL_RGB8}, //GLint yuvInternalFormat[3];
    {GL_RGB,GL_RGB,GL_RGB}, //GLenum yuvGlFormat[3];
    textureFormat=RGB,
    isPlanar=true,
    dataType=GL_UNSIGNED_BYTE
}
pixelFormats[AV_PIX_FMT_MONOWHITE] = {
    {one,one,one},  //AVRational yuvSizes[3];
    {one,one,one},  //AVRational yuvWidths[3];
    {one,one,one},  //AVRational yuvHeights[3];
    {GL_RGB8,GL_RGB8,GL_RGB8}, //GLint yuvInternalFormat[3];
    {GL_LUMINANCE,GL_LUMINANCE,GL_LUMINANCE}, //GLenum yuvGlFormat[3];
    textureFormat=RGB,
    isPlanar=true,
    dataType=GL_UNSIGNED_BYTE
}
pixelFormats[AV_PIX_FMT_MONOBLACK] = {
    {one,one,one},  //AVRational yuvSizes[3];
    {one,one,one},  //AVRational yuvWidths[3];
    {one,one,one},  //AVRational yuvHeights[3];
    {GL_RGB8,GL_RGB8,GL_RGB8}, //GLint yuvInternalFormat[3];
    {GL_LUMINANCE,GL_LUMINANCE,GL_LUMINANCE}, //GLenum yuvGlFormat[3];
    textureFormat=RGB,
    isPlanar=true,
    dataType=GL_UNSIGNED_BYTE
}
pixelFormats[AV_PIX_FMT_PAL8] = {
    {one,one,one},  //AVRational yuvSizes[3];
    {one,one,one},  //AVRational yuvWidths[3];
    {one,two,two},  //AVRational yuvHeights[3];
    {GL_LUMINANCE,GL_LUMINANCE,GL_LUMINANCE}, //GLint yuvInternalFormat[3];
    {GL_LUMINANCE,GL_LUMINANCE,GL_LUMINANCE}, //GLenum yuvGlFormat[3];
    textureFormat=RGB,
    isPlanar=true,
    dataType=GL_UNSIGNED_BYTE
}
pixelFormats[AV_PIX_FMT_BGR8] = {
    {one,zero,zero},  //AVRational yuvSizes[3];
    {one,zero,zero},  //AVRational yuvWidths[3];
    {one,zero,zero},  //AVRational yuvHeights[3];
    {GL_RGB,GL_RGB,GL_RGB}, //GLint yuvInternalFormat[3];
    {GL_RGB,GL_RGB,GL_RGB}, //GLenum yuvGlFormat[3];
    textureFormat=BGR8,
    isPlanar=false,
    dataType=GL_UNSIGNED_BYTE_2_3_3_REV
}
pixelFormats[AV_PIX_FMT_RGB8] = {
    {one,zero,zero},  //AVRational yuvSizes[3];
    {one,zero,zero},  //AVRational yuvWidths[3];
    {one,zero,zero},  //AVRational yuvHeights[3];
    {GL_RGB,GL_RGB,GL_RGB}, //GLint yuvInternalFormat[3];
    {GL_RGB,GL_RGB,GL_RGB}, //GLenum yuvGlFormat[3];
    textureFormat=BGR8,
    isPlanar=false,
    dataType=GL_UNSIGNED_BYTE_3_3_2
}
pixelFormats[AV_PIX_FMT_BGR4] = {
    {one,zero,zero},  //AVRational yuvSizes[3];
    {one,zero,zero},  //AVRational yuvWidths[3];
    {one,zero,zero},  //AVRational yuvHeights[3];
    {GL_LUMINANCE4,GL_LUMINANCE4,GL_LUMINANCE4}, //GLint yuvInternalFormat[3];
    {GL_LUMINANCE,GL_LUMINANCE,GL_LUMINANCE}, //GLenum yuvGlFormat[3];
    textureFormat=BGR4,
    isPlanar=false,
    dataType=GL_UNSIGNED_BYTE
}
pixelFormats[AV_PIX_FMT_NV12] = {
    {one,two,zero},  //AVRational yuvSizes[3];
    {one,two,zero},  //AVRational yuvWidths[3];
    {one,two,zero},  //AVRational yuvHeights[3];
    {GL_LUMINANCE,GL_LUMINANCE_ALPHA,GL_LUMINANCE}, //GLint yuvInternalFormat[3];
    {GL_LUMINANCE,GL_LUMINANCE_ALPHA,GL_LUMINANCE}, //GLenum yuvGlFormat[3];
    textureFormat=NV12,
    isPlanar=true,
    dataType=GL_UNSIGNED_BYTE
}
pixelFormats[AV_PIX_FMT_NV21] = {
    {one,two,zero},  //AVRational yuvSizes[3];
    {one,two,zero},  //AVRational yuvWidths[3];
    {one,two,zero},  //AVRational yuvHeights[3];
    {GL_LUMINANCE,GL_LUMINANCE_ALPHA,GL_LUMINANCE}, //GLint yuvInternalFormat[3];
    {GL_LUMINANCE,GL_LUMINANCE_ALPHA,GL_LUMINANCE}, //GLenum yuvGlFormat[3];
    textureFormat=NV21,
    isPlanar=true,
    dataType=GL_UNSIGNED_BYTE
}
pixelFormats[AV_PIX_FMT_YUV420P10LE] = {
    {two,two,two},  //AVRational yuvSizes[3];
    {one,two,two},  //AVRational yuvWidths[3];
    {one,two,two},  //AVRational yuvHeights[3];
    {GL_LUMINANCE_ALPHA,GL_LUMINANCE_ALPHA,GL_LUMINANCE_ALPHA}, //GLint yuvInternalFormat[3];
    {GL_LUMINANCE_ALPHA,GL_LUMINANCE_ALPHA,GL_LUMINANCE_ALPHA}, //GLenum yuvGlFormat[3];
    textureFormat=YUV420P10LE,
    isPlanar=true,
    dataType=GL_UNSIGNED_BYTE
}
pixelFormats[AV_PIX_FMT_YUV422P10LE] = {
    {two,two,two},  //AVRational yuvSizes[3];
    {one,one,one},  //AVRational yuvWidths[3];
    {one,one,one},  //AVRational yuvHeights[3];
    {GL_LUMINANCE_ALPHA,GL_LUMINANCE_ALPHA,GL_LUMINANCE_ALPHA}, //GLint yuvInternalFormat[3];
    {GL_LUMINANCE_ALPHA,GL_LUMINANCE_ALPHA,GL_LUMINANCE_ALPHA}, //GLenum yuvGlFormat[3];
    textureFormat=YUV420P10LE,
    isPlanar=true,
    dataType=GL_UNSIGNED_BYTE
}
pixelFormats[AV_PIX_FMT_YUV444P10LE] = {
    {two,two,two},  //AVRational yuvSizes[3];
    {one,one,one},  //AVRational yuvWidths[3];
    {one,one,one},  //AVRational yuvHeights[3];
    {GL_LUMINANCE_ALPHA,GL_LUMINANCE_ALPHA,GL_LUMINANCE_ALPHA}, //GLint yuvInternalFormat[3];
    {GL_LUMINANCE_ALPHA,GL_LUMINANCE_ALPHA,GL_LUMINANCE_ALPHA}, //GLenum yuvGlFormat[3];
    textureFormat=YUV420P10LE,
    isPlanar=true,
    dataType=GL_UNSIGNED_BYTE
}
pixelFormats[AV_PIX_FMT_YUVA420P10LE] = {
    {two,two,two},  //AVRational yuvSizes[3];
    {one,two,two},  //AVRational yuvWidths[3];
    {one,two,two},  //AVRational yuvHeights[3];
    {GL_LUMINANCE_ALPHA,GL_LUMINANCE_ALPHA,GL_LUMINANCE_ALPHA}, //GLint yuvInternalFormat[3];
    {GL_LUMINANCE_ALPHA,GL_LUMINANCE_ALPHA,GL_LUMINANCE_ALPHA}, //GLenum yuvGlFormat[3];
    textureFormat=YUV420P10LE,
    isPlanar=true,
    dataType=GL_UNSIGNED_BYTE
}
pixelFormats[AV_PIX_FMT_YUVA422P10LE] = {
    {two,two,two},  //AVRational yuvSizes[3];
    {one,one,one},  //AVRational yuvWidths[3];
    {one,one,one},  //AVRational yuvHeights[3];
    {GL_LUMINANCE_ALPHA,GL_LUMINANCE_ALPHA,GL_LUMINANCE_ALPHA}, //GLint yuvInternalFormat[3];
    {GL_LUMINANCE_ALPHA,GL_LUMINANCE_ALPHA,GL_LUMINANCE_ALPHA}, //GLenum yuvGlFormat[3];
    textureFormat=YUV420P10LE,
    isPlanar=true,
    dataType=GL_UNSIGNED_BYTE
}
pixelFormats[AV_PIX_FMT_YUVA444P10LE] = {
    {two,two,two},  //AVRational yuvSizes[3];
    {one,one,one},  //AVRational yuvWidths[3];
    {one,one,one},  //AVRational yuvHeights[3];
    {GL_LUMINANCE_ALPHA,GL_LUMINANCE_ALPHA,GL_LUMINANCE_ALPHA}, //GLint yuvInternalFormat[3];
    {GL_LUMINANCE_ALPHA,GL_LUMINANCE_ALPHA,GL_LUMINANCE_ALPHA}, //GLenum yuvGlFormat[3];
    textureFormat=YUV420P10LE,
    isPlanar=true,
    dataType=GL_UNSIGNED_BYTE
}
pixelFormats[AV_PIX_FMT_YUV410P] = {
    {one,one,one},  //AVRational yuvSizes[3];
    {one,four,four},  //AVRational yuvWidths[3];
    {one,four,four},  //AVRational yuvHeights[3];
    {GL_LUMINANCE,GL_LUMINANCE,GL_LUMINANCE}, //GLint yuvInternalFormat[3];
    {GL_LUMINANCE,GL_LUMINANCE,GL_LUMINANCE}, //GLenum yuvGlFormat[3];
    textureFormat=YUV,
    isPlanar=true,
    dataType=GL_UNSIGNED_BYTE
}
pixelFormats[AV_PIX_FMT_YUV411P] = {
    {one,one,one},  //AVRational yuvSizes[3];
    {one,one,one},  //AVRational yuvWidths[3];
    {one,one,one},  //AVRational yuvHeights[3];
    {GL_LUMINANCE,GL_LUMINANCE,GL_LUMINANCE}, //GLint yuvInternalFormat[3];
    {GL_LUMINANCE,GL_LUMINANCE,GL_LUMINANCE}, //GLenum yuvGlFormat[3];
    textureFormat=YUV,
    isPlanar=true,
    dataType=GL_UNSIGNED_BYTE
}
pixelFormats[AV_PIX_FMT_ARGB] = {
    {four,zero,zero},  //AVRational yuvSizes[3];
    {one,zero,zero},  //AVRational yuvWidths[3];
    {one,zero,zero},  //AVRational yuvHeights[3];
    {GL_RGBA,GL_RGBA,GL_RGBA}, //GLint yuvInternalFormat[3];
    {GL_RGBA,GL_RGBA,GL_RGBA}, //GLenum yuvGlFormat[3];
    textureFormat=ARGB,
    isPlanar=false,
    dataType=GL_UNSIGNED_BYTE
}
pixelFormats[AV_PIX_FMT_RGBA] = {
    {four,zero,zero},  //AVRational yuvSizes[3];
    {one,zero,zero},  //AVRational yuvWidths[3];
    {one,zero,zero},  //AVRational yuvHeights[3];
    {GL_RGBA,GL_RGBA,GL_RGBA}, //GLint yuvInternalFormat[3];
    {GL_RGBA,GL_RGBA,GL_RGBA}, //GLenum yuvGlFormat[3];
    textureFormat=RGBA,
    isPlanar=false,
    dataType=GL_UNSIGNED_BYTE
}
pixelFormats[AV_PIX_FMT_ABGR] = {
    {four,zero,zero},  //AVRational yuvSizes[3];
    {one,zero,zero},  //AVRational yuvWidths[3];
    {one,zero,zero},  //AVRational yuvHeights[3];
    {GL_RGBA,GL_RGBA,GL_RGBA}, //GLint yuvInternalFormat[3];
    {GL_RGBA,GL_RGBA,GL_RGBA}, //GLenum yuvGlFormat[3];
    textureFormat=ABGR,
    isPlanar=false,
    dataType=GL_UNSIGNED_BYTE
}
pixelFormats[AV_PIX_FMT_BGRA] = {
    {four,zero,zero},  //AVRational yuvSizes[3];
    {one,zero,zero},  //AVRational yuvWidths[3];
    {one,zero,zero},  //AVRational yuvHeights[3];
    {GL_RGBA,GL_RGBA,GL_RGBA}, //GLint yuvInternalFormat[3];
    {GL_RGBA,GL_RGBA,GL_RGBA}, //GLenum yuvGlFormat[3];
    textureFormat=BGRA,
    isPlanar=false,
    dataType=GL_UNSIGNED_BYTE
}
pixelFormats[AV_PIX_FMT_GRAY16BE] = {
    {two,zero,zero},  //AVRational yuvSizes[3];
    {one,zero,zero},  //AVRational yuvWidths[3];
    {one,zero,zero},  //AVRational yuvHeights[3];
    {GL_LUMINANCE16,GL_LUMINANCE16,GL_LUMINANCE16}, //GLint yuvInternalFormat[3];
    {GL_LUMINANCE,GL_LUMINANCE,GL_LUMINANCE}, //GLenum yuvGlFormat[3];
    textureFormat=GRAY,
    isPlanar=true,
    dataType=GL_UNSIGNED_SHORT
}
pixelFormats[AV_PIX_FMT_GRAY16LE] = {
    {two,zero,zero},  //AVRational yuvSizes[3];
    {one,zero,zero},  //AVRational yuvWidths[3];
    {one,zero,zero},  //AVRational yuvHeights[3];
    {GL_LUMINANCE16,GL_LUMINANCE16,GL_LUMINANCE16}, //GLint yuvInternalFormat[3];
    {GL_LUMINANCE,GL_LUMINANCE,GL_LUMINANCE}, //GLenum yuvGlFormat[3];
    textureFormat=GRAY,
    isPlanar=true,
    dataType=GL_UNSIGNED_SHORT
}
pixelFormats[AV_PIX_FMT_YUV440P] = {
    {one,one,one},  //AVRational yuvSizes[3];
    {one,two,two},  //AVRational yuvWidths[3];
    {one,one,two},  //AVRational yuvHeights[3];
    {GL_LUMINANCE,GL_LUMINANCE,GL_LUMINANCE}, //GLint yuvInternalFormat[3];
    {GL_LUMINANCE,GL_LUMINANCE,GL_LUMINANCE}, //GLenum yuvGlFormat[3];
    textureFormat=YUV,
    isPlanar=true,
    dataType=GL_UNSIGNED_BYTE
}
pixelFormats[AV_PIX_FMT_YUVJ440P] = {
    {one,one,one},  //AVRational yuvSizes[3];
    {one,two,two},  //AVRational yuvWidths[3];
    {one,one,two},  //AVRational yuvHeights[3];
    {GL_LUMINANCE,GL_LUMINANCE,GL_LUMINANCE}, //GLint yuvInternalFormat[3];
    {GL_LUMINANCE,GL_LUMINANCE,GL_LUMINANCE}, //GLenum yuvGlFormat[3];
    textureFormat=YUVJ,
    isPlanar=true,
    dataType=GL_UNSIGNED_BYTE
}
pixelFormats[AV_PIX_FMT_YUVA420P] = {
    {one,one,one},  //AVRational yuvSizes[3];
    {one,two,two},  //AVRational yuvWidths[3];
    {one,one,two},  //AVRational yuvHeights[3];
    {GL_LUMINANCE,GL_LUMINANCE,GL_LUMINANCE}, //GLint yuvInternalFormat[3];
    {GL_LUMINANCE,GL_LUMINANCE,GL_LUMINANCE}, //GLenum yuvGlFormat[3];
    textureFormat=YUV,
    isPlanar=true,
    dataType=GL_UNSIGNED_BYTE
}
pixelFormats[AV_PIX_FMT_UYYVYY411] = {
    {one,one,one},  //AVRational yuvSizes[3];
    {one,two,two},  //AVRational yuvWidths[3];
    {one,two,two},  //AVRational yuvHeights[3];
    {GL_LUMINANCE,GL_LUMINANCE,GL_LUMINANCE}, //GLint yuvInternalFormat[3];
    {GL_LUMINANCE,GL_LUMINANCE,GL_LUMINANCE}, //GLenum yuvGlFormat[3];
    textureFormat=UYYVYY,
    isPlanar=false,
    dataType=GL_UNSIGNED_BYTE
}
pixelFormats[AV_PIX_FMT_RGB444LE] = {
    {two,zero,zero},  //AVRational yuvSizes[3];
    {one,zero,zero},  //AVRational yuvWidths[3];
    {one,zero,zero},  //AVRational yuvHeights[3];
    {GL_RGBA,GL_RGBA,GL_RGBA}, //GLint yuvInternalFormat[3];
    {GL_RGBA,GL_RGBA,GL_RGBA}, //GLenum yuvGlFormat[3];
    textureFormat=RGB444,
    isPlanar=false,
    dataType=GL_UNSIGNED_SHORT_4_4_4_4
}
pixelFormats[AV_PIX_FMT_RGB444BE] = {
    {two,zero,zero},  //AVRational yuvSizes[3];
    {one,zero,zero},  //AVRational yuvWidths[3];
    {one,zero,zero},  //AVRational yuvHeights[3];
    {GL_RGBA,GL_RGBA,GL_RGBA}, //GLint yuvInternalFormat[3];
    {GL_RGBA,GL_RGBA,GL_RGBA}, //GLenum yuvGlFormat[3];
    textureFormat=RGB444,
    isPlanar=false,
    dataType=GL_UNSIGNED_SHORT_4_4_4_4
}
pixelFormats[AV_PIX_FMT_BGR444LE] = {
    {two,zero,zero},  //AVRational yuvSizes[3];
    {one,zero,zero},  //AVRational yuvWidths[3];
    {one,zero,zero},  //AVRational yuvHeights[3];
    {GL_RGBA,GL_RGBA,GL_RGBA}, //GLint yuvInternalFormat[3];
    {GL_RGBA,GL_RGBA,GL_RGBA}, //GLenum yuvGlFormat[3];
    textureFormat=BGR444,
    isPlanar=false,
    dataType=GL_UNSIGNED_SHORT_4_4_4_4
}
pixelFormats[AV_PIX_FMT_BGR444BE] = {
    {two,zero,zero},  //AVRational yuvSizes[3];
    {one,zero,zero},  //AVRational yuvWidths[3];
    {one,zero,zero},  //AVRational yuvHeights[3];
    {GL_RGBA,GL_RGBA,GL_RGBA}, //GLint yuvInternalFormat[3];
    {GL_RGBA,GL_RGBA,GL_RGBA}, //GLenum yuvGlFormat[3];
    textureFormat=BGR444,
    isPlanar=false,
    dataType=GL_UNSIGNED_SHORT_4_4_4_4
}
pixelFormats[AV_PIX_FMT_YUV420P16LE] = {
    {two,two,two},  //AVRational yuvSizes[3];
    {one,two,two},  //AVRational yuvWidths[3];
    {one,two,two},  //AVRational yuvHeights[3];
    {GL_LUMINANCE16,GL_LUMINANCE16,GL_LUMINANCE16}, //GLint yuvInternalFormat[3];
    {GL_LUMINANCE,GL_LUMINANCE,GL_LUMINANCE}, //GLenum yuvGlFormat[3];
    textureFormat=YUV16,
    isPlanar=true,
    dataType=GL_UNSIGNED_SHORT
}
pixelFormats[AV_PIX_FMT_YUV420P16BE] = {
    {two,two,two},  //AVRational yuvSizes[3];
    {one,two,two},  //AVRational yuvWidths[3];
    {one,two,two},  //AVRational yuvHeights[3];
    {GL_LUMINANCE16,GL_LUMINANCE16,GL_LUMINANCE16}, //GLint yuvInternalFormat[3];
    {GL_LUMINANCE,GL_LUMINANCE,GL_LUMINANCE}, //GLenum yuvGlFormat[3];
    textureFormat=YUV16,
    isPlanar=true,
    dataType=GL_UNSIGNED_SHORT
}
pixelFormats[AV_PIX_FMT_YUV422P16LE] = {
    {two,two,two},  //AVRational yuvSizes[3];
    {one,one,one},  //AVRational yuvWidths[3];
    {one,one,one},  //AVRational yuvHeights[3];
    {GL_LUMINANCE16,GL_LUMINANCE16,GL_LUMINANCE16}, //GLint yuvInternalFormat[3];
    {GL_LUMINANCE,GL_LUMINANCE,GL_LUMINANCE}, //GLenum yuvGlFormat[3];
    textureFormat=YUV16,
    isPlanar=true,
    dataType=GL_UNSIGNED_SHORT
}
pixelFormats[AV_PIX_FMT_YUV422P16BE] = {
    {two,two,two},  //AVRational yuvSizes[3];
    {one,one,one},  //AVRational yuvWidths[3];
    {one,one,one},  //AVRational yuvHeights[3];
    {GL_LUMINANCE16,GL_LUMINANCE16,GL_LUMINANCE16}, //GLint yuvInternalFormat[3];
    {GL_LUMINANCE,GL_LUMINANCE,GL_LUMINANCE}, //GLenum yuvGlFormat[3];
    textureFormat=YUV16,
    isPlanar=true,
    dataType=GL_UNSIGNED_SHORT
}
pixelFormats[AV_PIX_FMT_YUV444P16LE] = {
    {two,two,two},  //AVRational yuvSizes[3];
    {one,one,one},  //AVRational yuvWidths[3];
    {one,one,one},  //AVRational yuvHeights[3];
    {GL_LUMINANCE16,GL_LUMINANCE16,GL_LUMINANCE16}, //GLint yuvInternalFormat[3];
    {GL_LUMINANCE,GL_LUMINANCE,GL_LUMINANCE}, //GLenum yuvGlFormat[3];
    textureFormat=YUV16,
    isPlanar=true,
    dataType=GL_UNSIGNED_SHORT
}
pixelFormats[AV_PIX_FMT_YUV444P16BE] = {
    {two,two,two},  //AVRational yuvSizes[3];
    {one,one,one},  //AVRational yuvWidths[3];
    {one,one,one},  //AVRational yuvHeights[3];
    {GL_LUMINANCE16,GL_LUMINANCE16,GL_LUMINANCE16}, //GLint yuvInternalFormat[3];
    {GL_LUMINANCE,GL_LUMINANCE,GL_LUMINANCE}, //GLenum yuvGlFormat[3];
    textureFormat=YUV16,
    isPlanar=true,
    dataType=GL_UNSIGNED_SHORT
}
pixelFormats[AV_PIX_FMT_YUVA420P16LE] = {
    {two,two,two},  //AVRational yuvSizes[3];
    {one,two,two},  //AVRational yuvWidths[3];
    {one,one,one},  //AVRational yuvHeights[3];
    {GL_LUMINANCE16,GL_LUMINANCE16,GL_LUMINANCE16}, //GLint yuvInternalFormat[3];
    {GL_LUMINANCE,GL_LUMINANCE,GL_LUMINANCE}, //GLenum yuvGlFormat[3];
    textureFormat=YUV,
    isPlanar=true,
    dataType=GL_UNSIGNED_SHORT
}
pixelFormats[AV_PIX_FMT_YUVA422P16LE] = {
    {two,two,two},  //AVRational yuvSizes[3];
    {one,one,one},  //AVRational yuvWidths[3];
    {one,one,one},  //AVRational yuvHeights[3];
    {GL_LUMINANCE16,GL_LUMINANCE16,GL_LUMINANCE16}, //GLint yuvInternalFormat[3];
    {GL_LUMINANCE,GL_LUMINANCE,GL_LUMINANCE}, //GLenum yuvGlFormat[3];
    textureFormat=YUV,
    isPlanar=true,
    dataType=GL_UNSIGNED_SHORT
}
pixelFormats[AV_PIX_FMT_YUVA444P16LE] = {
    {two,two,two},  //AVRational yuvSizes[3];
    {one,one,one},  //AVRational yuvWidths[3];
    {one,one,one},  //AVRational yuvHeights[3];
    {GL_LUMINANCE16,GL_LUMINANCE16,GL_LUMINANCE16}, //GLint yuvInternalFormat[3];
    {GL_LUMINANCE,GL_LUMINANCE,GL_LUMINANCE}, //GLenum yuvGlFormat[3];
    textureFormat=YUV,
    isPlanar=true,
    dataType=GL_UNSIGNED_SHORT
}
pixelFormats[AV_PIX_FMT_BAYER_BGGR16LE] = {
    {two,one,one},  //AVRational yuvSizes[3];
    {one,one,one},  //AVRational yuvWidths[3];
    {one,two,two},  //AVRational yuvHeights[3];
    {GL_LUMINANCE16,GL_LUMINANCE16,GL_LUMINANCE16}, //GLint yuvInternalFormat[3];
    {GL_LUMINANCE,GL_LUMINANCE,GL_LUMINANCE}, //GLenum yuvGlFormat[3];
    textureFormat=BGGR,
    isPlanar=true,
    dataType=GL_UNSIGNED_SHORT
}
pixelFormats[AV_PIX_FMT_BAYER_RGGB16LE] = {
    {two,one,one},  //AVRational yuvSizes[3];
    {one,one,one},  //AVRational yuvWidths[3];
    {one,two,two},  //AVRational yuvHeights[3];
    {GL_LUMINANCE16,GL_LUMINANCE16,GL_LUMINANCE16}, //GLint yuvInternalFormat[3];
    {GL_LUMINANCE,GL_LUMINANCE,GL_LUMINANCE}, //GLenum yuvGlFormat[3];
    textureFormat=RGGB,
    isPlanar=true,
    dataType=GL_UNSIGNED_SHORT
}
pixelFormats[AV_PIX_FMT_BAYER_GBRG16LE] = {
    {two,one,one},  //AVRational yuvSizes[3];
    {one,one,one},  //AVRational yuvWidths[3];
    {one,two,two},  //AVRational yuvHeights[3];
    {GL_LUMINANCE16,GL_LUMINANCE16,GL_LUMINANCE16}, //GLint yuvInternalFormat[3];
    {GL_LUMINANCE,GL_LUMINANCE,GL_LUMINANCE}, //GLenum yuvGlFormat[3];
    textureFormat=GBRG,
    isPlanar=true,
    dataType=GL_UNSIGNED_SHORT
}
pixelFormats[AV_PIX_FMT_BAYER_GRBG16LE] = {
    {two,one,one},  //AVRational yuvSizes[3];
    {one,one,one},  //AVRational yuvWidths[3];
    {one,two,two},  //AVRational yuvHeights[3];
    {GL_LUMINANCE16,GL_LUMINANCE16,GL_LUMINANCE16}, //GLint yuvInternalFormat[3];
    {GL_LUMINANCE,GL_LUMINANCE,GL_LUMINANCE}, //GLenum yuvGlFormat[3];
    textureFormat=GRBG,
    isPlanar=true,
    dataType=GL_UNSIGNED_SHORT
}
pixelFormats[AV_PIX_FMT_BAYER_GRBG16LE] = {
    {two,one,one},  //AVRational yuvSizes[3];
    {one,one,one},  //AVRational yuvWidths[3];
    {one,two,two},  //AVRational yuvHeights[3];
    {GL_LUMINANCE16,GL_LUMINANCE16,GL_LUMINANCE16}, //GLint yuvInternalFormat[3];
    {GL_LUMINANCE,GL_LUMINANCE,GL_LUMINANCE}, //GLenum yuvGlFormat[3];
    textureFormat=GRBG,
    isPlanar=true,
    dataType=GL_UNSIGNED_SHORT
}
pixelFormats[AV_PIX_FMT_RGB565LE] = {
    {two,zero,zero},  //AVRational yuvSizes[3];
    {one,zero,zero},  //AVRational yuvWidths[3];
    {one,zero,zero},  //AVRational yuvHeights[3];
    {GL_RGB,GL_RGB,GL_RGB}, //GLint yuvInternalFormat[3];
    {GL_RGB,GL_RGB,GL_RGB}, //GLenum yuvGlFormat[3];
    textureFormat=RGB,
    isPlanar=true,
    dataType=GL_UNSIGNED_SHORT_5_6_5
}
pixelFormats[AV_PIX_FMT_RGB555LE] = {
    {two,zero,zero},  //AVRational yuvSizes[3];
    {one,zero,zero},  //AVRational yuvWidths[3];
    {one,zero,zero},  //AVRational yuvHeights[3];
    {GL_BGRA,GL_BGRA,GL_BGRA}, //GLint yuvInternalFormat[3];
    {GL_BGRA,GL_BGRA,GL_BGRA}, //GLenum yuvGlFormat[3];
    textureFormat=RGB,
    isPlanar=true,
    dataType=GL_UNSIGNED_SHORT_1_5_5_5_REV
}
pixelFormats[AV_PIX_FMT_BGR565LE] = {
    {two,zero,zero},  //AVRational yuvSizes[3];
    {one,zero,zero},  //AVRational yuvWidths[3];
    {one,zero,zero},  //AVRational yuvHeights[3];
    {GL_RGB,GL_RGB,GL_RGB}, //GLint yuvInternalFormat[3];
    {GL_RGB,GL_RGB,GL_RGB}, //GLenum yuvGlFormat[3];
    textureFormat=BGR565,
    isPlanar=true,
    dataType=GL_UNSIGNED_SHORT_5_6_5
}
pixelFormats[AV_PIX_FMT_RGB555LE] = {
    {two,zero,zero},  //AVRational yuvSizes[3];
    {one,zero,zero},  //AVRational yuvWidths[3];
    {one,zero,zero},  //AVRational yuvHeights[3];
    {GL_BGRA,GL_BGRA,GL_BGRA}, //GLint yuvInternalFormat[3];
    {GL_BGRA,GL_BGRA,GL_BGRA}, //GLenum yuvGlFormat[3];
    textureFormat=BGR565,
    isPlanar=true,
    dataType=GL_UNSIGNED_SHORT_1_5_5_5_REV
}
pixelFormats[AV_PIX_FMT_RGB48LE] = {
    {two,zero,zero},  //AVRational yuvSizes[3];
    {one,zero,zero},  //AVRational yuvWidths[3];
    {one,zero,zero},  //AVRational yuvHeights[3];
    {GL_LUMINANCE16,GL_LUMINANCE16,GL_LUMINANCE16}, //GLint yuvInternalFormat[3];
    {GL_LUMINANCE,GL_LUMINANCE,GL_LUMINANCE}, //GLenum yuvGlFormat[3];
    textureFormat=RGB,
    isPlanar=true,
    dataType=GL_UNSIGNED_SHORT
}
pixelFormats[AV_PIX_FMT_BGR48LE] = {
    {two,zero,zero},  //AVRational yuvSizes[3];
    {one,zero,zero},  //AVRational yuvWidths[3];
    {one,zero,zero},  //AVRational yuvHeights[3];
    {GL_LUMINANCE16,GL_LUMINANCE16,GL_LUMINANCE16}, //GLint yuvInternalFormat[3];
    {GL_LUMINANCE,GL_LUMINANCE,GL_LUMINANCE}, //GLenum yuvGlFormat[3];
    textureFormat=BGR,
    isPlanar=true,
    dataType=GL_UNSIGNED_SHORT
}
pixelFormats[AV_PIX_FMT_RGBA64LE] = {
    {two,zero,zero},  //AVRational yuvSizes[3];
    {one,zero,zero},  //AVRational yuvWidths[3];
    {one,zero,zero},  //AVRational yuvHeights[3];
    {GL_LUMINANCE_ALPHA,GL_LUMINANCE_ALPHA,GL_LUMINANCE_ALPHA}, //GLint yuvInternalFormat[3];
    {GL_LUMINANCE,GL_LUMINANCE,GL_LUMINANCE}, //GLenum yuvGlFormat[3];
    textureFormat=RGB,
    isPlanar=true,
    dataType=GL_UNSIGNED_SHORT
}
pixelFormats[AV_PIX_FMT_BGRA64LE] = {
    {two,zero,zero},  //AVRational yuvSizes[3];
    {one,zero,zero},  //AVRational yuvWidths[3];
    {one,zero,zero},  //AVRational yuvHeights[3];
    {GL_LUMINANCE_ALPHA,GL_LUMINANCE_ALPHA,GL_LUMINANCE_ALPHA}, //GLint yuvInternalFormat[3];
    {GL_LUMINANCE,GL_LUMINANCE,GL_LUMINANCE}, //GLenum yuvGlFormat[3];
    textureFormat=BGR,
    isPlanar=true,
    dataType=GL_UNSIGNED_SHORT
}
pixelFormats[AV_PIX_FMT_NV16] = {
    {one,one,one},  //AVRational yuvSizes[3];
    {one,two,two},  //AVRational yuvWidths[3];
    {one,one,one},  //AVRational yuvHeights[3];
    {GL_LUMINANCE,GL_LUMINANCE,GL_LUMINANCE}, //GLint yuvInternalFormat[3];
    {GL_LUMINANCE,GL_LUMINANCE,GL_LUMINANCE}, //GLenum yuvGlFormat[3];
    textureFormat=YUV,
    isPlanar=true,
    dataType=GL_UNSIGNED_BYTE
}
pixelFormats[AV_PIX_FMT_NV20LE] = {
    {one,one,one},  //AVRational yuvSizes[3];
    {one,two,two},  //AVRational yuvWidths[3];
    {one,two,two},  //AVRational yuvHeights[3];
    {GL_LUMINANCE,GL_LUMINANCE,GL_LUMINANCE}, //GLint yuvInternalFormat[3];
    {GL_LUMINANCE,GL_LUMINANCE,GL_LUMINANCE}, //GLenum yuvGlFormat[3];
    textureFormat=YUV,
    isPlanar=true,
    dataType=GL_UNSIGNED_BYTE
}
pixelFormats[AV_PIX_FMT_DXVA2_VLD] = {
    {one,two,zero},  //AVRational yuvSizes[3];
    {one,two,zero},  //AVRational yuvWidths[3];
    {one,two,zero},  //AVRational yuvHeights[3];
    {GL_LUMINANCE,GL_LUMINANCE_ALPHA,GL_LUMINANCE}, //GLint yuvInternalFormat[3];
    {GL_LUMINANCE,GL_LUMINANCE_ALPHA,GL_LUMINANCE}, //GLenum yuvGlFormat[3];
    textureFormat=NV12,
    isPlanar=true,
    dataType=GL_UNSIGNED_BYTE
}
