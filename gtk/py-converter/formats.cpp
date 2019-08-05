params[AV_PIX_FMT_YUV420P] = RenderParams(AV_PIX_FMT_YUV420P,one,one,one,one,two,two,one,two,two,GL_LUMINANCE,GL_LUMINANCE,YUV,true);
params[AV_PIX_FMT_YUVJ420P] = RenderParams(AV_PIX_FMT_YUVJ420P,one,one,one,one,two,two,one,two,two,GL_LUMINANCE,GL_LUMINANCE,YUVJ,true);
params[AV_PIX_FMT_YUV422P] = RenderParams(AV_PIX_FMT_YUV422P,one,one,one,one,one,one,one,one,one,GL_LUMINANCE,GL_LUMINANCE,YUV,true);
params[AV_PIX_FMT_YUVJ422P] = RenderParams(AV_PIX_FMT_YUVJ422P,one,one,one,one,one,one,one,one,one,GL_LUMINANCE,GL_LUMINANCE,YUVJ,true);
params[AV_PIX_FMT_YUV444P] = RenderParams(AV_PIX_FMT_YUV444P,one,one,one,one,one,one,one,one,one,GL_LUMINANCE,GL_LUMINANCE,YUV,true);
params[AV_PIX_FMT_YUVJ444P] = RenderParams(AV_PIX_FMT_YUVJ444P,one,one,one,one,one,one,one,one,one,GL_LUMINANCE,GL_LUMINANCE,YUVJ,true);
params[AV_PIX_FMT_GRAY8] = RenderParams(AV_PIX_FMT_GRAY8,one,one,one,one,one,one,one,one,one,GL_LUMINANCE,GL_LUMINANCE,GRAY,true);
params[AV_PIX_FMT_UYVY422] = RenderParams(AV_PIX_FMT_UYVY422,four,zero,zero,one,zero,zero,one,zero,zero,GL_RGBA,GL_RGBA,UYVY,false);
params[AV_PIX_FMT_YUYV422] = RenderParams(AV_PIX_FMT_YUYV422,four,zero,zero,one,zero,zero,one,zero,zero,GL_RGBA,GL_RGBA,YUYV,false);
params[AV_PIX_FMT_BGR24] = RenderParams(AV_PIX_FMT_BGR24,three,one,one,one,one,one,one,one,one,GL_RGB8,GL_BGR,BGR,true);
params[AV_PIX_FMT_RGB24] = RenderParams(AV_PIX_FMT_RGB24,three,one,one,one,one,one,one,one,one,GL_RGB8,GL_RGB,RGB,true);
params[AV_PIX_FMT_MONOWHITE] = RenderParams(AV_PIX_FMT_MONOWHITE,one,one,one,one,one,one,one,one,one,GL_RGB8,GL_LUMINANCE,RGB,true);
params[AV_PIX_FMT_MONOBLACK] = RenderParams(AV_PIX_FMT_MONOBLACK,one,one,one,one,one,one,one,one,one,GL_RGB8,GL_LUMINANCE,RGB,true);
params[AV_PIX_FMT_PAL8] = RenderParams(AV_PIX_FMT_PAL8,one,one,one,one,one,one,one,two,two,GL_LUMINANCE,GL_LUMINANCE,RGB,true);
params[AV_PIX_FMT_BGR8] = RenderParams(AV_PIX_FMT_BGR8,one,zero,zero,one,zero,zero,one,zero,zero,GL_RGB,GL_RGB,BGR8,false,GL_UNSIGNED_BYTE_2_3_3_REV);
params[AV_PIX_FMT_RGB8] = RenderParams(AV_PIX_FMT_RGB8,one,zero,zero,one,zero,zero,one,zero,zero,GL_RGB,GL_RGB,BGR8,false,GL_UNSIGNED_BYTE_3_3_2);
params[AV_PIX_FMT_BGR4] = RenderParams(AV_PIX_FMT_BGR4,one,zero,zero,one,zero,zero,one,zero,zero,GL_LUMINANCE4,GL_LUMINANCE,BGR4,false,GL_UNSIGNED_BYTE);
params[AV_PIX_FMT_NV12] = RenderParams(AV_PIX_FMT_NV12,one,two,zero,one,two,zero,one,two,zero,GL_LUMINANCE,GL_LUMINANCE,GL_LUMINANCE_ALPHA,GL_LUMINANCE_ALPHA,GL_LUMINANCE,GL_LUMINANCE,NV12,true);
params[AV_PIX_FMT_NV21] = RenderParams(AV_PIX_FMT_NV21,one,two,zero,one,two,zero,one,two,zero,GL_LUMINANCE,GL_LUMINANCE,GL_LUMINANCE_ALPHA,GL_LUMINANCE_ALPHA,GL_LUMINANCE,GL_LUMINANCE,NV21,true);
params[AV_PIX_FMT_YUV420P10LE] = RenderParams(AV_PIX_FMT_YUV420P10LE,two, two,two,one,two,two,one,two,two,GL_LUMINANCE_ALPHA,GL_LUMINANCE_ALPHA,YUV420P10LE,true);
params[AV_PIX_FMT_YUV422P10LE] = RenderParams(AV_PIX_FMT_YUV422P10LE,two, two,two,one,one,one,one,one,one,GL_LUMINANCE_ALPHA,GL_LUMINANCE_ALPHA,YUV420P10LE,true);
params[AV_PIX_FMT_YUV444P10LE] = RenderParams(AV_PIX_FMT_YUV444P10LE,two,two,two,one,one,one,one,one,one,GL_LUMINANCE_ALPHA,GL_LUMINANCE_ALPHA,YUV420P10LE,true);
params[AV_PIX_FMT_YUVA420P10LE] = RenderParams(AV_PIX_FMT_YUVA420P10LE,two, two,two,one,two,two,one,two,two,GL_LUMINANCE_ALPHA,GL_LUMINANCE_ALPHA,YUV420P10LE,true);
params[AV_PIX_FMT_YUVA422P10LE] = RenderParams(AV_PIX_FMT_YUVA422P10LE,two, two,two,one,one,one,one,one,one,GL_LUMINANCE_ALPHA,GL_LUMINANCE_ALPHA,YUV420P10LE,true);
params[AV_PIX_FMT_YUVA444P10LE] = RenderParams(AV_PIX_FMT_YUVA444P10LE,two,two,two,one,one,one,one,one,one,GL_LUMINANCE_ALPHA,GL_LUMINANCE_ALPHA,YUV420P10LE,true);

