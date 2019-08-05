#ifndef _EVG_OPEN_GL_AREA_
#define _EVG_OPEN_GL_AREA_

#include <gtkmm/drawingarea.h>
#include "VideoReceiver.h"
#include "Program.h"
#include "Frame.h"
#include <GL/glew.h>
#include <GL/glx.h>

class OpenGLArea : public Gtk::DrawingArea, public VideoReceiver {
  public:
    OpenGLArea();
    unsigned char *buffer[3] = {0};
    void init();
    int receiveVideo(Frame* frame);

    void on_realize();
    bool on_draw (const Cairo::RefPtr<Cairo::Context> &);
    bool on_configure_event (GdkEventConfigure *event);

    virtual void glInit();
    virtual void glDraw();

  protected:
    bool firstFrameReceived = false;

  private:
    void on_glx_init ();
    int frameWidth = 0;
    int frameHeight = 0;
    GLuint unis[3] = {0};
    GLuint texs[3] = {0};
    Program program;
    //GLuint program;
    Display *xdisplay;
    GLXDrawable drawable;
    GLXContext context;
};


#endif
