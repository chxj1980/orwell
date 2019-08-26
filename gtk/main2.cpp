//#include <gtkmm/window.h>
//#include <gtkmm/box.h>
//#include <gtkmm/button.h>
//#include <gtkmm/main.h>

#include "MyRTSPClient.h"
#include "FfmpegHardwareDecoder.h"
#include "FfmpegSoftwareDecoder.h"
#include "Singleton.h"
#include "VideoReceiver.h"

#include <iostream>
//#include <GL/glew.h>
//#include <GL/glx.h>
#include <thread>
#include <mutex>

//#include "OpenGLArea.h"
//#include "OpenglSmartRenderer.h"
//#include "OpenglSmartRenderer2.h"
#include "OpenglSmartRenderer3.h"
#include "SimpleRenderer.h"

/* 
class GLWindow : public Gtk::Window
{
  public:
	GLWindow()
	{
		vbox = new Gtk::VBox;
		drawing_area = new OpenglSmartRenderer2();

		GtkDrawingArea *gDrawingArea = drawing_area->gobj();
		GtkWidget *gWidget = GTK_WIDGET(gDrawingArea);
		vbox->pack_start(*drawing_area, true, true);
		add(*vbox);
	}

  private:
	Gtk::Button *button;
	Gtk::VBox *vbox;
	OpenglSmartRenderer2 *drawing_area;
};
*/
int main(int argc,  char** argv)
{
	//Gtk::Main kit;
	auto app = Gtk::Application::create(argc, argv, "");
    
	
	//TODO (VERY IMPORTANT): when Windows is created, it searches for "cam1" in Singleton. 
	//It must be already setted. I must find a way to not cause problems if it's not setted yet.
	//GLWindow window;
	//window.show_all();
	//OpenglSmartRenderer3 openglSmartRenderer3;
	SimpleRenderer simpleRenderer;
	//return app->run(openglSmartRenderer3);
	return app->run(simpleRenderer);
	//kit.run(window);
}
