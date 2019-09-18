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
    MyOpenGLArea()
    {
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
        eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
        assertEGLError("eglGetDisplay");

        eglInitialize(eglDisplay, nullptr, nullptr);
        assertEGLError("eglInitialize");

        eglChooseConfig(eglDisplay, nullptr, &eglConfig, 1, &numConfigs);
        assertEGLError("eglChooseConfig");

        eglBindAPI(EGL_OPENGL_API);
        assertEGLError("eglBindAPI");

        eglContext = eglCreateContext(eglDisplay, eglConfig, EGL_NO_CONTEXT, NULL);
        assertEGLError("eglCreateContext");

        //surface = eglCreatePbufferSurface(eglDisplay, eglConfig, nullptr);
        //assertEGLError("eglCreatePbufferSurface");

        eglMakeCurrent(eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, eglContext);
        assertEGLError("eglMakeCurrent");
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
    EGLSurface eglSurface;
    EGLContext eglContext;
    int numConfigs;
};

int main(int argc, char **argv)
{
    auto app = Gtk::Application::create(argc, argv, "");
    MyOpenGLArea myOpenGLArea;
    return app->run(myOpenGLArea);
}