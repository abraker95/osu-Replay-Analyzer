#include "HitobjectHitTiming.h"
#include "../../irrlicht/include/irrlicht.h"

#include "../../utils/mathUtils.h"

HitObjectHitTiming::HitObjectHitTiming(int _xpos, int _ypos, int _width, int _height, double* _zoom, Window& _win) : GuiObj(_xpos, _ypos, _width, _height, 0)
{
	guiType = "Hitobject_hit_timing";
	zoom = _zoom;
}

HitObjectHitTiming::~HitObjectHitTiming()
{

}

void HitObjectHitTiming::UpdateInternal(Window& _win)
{
	switch (state)
	{
		case IDLE:
			IdleLogic(_win);
			break;

		case HIGHLIGHTED:
			HighlightedLogic(_win);
			break;

		default:
			// ???
			break;
	}
}

void HitObjectHitTiming::Draw(Window& _win)
{
	_win.driver->draw2DRectangle(SColor(255, 125, 0, 255), rect<s32>(absXpos, absYpos, absXpos + width, absYpos + height));

	double _90p = 0;

	for (double x = MIN(-100, -width/2.0); x < MAX(100, width/2.0); x++)
	{
		double y = height*(1.0 - Guassian(x, 40.0, true));
		_win.driver->drawPixel(absXpos + ((*zoom) * x), absYpos + y, irr::video::SColor(255, 255, 255, 255));
		
		if (_90p == 0)
			if (1.0 - (y / height) > 0.9) _90p = x;
	}

	_win.driver->draw2DRectangle(SColor(128, 0, 225, 0), rect<s32>(absXpos + (*zoom)*_90p, absYpos, absXpos - (*zoom)*_90p, absYpos + height));
}

void HitObjectHitTiming::IdleLogic(Window &_win)
{

}

void HitObjectHitTiming::HighlightedLogic(Window &_win)
{

}