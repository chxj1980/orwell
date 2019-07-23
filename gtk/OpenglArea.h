#ifndef _EVG_OPEN_GL_AREA_
#define _EVG_OPEN_GL_AREA_

#include <gtkmm/drawingarea.h>
#include "VideoReceiver.h"
#include "Program.h"
#include <GL/glew.h>
#include <GL/glx.h>

class OpenGLArea : public Gtk::DrawingArea, public VideoReceiver {
  public:
    OpenGLArea();
    unsigned char *buffer[3] = {0};
    void init();
    void receiveVideo(unsigned char**videoBuffer, int frameWidth, int frameHeight);

    virtual void on_realize();
    virtual bool on_draw (const Cairo::RefPtr<Cairo::Context> &);
    virtual bool on_configure_event (GdkEventConfigure *event);

    virtual void glInit();
    virtual void glDraw();

  private:
    bool firstFrameReceived = false;
    void on_glx_init ();
    int frameWidth = 0;
    int frameHeight = 0;
    GLuint unis[3] = {0};
    GLuint texs[3] = {0};
    Program* program;
    //GLuint program;
    Display *xdisplay;
    GLXDrawable drawable;
    GLXContext context;
};


#endif
