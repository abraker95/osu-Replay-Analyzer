#include "drawUtils.h"
#include "Geometry.h"
#include "mathUtils.h"

void DrawLine(Window &_win, Hitcircle &_hc1, Hitcircle &_hc2)
{
	_win.driver->draw2DLine(_hc1.getPos(), _hc2.getPos(), SColor(255, 255, 0, 0));

	position2di midpoint = getMidpoint(_hc1.getPos(), _hc2.getPos());

	_win.font->draw(core::stringw(getDist(_hc1.getPos(), _hc2.getPos())),
		core::rect<s32>(midpoint.X, midpoint.Y, 100, 10),
		video::SColor(255, 255, 255, 255));

	// \TODO draw dist or velocity label
}

void DrawAngle(Window &_win, Hitcircle &_hc1, Hitcircle &_hc2, Hitcircle &_hc3)
{
	_win.driver->draw2DLine(_hc1.getEndPoint(), _hc2.getPos(), SColor(255, 255, 0, 0));
	_win.driver->draw2DLine(_hc2.getEndPoint(), _hc3.getPos(), SColor(255, 255, 0, 0));
	
	const int radius = 10;
	double angle = getAngle(_hc1.getPos(), _hc2.getPos(), _hc3.getPos());
	
	_win.font->draw(core::stringw(rad2deg(angle)),
		core::rect<s32>(_hc2.getPos().X, _hc2.getPos().Y, 100, 10),
		video::SColor(255, 255, 255, 255));


	/// \TODO: Properly draw the fucking arc
	/*
	double theta = addAngle(getAbsoluteAngle(_hc2.getPos(), _hc1.getPos()), -getAbsoluteAngle(_hc2.getPos(), _hc3.getPos()));
	bool side = theta > M_PI;

	double rot;
	if (!side)
	{
		rot = getAngle(_hc3.getPos(), _hc2.getPos(), _hc2.getPos() + position2di(1, 0));
		for (double i = rot; i < rot + angle; i += PX_PER_RAD(radius))
			_win.driver->drawPixel(_hc2.getPos().X + cos(i)*radius, _hc2.getPos().Y + sin(i)*radius, SColor(255, 255, 0, 0));
	}	
	else
	{
		rot = getAngle(_hc3.getPos(), _hc2.getPos(), _hc2.getPos() + position2di(1, 0));
		for (double i = rot; i > rot - angle; i -= PX_PER_RAD(radius))
			_win.driver->drawPixel(_hc2.getPos().X + cos(i)*radius, _hc2.getPos().Y + sin(i)*radius, SColor(255, 255, 0, 0));
	}*/
}