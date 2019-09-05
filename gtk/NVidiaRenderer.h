#ifndef NvidiaRenderer_H
#define NvidiaRenderer_H
#include "OpenGLArea2.h"
#include "VideoReceiver.h"
#include "Program.h"
#include "DecodedFrame.h"
#include "PixelFormats.h"
#include "Singleton.h"
#include "ThreadSafeDeque.h"
#include <mutex>
#include <condition_variable>
#include <stdexcept>
#include <gtk/gtk.h>
#include <gdk/gdkx.h>
#include <EGL/egl.h>
#include <GL/gl.h>

#define TEST_ERROR(condition, message)     \
    if (condition)                         \
    {                                      \
        throw std::runtime_error(message); \
    }

struct NVidiaRendererException : public std::exception
{
public:
    NVidiaRendererException(std::string error, int errorCode) : error(error), errorCode(errorCode)
    {
    }
    const char *what() const throw()
    {
        if (errorCode)
            return (error + ": " + std::to_string(errorCode)).c_str();
        else
            return error.c_str();
    }

private:
    std::string error;
    int errorCode;
};

class NvidiaRenderer : public OpenGLArea2 //, public VideoReceiver
{
public:
    unsigned char *buffer[3] = {0};
    NvidiaRenderer()
    {
        eglCreateImageKHR =
            (PFNEGLCREATEIMAGEKHRPROC)eglGetProcAddress("eglCreateImageKHR");
        TEST_ERROR(!eglCreateImageKHR,
                   "ERROR getting proc addr of eglCreateImageKHR");

        eglDestroyImageKHR =
            (PFNEGLDESTROYIMAGEKHRPROC)eglGetProcAddress("eglDestroyImageKHR");
        TEST_ERROR(!eglDestroyImageKHR,
                   "ERROR getting proc addr of eglDestroyImageKHR");

        eglCreateSyncKHR =
            (PFNEGLCREATESYNCKHRPROC)eglGetProcAddress("eglCreateSyncKHR");
        TEST_ERROR(!eglCreateSyncKHR,
                   "ERROR getting proc addr of eglCreateSyncKHR");

        eglDestroySyncKHR =
            (PFNEGLDESTROYSYNCKHRPROC)eglGetProcAddress("eglDestroySyncKHR");
        TEST_ERROR(!eglDestroySyncKHR,
                   "ERROR getting proc addr of eglDestroySyncKHR");

        eglClientWaitSyncKHR =
            (PFNEGLCLIENTWAITSYNCKHRPROC)eglGetProcAddress("eglClientWaitSyncKHR");
        TEST_ERROR(!eglClientWaitSyncKHR,
                   "ERROR getting proc addr of eglClientWaitSyncKHR");

        eglGetSyncAttribKHR =
            (PFNEGLGETSYNCATTRIBKHRPROC)eglGetProcAddress("eglGetSyncAttribKHR");
        TEST_ERROR(!eglGetSyncAttribKHR,
                   "ERROR getting proc addr of eglGetSyncAttribKHR");

        glEGLImageTargetTexture2DOES =
            (PFNGLEGLIMAGETARGETTEXTURE2DOESPROC)
                eglGetProcAddress("glEGLImageTargetTexture2DOES");
        TEST_ERROR(!glEGLImageTargetTexture2DOES,
                   "ERROR getting proc addr of glEGLImageTargetTexture2DOES");
    }
    void init();
    //int receiveVideo(DecodedFrame& frame);
    void realize();
    bool render(const Glib::RefPtr<Gdk::GLContext> &context);
    void setDecodedFramesFifo(std::shared_ptr<ThreadSafeDeque<DecodedFrame>> decodedFramesFifo)
    {
        this->decodedFramesFifo = decodedFramesFifo;
    }

    //virtual void on_realize();
    //virtual bool on_draw (const Cairo::RefPtr<Cairo::Context> &);
    //virtual bool on_configure_event (GdkEventConfigure *event);

    void glInit();
    void glDraw();

    void run();

    const GLfloat vertices_textures[20] = {
        //vertices            //positions
        -1.0f, -1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, -1.0f, 0.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 0.0f, 1.0f, 0.0f};

    const int VERTEX_POINTER = 0;
    const int FRAGMENT_POINTER = 1;

protected:
    bool firstFrameReceived = false;
    std::unique_ptr<Program> program;
    std::shared_ptr<ThreadSafeDeque<DecodedFrame>> decodedFramesFifo;

private:
    //void on_glx_init ();
    DecodedFrame frame;
    int frameWidth = 0;
    int frameHeight = 0;
    int format;
    bool firstRun = true;
    bool initiatedTextures = false;
    bool initiatedFrameBufferObjects = false;
    static const int TEXTURE_NUMBER = 3;
    GLuint textureId[TEXTURE_NUMBER];
    //int textureFormat;
    GLint textureLocation[TEXTURE_NUMBER];
    GLuint pixelBufferObjects[TEXTURE_NUMBER];
    unsigned int vertexBufferObject, vertexArrayObject, TBO, EBO;
    GLint vextexInLocation;
    GLint textureInLocation;
    std::mutex mutex;
    std::condition_variable conditionVariable;
    static PFNEGLCREATEIMAGEKHRPROC eglCreateImageKHR;
    static PFNEGLDESTROYIMAGEKHRPROC eglDestroyImageKHR;
    static PFNEGLCREATESYNCKHRPROC eglCreateSyncKHR;
    static PFNEGLDESTROYSYNCKHRPROC eglDestroySyncKHR;
    static PFNEGLCLIENTWAITSYNCKHRPROC eglClientWaitSyncKHR;
    static PFNEGLGETSYNCATTRIBKHRPROC eglGetSyncAttribKHR;
    static PFNGLEGLIMAGETARGETTEXTURE2DOESPROC glEGLImageTargetTexture2DOES;
    uint32_t texture_id;
    int render_fd;
    EGLDisplay *egl_display;
    EGLSurface *egl_surface;
    EGLContext *egl_context;
    //std::mutex mutex;
    //std::condition_variable conditionVariable;
    //PixelFormats pixelFormats;
    //GLuint program;
    //Display *xdisplay;
    //GLXDrawable drawable;
    //GLXContext context;
};

#endif //NvidiaRenderer_H
