#ifndef _EVG_OPEN_GL_AREA_
#define _EVG_OPEN_GL_AREA_

#include <gtkmm/drawingarea.h>
#include "VideoReceiver.h"
#include <GL/glew.h>
#include <GL/glx.h>

class OpenGLArea : public Gtk::DrawingArea, public VideoReceiver {
  public:
    OpenGLArea ();
    unsigned char *buffer[3] = {0};
    void receiveVideo(unsigned char**videoBuffer, int frameWidth, int frameHeight);}

  protected:
    virtual void on_realize ();
    virtual bool on_draw (const Cairo::RefPtr<Cairo::Context> &);
    virtual bool on_configure_event (GdkEventConfigure *event);

    virtual void glInit();
    virtual void glDraw();

  private:
    void on_glx_init ();
    //GLuint program;
    int frameWidth = 0;
    int frameHeight = 0;
    GLuint unis[3] = {0};
    GLuint texs[3] = {0};
    Display *xdisplay;
    GLXDrawable drawable;
    GLXContext context;
};


#endif