params[AV_PIX_FMT_YUV410P] = RenderParams(AV_PIX_FMT_YUV410P,one,one,one,one,four,four,one,four,four,GL_LUMINANCE,GL_LUMINANCE,YUV,true);
params[AV_PIX_FMT_YUV411P] = RenderParams(AV_PIX_FMT_YUV411P,one,one,one,one,one,one,one,one,one,GL_LUMINANCE,GL_LUMINANCE,YUV,true);


params[AV_PIX_FMT_ARGB] = RenderParams(AV_PIX_FMT_ARGB,four,zero,zero,one,zero,zero,one,zero,zero,GL_RGBA,GL_RGBA,ARGB,false);
params[AV_PIX_FMT_RGBA] = RenderParams(AV_PIX_FMT_RGBA,four,zero,zero,one,zero,zero,one,zero,zero,GL_RGBA,GL_RGBA,RGBA,false);
params[AV_PIX_FMT_ABGR] = RenderParams(AV_PIX_FMT_ABGR,four,zero,zero,one,zero,zero,one,zero,zero,GL_RGBA,GL_RGBA,ABGR,false);
params[AV_PIX_FMT_BGRA] = RenderParams(AV_PIX_FMT_BGRA,four,zero,zero,one,zero,zero,one,zero,zero,GL_RGBA,GL_RGBA,BGRA,false);

params[AV_PIX_FMT_GRAY16BE] = RenderParams(AV_PIX_FMT_GRAY16BE,two,zero,zero,one,zero,zero,one,zero,zero,GL_LUMINANCE16,GL_LUMINANCE,GRAY,true,GL_UNSIGNED_SHORT);
params[AV_PIX_FMT_GRAY16LE] = RenderParams(AV_PIX_FMT_GRAY16LE,two,zero,zero,one,zero,zero,one,zero,zero,GL_LUMINANCE16,GL_LUMINANCE,GRAY,true,GL_UNSIGNED_SHORT);


params[AV_PIX_FMT_YUV440P] = RenderParams(AV_PIX_FMT_YUV440P,one,one,one,one,two,two,one,one,two,GL_LUMINANCE,GL_LUMINANCE,YUV,true);
params[AV_PIX_FMT_YUVJ440P] = RenderParams(AV_PIX_FMT_YUVJ440P,one,one,one,one,two,two,one,one,two,GL_LUMINANCE,GL_LUMINANCE,YUVJ,true);
params[AV_PIX_FMT_YUVA420P] = RenderParams(AV_PIX_FMT_YUVA420P,one,one,one,one,two,two,one,one,two,GL_LUMINANCE,GL_LUMINANCE,YUV,true);

