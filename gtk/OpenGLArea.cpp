#include "OpenGLArea.h"

#include <gtkmm/drawingarea.h>
#include <gdk/gdkx.h>

#include <memory>
#include <iostream>
#include <algorithm>

//#include <GL/glew.h>
//#include <GL/glx.h>

#define VERTEX_POINTER 0
#define FRAGMENT_POINTER 1

class XVisualInfoDeleter
{
public:
	void operator()(XVisualInfo *visual) const
	{
		XFree(visual);
	}
};

#define GLX_CONTEXT_MAJOR_VERSION_ARB 0x2091
#define GLX_CONTEXT_MINOR_VERSION_ARB 0x2092
typedef GLXContext (*glXCreateContextAttribsARBProc)(Display *, GLXFBConfig, GLXContext, Bool, const int *);

// Helper to check for extension string presence.  Adapted from:
//   http://www.opengl.org/resources/features/OGLextensions/
static bool isExtensionSupported(const char *extList, const char *extension)
{
	const char *start;
	const char *where, *terminator;

	/* Extension names should not have spaces. */
	where = strchr(extension, ' ');
	if (where || *extension == '\0')
		return false;

	/* It takes a bit of care to be fool-proof about parsing the
     OpenGL extensions string. Don't be fooled by sub-strings,
     etc. */
	for (start = extList;;)
	{
		where = strstr(start, extension);

		if (!where)
			break;

		terminator = where + strlen(extension);

		if (where == start || *(where - 1) == ' ')
			if (*terminator == ' ' || *terminator == '\0')
				return true;

		start = terminator;
	}

	return false;
}

static bool ctxErrorOccurred = false;
static int ctxErrorHandler(Display *dpy, XErrorEvent *ev)
{
	ctxErrorOccurred = true;
	return 0;
}

OpenGLArea::OpenGLArea()
{
	// Get the default screen's GLX extension list
	const char *glxExts = glXQueryExtensionsString(display,
												   DefaultScreen(display));
	 // Get a matching FB config
  static int visual_attribs[] =
    {
      GLX_X_RENDERABLE    , True,
      GLX_DRAWABLE_TYPE   , GLX_WINDOW_BIT,
      GLX_RENDER_TYPE     , GLX_RGBA_BIT,
      GLX_X_VISUAL_TYPE   , GLX_TRUE_COLOR,
      GLX_RED_SIZE        , 8,
      GLX_GREEN_SIZE      , 8,
      GLX_BLUE_SIZE       , 8,
      GLX_ALPHA_SIZE      , 8,
      GLX_DEPTH_SIZE      , 24,
      GLX_STENCIL_SIZE    , 8,
      GLX_DOUBLEBUFFER    , True,
      //GLX_SAMPLE_BUFFERS  , 1,
      //GLX_SAMPLES         , 4,
      None
    };
	printf("Getting matching framebuffer configs\n");
	int fbcount;
	GLXFBConfig *fbc = glXChooseFBConfig(display, DefaultScreen(display), visual_attribs, &fbcount);
	if (!fbc)
	{
		printf("Failed to retrieve a framebuffer config\n");
		exit(1);
	}
	printf("Found %d matching FB configs.\n", fbcount);

	// Pick the FB config/visual with the most samples per pixel
	printf("Getting XVisualInfos\n");
	int best_fbc = -1, worst_fbc = -1, best_num_samp = -1, worst_num_samp = 999;

	int i;
	for (i = 0; i < fbcount; ++i)
	{
		XVisualInfo *vi = glXGetVisualFromFBConfig(display, fbc[i]);
		if (vi)
		{
			int samp_buf, samples;
			glXGetFBConfigAttrib(display, fbc[i], GLX_SAMPLE_BUFFERS, &samp_buf);
			glXGetFBConfigAttrib(display, fbc[i], GLX_SAMPLES, &samples);

			printf("  Matching fbconfig %d, visual ID 0x%2x: SAMPLE_BUFFERS = %d,"
				   " SAMPLES = %d\n",
				   i, vi->visualid, samp_buf, samples);

			if (best_fbc < 0 || samp_buf && samples > best_num_samp)
				best_fbc = i, best_num_samp = samples;
			if (worst_fbc < 0 || !samp_buf || samples < worst_num_samp)
				worst_fbc = i, worst_num_samp = samples;
		}
		XFree(vi);
	}

	GLXFBConfig bestFbc = fbc[best_fbc];

	// NOTE: It is not necessary to create or make current to a context before
	// calling glXGetProcAddressARB
	glXCreateContextAttribsARBProc glXCreateContextAttribsARB = 0;
	glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc)
		glXGetProcAddressARB((const GLubyte *)"glXCreateContextAttribsARB");

	GLXContext ctx = 0;

	// Install an X error handler so the application won't exit if GL 3.0
	// context allocation fails.
	//
	// Note this error handler is global.  All display connections in all threads
	// of a process use the same error handler, so be sure to guard against other
	// threads issuing X commands while this code is running.
	ctxErrorOccurred = false;
	int (*oldHandler)(Display *, XErrorEvent *) =
		XSetErrorHandler(&ctxErrorHandler);

	// Check for the GLX_ARB_create_context extension string and the function.
	// If either is not present, use GLX 1.3 context creation method.
	if (!isExtensionSupported(glxExts, "GLX_ARB_create_context") ||
		!glXCreateContextAttribsARB)
	{
		printf("glXCreateContextAttribsARB() not found"
			   " ... using old-style GLX context\n");
		ctx = glXCreateNewContext(display, bestFbc, GLX_RGBA_TYPE, 0, True);
	}

	// If it does, try to get a GL 3.0 context!
	else
	{
		int context_attribs[] =
			{
				GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
				GLX_CONTEXT_MINOR_VERSION_ARB, 3,
				//GLX_CONTEXT_FLAGS_ARB        , GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
				None};

		printf("Creating context\n");
		ctx = glXCreateContextAttribsARB(display, bestFbc, 0,
										 True, context_attribs);

		// Sync to ensure any errors generated are processed.
		XSync(display, False);
		if (!ctxErrorOccurred && ctx)
			printf("Created GL 3.0 context\n");
		else
		{
			// Couldn't create GL 3.0 context.  Fall back to old-style 2.x context.
			// When a context version below 3.0 is requested, implementations will
			// return the newest context version compatible with OpenGL versions less
			// than version 3.0.
			// GLX_CONTEXT_MAJOR_VERSION_ARB = 1
			context_attribs[1] = 1;
			// GLX_CONTEXT_MINOR_VERSION_ARB = 0
			context_attribs[3] = 0;

			ctxErrorOccurred = false;

			printf("Failed to create GL 3.0 context"
				   " ... using old-style GLX context\n");
			ctx = glXCreateContextAttribsARB(display, bestFbc, 0,
											 True, context_attribs);
		}
	}
}

