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

void DrawArc(Window &_win, int _xpos, int _ypos, double _radius, SColor _color, double _startAngle, double _radAngle)
{
	// step is such that the circle is drawn with minimal
	// amount of iterations while not showing any gaps
	for (double i = _startAngle; i < _startAngle + _radAngle; i += PX_PER_RAD(_radius))
		_win.driver->drawPixel(_xpos + cos(i)*_radius, _ypos + sin(i)*_radius, _color);
}

void DrawAngle(Window &_win, Hitcircle &_hc1, Hitcircle &_hc2, Hitcircle &_hc3)
{
	const int RADIUS = 10;
	double thetaRot;
	double angle = getAngle(_hc1.getPos(), _hc2.getPos(), _hc3.getPos());
	

	_win.driver->draw2DLine(_hc1.getEndPoint(), _hc2.getPos(), SColor(255, 255, 0, 0));
	_win.driver->draw2DLine(_hc2.getEndPoint(), _hc3.getPos(), SColor(255, 255, 0, 0));
	
	_win.font->draw(core::stringw(rad2deg(angle)),
		core::rect<s32>(_hc2.getPos().X + 10, _hc2.getPos().Y + 10, 100, 10),
		video::SColor(255, 255, 255, 255));

	if (getRotDir(_hc1.getPos(), _hc2.getPos(), _hc3.getPos()) == DIR::CW)
		thetaRot = getAbsoluteAngle(_hc2.getPos(), _hc3.getPos());
	else
		thetaRot = getAbsoluteAngle(_hc2.getPos(), _hc1.getPos()); 
	
	DrawArc(_win, _hc2.getPos().X, _hc2.getPos().Y, RADIUS, SColor(255, 255, 0, 0), thetaRot, angle);
}