params[AV_PIX_FMT_UYYVYY411] = RenderParams(AV_PIX_FMT_UYYVYY411,one,one,one,one,two,two,one,two,two,GL_LUMINANCE,GL_LUMINANCE,UYYVYY,false);


params[AV_PIX_FMT_RGB444LE] = RenderParams(AV_PIX_FMT_RGB444LE,two,zero,zero,one,zero,zero,one,zero,zero,GL_RGBA,GL_RGBA,RGB444,false,GL_UNSIGNED_SHORT_4_4_4_4);
params[AV_PIX_FMT_RGB444BE] = RenderParams(AV_PIX_FMT_RGB444BE,two,zero,zero,one,zero,zero,one,zero,zero,GL_RGBA,GL_RGBA,RGB444,false,GL_UNSIGNED_SHORT_4_4_4_4);

params[AV_PIX_FMT_BGR444LE] = RenderParams(AV_PIX_FMT_BGR444LE,two,zero,zero,one,zero,zero,one,zero,zero,GL_RGBA,GL_RGBA,BGR444,false,GL_UNSIGNED_SHORT_4_4_4_4);
params[AV_PIX_FMT_BGR444BE] = RenderParams(AV_PIX_FMT_BGR444BE,two,zero,zero,one,zero,zero,one,zero,zero,GL_RGBA,GL_RGBA,BGR444,false,GL_UNSIGNED_SHORT_4_4_4_4);




params[AV_PIX_FMT_YUV420P16LE] = RenderParams(AV_PIX_FMT_YUV420P16LE,two, two,two,one,two,two,one,two,two,GL_LUMINANCE16,GL_LUMINANCE,YUV16,true,GL_UNSIGNED_SHORT);
params[AV_PIX_FMT_YUV420P16BE] = RenderParams(AV_PIX_FMT_YUV420P16BE,two, two,two,one,two,two,one,two,two,GL_LUMINANCE16,GL_LUMINANCE,YUV16,true,GL_UNSIGNED_SHORT);

params[AV_PIX_FMT_YUV422P16LE] = RenderParams(AV_PIX_FMT_YUV422P16LE,two, two,two,one,one,one,one,one,one,GL_LUMINANCE16,GL_LUMINANCE,YUV16,true,GL_UNSIGNED_SHORT);
params[AV_PIX_FMT_YUV422P16BE] = RenderParams(AV_PIX_FMT_YUV422P16BE,two, two,two,one,one,one,one,one,one,GL_LUMINANCE16,GL_LUMINANCE,YUV16,true,GL_UNSIGNED_SHORT);

params[AV_PIX_FMT_YUV444P16LE] = RenderParams(AV_PIX_FMT_YUV444P16LE,two, two,two,one,one,one,one,one,one,GL_LUMINANCE16,GL_LUMINANCE,YUV16,true,GL_UNSIGNED_SHORT);
params[AV_PIX_FMT_YUV444P16BE] = RenderParams(AV_PIX_FMT_YUV444P16BE,two, two,two,one,one,one,one,one,one,GL_LUMINANCE16,GL_LUMINANCE,YUV16,true,GL_UNSIGNED_SHORT);

params[AV_PIX_FMT_YUVA420P16LE] = RenderParams(AV_PIX_FMT_YUVA420P16LE,two,two,two,one,two,two,one,one,one,GL_LUMINANCE16,GL_LUMINANCE,YUV,true,GL_UNSIGNED_SHORT);
params[AV_PIX_FMT_YUVA422P16LE] = RenderParams(AV_PIX_FMT_YUVA422P16LE,two,two,two,one,one,one,one,one,one,GL_LUMINANCE16,GL_LUMINANCE,YUV,true,GL_UNSIGNED_SHORT);
params[AV_PIX_FMT_YUVA444P16LE] = RenderParams(AV_PIX_FMT_YUVA444P16LE,two,two,two,one,one,one,one,one,one,GL_LUMINANCE16,GL_LUMINANCE,YUV,true,GL_UNSIGNED_SHORT);

