#include "Window.h"

Window::Window(int _width , int _height)
{
	width = _width;
	height = _height;

	device = createDevice(video::EDT_SOFTWARE, dimension2d<u32>(width, height), 16, false, false, false, &reciever);
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
}

dimension2di Window::getDimentions()
{
	return dimension2di(this->width, this->height);
}