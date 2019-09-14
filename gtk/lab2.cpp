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
    }

public:
    Gtk::GLArea glArea;
    Gtk::Box vBox{Gtk::ORIENTATION_VERTICAL, false};

    void realize()
    {
        std::cout << "realize" << std::endl;
        EGLConfig egl_config;
        EGLint n_config;
        EGLint attributes[] = {EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT,
                            EGL_NONE};

        eglDisplay = eglGetDisplay((EGLNativeDisplayType)gdk_x11_display_get_xdisplay(glArea.get_display()->gobj()));

        eglInitialize(&eglDisplay, NULL, NULL);
        eglChooseConfig(&eglDisplay, attributes, &egl_config, 1, &n_config);
        eglBindAPI(EGL_OPENGL_API);
        eglSurface = eglCreateWindowSurface(&eglDisplay, egl_config, gdk_x11_window_get_xid(glArea.get_window()->gobj()), NULL);
        eglContext = eglCreateContext(&eglDisplay, egl_config, EGL_NO_CONTEXT, NULL);

    }
    bool render(const Glib::RefPtr<Gdk::GLContext> &context)
    {
        glDraw();
        glFinish();
        return true;
    }

    void glDraw()
    {
        std::cout << "drawing" << std::endl;
        eglMakeCurrent(&eglDisplay, &eglSurface, &eglSurface, &eglContext);
        /*
        if (this->firstRun)
		{
			std::cout << "firstRun of NVidiaRenderer" << std::endl;

			uint32_t pos_location = 0;
			uint32_t tc_location = 0;
			glEnable(GL_SCISSOR_TEST);
			program = std::make_unique<Program>();
			Shader vertexShader(ShaderType::Vertex);
			vertexShader.load_from_string(vertexShaderSource);
			//) Shader(ShaderType::Vertex, "video.vert")
			program->attach_shader(vertexShader);
			

			Shader fragmentShader(ShaderType::Fragment);
			fragmentShader.load_from_string(fragmentShaderSource);
			program->attach_shader(fragmentShader);

			program->link();
        }
        //glViewport(0, 0, gtk_widget_get_allocated_width(widget), gtk_widget_get_allocated_height(widget));
        */
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