params[AV_PIX_FMT_BAYER_BGGR16LE] = RenderParams(AV_PIX_FMT_BAYER_BGGR16LE,two,one,one,one,one,one,one,two,two,GL_LUMINANCE16,GL_LUMINANCE,BGGR,true,GL_UNSIGNED_SHORT);
params[AV_PIX_FMT_BAYER_RGGB16LE] = RenderParams(AV_PIX_FMT_BAYER_RGGB16LE,two,one,one,one,one,one,one,two,two,GL_LUMINANCE16,GL_LUMINANCE,RGGB,true,GL_UNSIGNED_SHORT);
params[AV_PIX_FMT_BAYER_GBRG16LE] = RenderParams(AV_PIX_FMT_BAYER_GBRG16LE,two,one,one,one,one,one,one,two,two,GL_LUMINANCE16,GL_LUMINANCE,GBRG,true,GL_UNSIGNED_SHORT);
params[AV_PIX_FMT_BAYER_GRBG16LE] = RenderParams(AV_PIX_FMT_BAYER_GRBG16LE,two,one,one,one,one,one,one,two,two,GL_LUMINANCE16,GL_LUMINANCE,GRBG,true,GL_UNSIGNED_SHORT);


params[AV_PIX_FMT_BAYER_GRBG16LE] = RenderParams(AV_PIX_FMT_BAYER_GRBG16LE,two,one,one,one,one,one,one,two,two,GL_LUMINANCE16,GL_LUMINANCE,GRBG,true,GL_UNSIGNED_SHORT);

params[AV_PIX_FMT_RGB565LE] = RenderParams(AV_PIX_FMT_RGB565LE,two,zero,zero,one,zero,zero,one,zero,zero,GL_RGB,GL_RGB,RGB,true,GL_UNSIGNED_SHORT_5_6_5);
params[AV_PIX_FMT_RGB555LE] = RenderParams(AV_PIX_FMT_RGB555LE,two,zero,zero,one,zero,zero,one,zero,zero,GL_BGRA,GL_BGRA,RGB,true,GL_UNSIGNED_SHORT_1_5_5_5_REV);


params[AV_PIX_FMT_BGR565LE] = RenderParams(AV_PIX_FMT_BGR565LE,two,zero,zero,one,zero,zero,one,zero,zero,GL_RGB,GL_RGB,BGR565,true,GL_UNSIGNED_SHORT_5_6_5);
params[AV_PIX_FMT_BGR555LE] = RenderParams(AV_PIX_FMT_RGB555LE,two,zero,zero,one,zero,zero,one,zero,zero,GL_BGRA,GL_BGRA,BGR565,true,GL_UNSIGNED_SHORT_1_5_5_5_REV);


params[AV_PIX_FMT_RGB48LE] = RenderParams(AV_PIX_FMT_RGB48LE,two,zero,zero,one,zero,zero,one,zero,zero,GL_LUMINANCE16,GL_LUMINANCE,RGB,true,GL_UNSIGNED_SHORT);
params[AV_PIX_FMT_BGR48LE] = RenderParams(AV_PIX_FMT_BGR48LE,two,zero,zero,one,zero,zero,one,zero,zero,GL_LUMINANCE16,GL_LUMINANCE,BGR,true,GL_UNSIGNED_SHORT);
params[AV_PIX_FMT_RGBA64LE] = RenderParams(AV_PIX_FMT_RGBA64LE,two,zero,zero,one,zero,zero,one,zero,zero,GL_LUMINANCE_ALPHA,GL_LUMINANCE,RGB,true,GL_UNSIGNED_SHORT);
params[AV_PIX_FMT_BGRA64LE] = RenderParams(AV_PIX_FMT_BGRA64LE,two,zero,zero,one,zero,zero,one,zero,zero,GL_LUMINANCE_ALPHA,GL_LUMINANCE,BGR,true,GL_UNSIGNED_SHORT);



//    params[AV_PIX_FMT_YUV420P9LE] = RenderParams(AV_PIX_FMT_YUV420P9LE,two, two,two,one,two,two,one,two,two,GL_LUMINANCE_ALPHA,GL_LUMINANCE_ALPHA,YUV420P10LE,true);


params[AV_PIX_FMT_NV16] = RenderParams(AV_PIX_FMT_NV16,one,one,one,one,two,two,one,one,one,GL_LUMINANCE,GL_LUMINANCE,YUV,true);
params[AV_PIX_FMT_NV20LE] = RenderParams(AV_PIX_FMT_NV20LE,one,one,one,one,two,two,one,two,two,GL_LUMINANCE,GL_LUMINANCE,YUV,true);



///硬解 DXVA same nv12
params[AV_PIX_FMT_DXVA2_VLD] = RenderParams(AV_PIX_FMT_DXVA2_VLD,one,two,zero,one,two,zero,one,two,zero,GL_LUMINANCE,GL_LUMINANCE,GL_LUMINANCE_ALPHA,GL_LUMINANCE_ALPHA,GL_LUMINANCE,GL_LUMINANCE,NV12,true);