void OpenGLArea::on_realize()
{
	Gtk::DrawingArea::on_realize();

	on_glx_init();

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK)
	{
		throw std::runtime_error("Failed to initialize GLEW");
	}

	std::cout << "Vendor: ........... " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Renderer: ......... " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "Version: .......... " << glGetString(GL_VERSION) << std::endl;
	std::cout << "GLSL version: ..... " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

	glInit();
}

void OpenGLArea::on_glx_init()
{
	GLint glx_attrs[] = {
		GLX_RGBA,
		GLX_RED_SIZE,
		8,
		GLX_GREEN_SIZE,
		8,
		GLX_BLUE_SIZE,
		8,
		GLX_ALPHA_SIZE,
		8,
		GLX_DEPTH_SIZE,
		24,
		GLX_STENCIL_SIZE,
		8,
		GLX_DOUBLEBUFFER,
		False,
		None,
	};

	display = GDK_WINDOW_XDISPLAY(Glib::unwrap(get_window()));
	drawable = GDK_WINDOW_XID(Glib::unwrap(get_window()));
	const std::unique_ptr<XVisualInfo, XVisualInfoDeleter> visual_info(glXChooseVisual(display, DefaultScreen(display), glx_attrs));

	if (!visual_info)
	{
		throw std::runtime_error("failed glXChooseVisual.");
	}

	context = glXCreateContext(display, visual_info.get(), nullptr, True);
	if (!context)
	{
		throw std::runtime_error("failed glXCreateContext.");
	}

	if (!glXMakeCurrent(display, drawable, context))
	{
		throw std::runtime_error("failed glXMakeCurrent.");
	}
}

bool OpenGLArea::on_configure_event(GdkEventConfigure *event)
{
	if (display != nullptr && !glXMakeCurrent(display, drawable, context))
		return false;
	GLsizei width = get_allocated_width();
	GLsizei height = get_allocated_height();
	GLsizei size = std::min(width, height);

	glViewport(0, 0, width, height);

	return true;
}

bool OpenGLArea::on_draw(const Cairo::RefPtr<Cairo::Context> &cr)
{
	if (!glXMakeCurrent(display, drawable, context))
		return false;

	glDraw();

	glXSwapBuffers(display, drawable);

	return true;
}
