#include "Window.h"
#include <chrono>

Window::Window(int _width , int _height)
{
	initOsWindow();

	device = createDevice(video::EDT_OPENGL, dimension2d<u32>(_width, _height), 16, false, false, false, &reciever);
		if (!device)
		{
			// \TODO: Print cause of error
			exit(1);
		}

	driver = device->getVideoDriver();
		if (!driver)
		{
			// \TODO: Print cause of error
			device->drop();
	
			exit(1);
		}

		if (!driver->queryFeature(video::EVDF_RENDER_TO_TARGET))
		{
			// \TODO: Print cause of error
			device->drop();

			exit(1);
		}

	guienv = device->getGUIEnvironment();
		if (!guienv)
		{
			// \TODO: Print cause of error
			device->drop();

			exit(1);
		}

	font = device->getGUIEnvironment()->getBuiltInFont();
		if (!font)
		{
			// \TODO: Print cause of error
			device->drop();

			exit(1);
		}

	currMs = 0;
	prevMs = 0;
}

dimension2di Window::getDimentions()
{
	return dimension2di(driver->getScreenSize());
}

void Window::updateFPS()
{
	prevMs = currMs;
	currMs = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

double Window::getFPS() const
{
	return 1000.0/ (double)(currMs - prevMs);
}


//  ----------- [PRIVATE] ------------
void Window::initOsWindow()
{
	#ifdef _WIN32
		initWindow();
	#endif

	#ifdef LINUX
		initLinux();
	#endif

	#ifdef _MAC
		initMac();
	#endif
}

void Window::initWindow()
{

}

void Window::initLinux()
{

}

void Window::initMac()
{

}