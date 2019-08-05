struct PixelFormat{
    AVPixelFormat format,
    AVRational    yWidth,
    AVRational    uWidth,
    AVRational    vWidth,
    AVRational    yHeight,
    AVRational    uHeight,
    AVRational    vHeight,
    AVRational    y,
    AVRational    u,
    AVRational    v,
    GLint         yf1,
    GLenum        yf2,
    GLint         uf1,
    GLenum        uf2,
    GLint         vf1,
    GLenum        vf2,
    int           f3,
    bool          isPlanar,
    GLenum dataType = GL_UNSIGNED_BYTE
};

AVRational zero  = {0,1};
AVRational one   = {1,1};
AVRational two   = {1,2};
AVRational three = {1,3};
AVRational four  = {1,4};

struct PixelFormat{
    //int textureFormat;
    /*
        , yInternalformat(yf1)
        , uInternalformat(uf1)
        , vInternalformat(vf1)
        , yGlFormat(yf2)
        , uGlFormat(uf2)
        , vGlFormat(vf2)
        , textureFormat(f3)

    */
    AVRational yuvSizes[3];
    AVRational yuvWidths[3];
    AVRational yuvHeights[3];
    GLint      yuvInternalFormat[3];
    GLenum     yuvGlFormat[3];

    GLenum dataType;
    bool isPlanar;
}

std::vector<PixelFormat> pixelFormats

pixelFormats[AV_PIX_FMT_YUV420P10LE] = {
    //f1,f2,f3
    //GL_LUMINANCE_ALPHA,GL_LUMINANCE_ALPHA,YUV420P10LE
    {one,two,two},  //AVRational yuvSizes[3];
    {two,two,two},  //AVRational yuvWidths[3];
    {one,two,two},  //AVRational yuvHeights[3];
    {GL_LUMINANCE_ALPHA, },             //GLint yuvInternalFormat[3];
    {},             //GLenum yuvGlFormat[3];

}

class RenderParams{
    public :
        RenderParams(AV_PIX_FMT_YUV420P10LE,          two,          two,          two,          one,          two,          two,         one,         two,         two,GL_LUMINANCE_ALPHA,GL_LUMINANCE_ALPHA,YUV420P10LE,true);
        RenderParams(AVPixelFormat f,       AVRational yw,AVRational uw,AVRational vw,AVRational yh,AVRational uh,AVRational vh,AVRational y,AVRational u,AVRational v,GLint f1          ,GLenum f2,         int f3,     bool planar,GLenum dataType = GL_UNSIGNED_BYTE): RenderParams(f,yw,uw,vw,yh,uh,vh,y,u,v,f1,f2,f1,f2,f1,f2,f3,planar,dataType){}
        RenderParams(AVPixelFormat f       ,AVRational yw,AVRational uw,AVRational vw,AVRational yh,AVRational uh,AVRational vh,AVRational y,AVRational u,AVRational v,GLint yf1         ,       GLenum yf2,GLint uf1,GLenum uf2,GLint vf1,GLenum vf2,int f3,bool planar,GLenum dataType = GL_UNSIGNED_BYTE)
            : format(f)
            , yWidth(yw)
            , uWidth(uw)
            , vWidth(vw)

            , yHeight(yh)
            , uHeight(uh)
            , vHeight(vh)

            , ySize(y)
            , uSize(u)
            , vSize(v)

            , yInternalformat(yf1)
            , uInternalformat(uf1)
            , vInternalformat(vf1)
            , yGlFormat(yf2)
            , uGlFormat(uf2)
            , vGlFormat(vf2)
            , textureFormat(f3)

            , isPlanar(planar)
            , dataType(dataType)
        {

            yuvsizes[0] = ySize;
            yuvsizes[1] = uSize;
            yuvsizes[2] = vSize;

            yuvwidths[0] = yWidth;
            yuvwidths[1] = uWidth;
            yuvwidths[2] = vWidth;

            yuvheights[0] = yHeight;
            yuvheights[1] = uHeight;
            yuvheights[2] = vHeight;

            yuvInternalformat[0] = yInternalformat;
            yuvInternalformat[1] = uInternalformat;
            yuvInternalformat[2] = vInternalformat;

            yuvGlFormat[0] = yGlFormat;
            yuvGlFormat[1] = uGlFormat;
            yuvGlFormat[2] = vGlFormat;
        }
    AVPixelFormat format;

    AVRational yWidth; //y的宽度(比率)
    AVRational uWidth; //u的宽度(比率)
    AVRational vWidth; //v的宽度(比率)

    AVRational yHeight; //y的高度(比率)
    AVRational uHeight; //u的高度(比率)
    AVRational vHeight; //v的高度(比率)

    AVRational ySize;// y的大小(比率)
    AVRational uSize;// u的大小(比率)
    AVRational vSize;// v的大小(比率)

    GLint yInternalformat; //数据位数
    GLint uInternalformat; //数据位数
    GLint vInternalformat; //数据位数

    GLenum yGlFormat; //
    GLenum uGlFormat; //
    GLenum vGlFormat; //

    int textureFormat; //自定义纹理格式，GL使用

    bool isPlanar; //是否是平面 true : 平面 | false : 打包

    AVRational yuvsizes[3];
    AVRational yuvwidths[3];
    AVRational yuvheights[3];
    GLint yuvInternalformat[3];
    GLenum yuvGlFormat[3];

    GLenum dataType;
}

