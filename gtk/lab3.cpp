#include <iostream>
#include <gtkmm.h>
#include <epoxy/gl.h>
#include <gtk/gtk.h>
#include <gdk/gdkx.h>
#include <GL/gl.h>
#include <chrono>
#include <thread>

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

        glArea.signal_render().connect(sigc::mem_fun(*this, &MyOpenGLArea::render), false);

        glArea.show();
        vBox.show();
    };

    void run()
    {
        while (true)
        {
            printf("run()\n");
            queue_draw();
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
    }

    virtual bool render(const Glib::RefPtr<Gdk::GLContext> &context)
    {
        printf("render()\n");
        return true;
    }

public:
    Gtk::GLArea glArea;
    Gtk::Box vBox{Gtk::ORIENTATION_VERTICAL, false};
};

int main(int argc, char **argv)
{
    auto app = Gtk::Application::create(argc, argv, "");
    MyOpenGLArea myOpenGLArea;
    std::thread t(&MyOpenGLArea::run, &myOpenGLArea);
    return app->run(myOpenGLArea);
}