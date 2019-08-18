/* Open GL Area
 *
 * Gtk::GLArea is a widget that allows custom drawing using OpenGL calls.
 *
 */

#include "OpenGLArea2.h"

using std::cerr;
using std::endl;
using std::string;

OpenGLArea2::OpenGLArea2()
{
    set_title("Orwell Experiments");
    set_default_size(640, 360);

    //m_VBox.property_margin() = 12;
    //m_VBox.set_spacing(6);
    add(m_VBox);

    glArea.set_hexpand(true);
    glArea.set_vexpand(true);
    glArea.set_auto_render(true);
    m_VBox.add(glArea);

    // Connect gl area signals
    //glArea.signal_realize().connect(sigc::mem_fun(*this, &OpenGLArea2::realize));
    // Important that the unrealize signal calls our handler to clean up
    // GL resources _before_ the default unrealize handler is called (the "false")
    //glArea.signal_unrealize().connect(sigc::mem_fun(*this, &OpenGLArea2::unrealize), false);
    glArea.signal_render().connect(sigc::mem_fun(*this, &OpenGLArea2::render), false);

    glArea.show();

    m_VBox.show();
}

OpenGLArea2::~OpenGLArea2()
{
}
/* 
void OpenGLArea2::realize()
{
    cerr << "Realize !" << endl;

    glArea.make_current();
    try
    {
        glArea.throw_if_error();
        //init_buffers();
        //init_shaders();
    }
    catch (const Gdk::GLError &gle)
    {
        cerr << "An error occured making the context current during realize:" << endl;
        cerr << gle.domain() << "-" << gle.code() << "-" << gle.what() << endl;
    }
}

void OpenGLArea2::unrealize()
{
    glArea.make_current();
    try
    {
        glArea.throw_if_error();

        // Delete buffers and program
        //glDeleteBuffers(1, &m_Vao);
        //glDeleteProgram(m_Program);
    }
    catch (const Gdk::GLError &gle)
    {
        cerr << "An error occured making the context current during unrealize" << endl;
        cerr << gle.domain() << "-" << gle.code() << "-" << gle.what() << endl;
    }
}
*/
//void Example_GLArea::draw(const Glib::RefPtr<Gtk::Adjustment> &adj)
//{
    //m_RotationAngles[axis] = adj->get_value();
    //openGLArea2.queue_draw();
//}
/* 
bool OpenGLArea2::render(const Glib::RefPtr<Gdk::GLContext> & )
{
    try
    {
        glArea.throw_if_error();
        glClearColor(0.5, 0.5, 0.5, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        draw_triangle();

        glFlush();

        return true;
    }
    catch (const Gdk::GLError &gle)
    {
        cerr << "An error occurred in the render callback of the GLArea" << endl;
        cerr << gle.domain() << "-" << gle.code() << "-" << gle.what() << endl;
        return false;
    }
}
*/


//----------
/* 
int main(int argc, char **argv)
{
    auto app = Gtk::Application::create(argc, argv, "org.brianna.love");

    OpenGLArea2 hw;

    return app->run(hw);
}
*/
