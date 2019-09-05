
#ifndef OPENGLAREA_H
#define OPENGLAREA_H
#include <iostream>
#include <string>
#include <vector>

#include <gtkmm.h>
#include <epoxy/gl.h>

class OpenGLArea2 : public Gtk::Window
{
public:
    OpenGLArea2();
    ~OpenGLArea2() override;

public:
    Gtk::Box m_VBox{Gtk::ORIENTATION_VERTICAL, false};
    Gtk::GLArea glArea;
    //Gtk::Box m_Controls{Gtk::ORIENTATION_VERTICAL, false};


    virtual void realize() = 0;
    //void unrealize();
    virtual bool render(const Glib::RefPtr<Gdk::GLContext> &context){return true;};
    //void draw_triangle();
};
#endif //OPENGLAREA_H