#include <iostream>
#include <gtkmm.h>
#include <epoxy/gl.h>
#include <gtk/gtk.h>
#include <gdk/gdkx.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GL/gl.h>
class MyOpenGLArea : public Gtk::Window
{
public:
    MyOpenGLArea(){
        set_title("Test");
        set_default_size(640, 360);

        add(vBox);

        glArea.set_hexpand(true);
        glArea.set_vexpand(true);
        glArea.set_auto_render(true);
        vBox.add(glArea);

        glArea.signal_realize().connect(sigc::mem_fun(*this, &MyOpenGLArea::realize));
        glArea.signal_render().connect(sigc::mem_fun(*this, &MyOpenGLArea::render), false);

        glArea.show();
        vBox.show();
    };

public:
    Gtk::GLArea glArea;
    Gtk::Box vBox{Gtk::ORIENTATION_VERTICAL, false};

    void realize()
    {
        EGLBoolean eglStatus;
        EGLConfig eglConfig;
        EGLint n_config;
        EGLint context_attribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };

        eglDisplay = eglGetDisplay((EGLNativeDisplayType)gdk_x11_display_get_xdisplay(glArea.get_display()->gobj()));

        eglStatus = eglInitialize(eglDisplay, NULL, NULL);
        if (!eglStatus)
    {
        printf("Error at eglInitialize\n");
            switch(eglStatus) {
                case EGL_BAD_DISPLAY:
                    printf("EGL_BAD_DISPLAY\n");
                    break;
                case EGL_NOT_INITIALIZED:
                    printf("EGL_NOT_INITIALIZED\n");
                    break;
                case EGL_FALSE:
                    printf("EGL_FALSE\n");
                    break;
            }
        }
        eglStatus = eglChooseConfig(eglDisplay, context_attribs, &eglConfig, 1, &numConfigs);
        if (!eglStatus)
    {
        printf("Error at eglChooseConfig\n");
            switch(eglStatus) {
                case EGL_BAD_DISPLAY:
                    printf("EGL_BAD_DISPLAY\n");
                    break;
                case EGL_BAD_ATTRIBUTE:
                    printf("EGL_BAD_ATTRIBUTE\n");
                    break;
                case EGL_NOT_INITIALIZED:
                    printf("EGL_NOT_INITIALIZED\n");
                    break;
                case EGL_BAD_PARAMETER:
                    printf("EGL_BAD_PARAMETER\n");
                    break;
                case EGL_FALSE:
                    printf("EGL_FALSE\n");
                    break;
            }
    }
    };


    virtual bool render(const Glib::RefPtr<Gdk::GLContext> &context)
    {
        glDraw();
        glFinish();
        return true;
    }

    void glDraw()
    {

    }

private:
    EGLDisplay eglDisplay;  

        eglSwapBuffers(&eglDisplay, &eglSurface);
        std::cout << "finished drawing" << std::endl;
    EGLSurface eglSurface;
    EGLContext eglContext;
};

int main(int argc, char **argv)
{
    auto app = Gtk::Application::create(argc, argv, "");
    MyOpenGLArea myOpenGLArea;
    return app->run(myOpenGLArea);
}