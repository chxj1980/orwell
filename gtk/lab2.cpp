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
        std::cout << "realize" << std::endl;
        GtkWidget *widget = this->Widget::gobj();
        EGLConfig eglConfig;
        EGLint n_config;
        EGLint attributes[] = {EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT,
                               EGL_NONE};

        eglDisplay = eglGetDisplay((EGLNativeDisplayType)gdk_x11_display_get_xdisplay(gtk_widget_get_display(widget)));
        eglInitialize(&eglDisplay, NULL, NULL);
        eglChooseConfig(&eglDisplay, attributes, &eglConfig, 1, &n_config);
        eglBindAPI(EGL_OPENGL_API);
        eglSurface = eglCreateWindowSurface(&eglDisplay, &eglConfig, gdk_x11_window_get_xid(gtk_widget_get_window(widget)), NULL);
        eglContext = eglCreateContext(&eglDisplay, &eglConfig, EGL_NO_CONTEXT, NULL);
    };


    virtual bool render(const Glib::RefPtr<Gdk::GLContext> &context)
    {
        glDraw();
        glFinish();
        return true;
    }

    void glDraw()
    {
        std::cout << "drawing" << std::endl;
        eglMakeCurrent(&eglDisplay, &eglSurface, &eglSurface, &eglContext);
        GtkWidget* widget = this->Widget::gobj();
        //glViewport(0, 0, gtk_widget_get_allocated_width(widget), gtk_widget_get_allocated_height(widget));

        eglSwapBuffers(&eglDisplay, &eglSurface);
        std::cout << "finished drawing" << std::endl;
    }

private:
    EGLDisplay eglDisplay;
    EGLSurface eglSurface;
    EGLContext eglContext;
};

int main(int argc, char **argv)
{
	auto app = Gtk::Application::create(argc, argv, "");
    MyOpenGLArea myOpenGLArea;
	return app->run(myOpenGLArea);
}