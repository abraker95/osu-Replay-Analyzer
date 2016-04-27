#ifndef WINDOW_H
#define WINDOW_H

#include "irrlicht/include/irrlicht.h"
#include "Event.h"

using namespace irr;
using namespace core;
using namespace video;
using namespace gui;

// \TODO: Figure out why this is not working. Getting this to work
//		will allow to have a global ever reciever for all windows
//static EventReceiver reciever;

// \TODO: Figure out how to change window position while using the 
//		software renderer. This will allow to have a better workspace/view

class Window
{
	public:
		IrrlichtDevice *device;
		IVideoDriver* driver;
		IGUIEnvironment* guienv;
		IGUIFont* font;
		EventReceiver reciever;

		Window(int _width = 640, int _height = 480);
		dimension2di getDimentions();

	private:
		int width;
		int height;
};

#endif