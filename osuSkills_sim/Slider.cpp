#include "Slider.h"
#include "mathUtils.h"

Slider::Slider(int _x, int _y, double _width, double _val)
{
	x = _x;
	y = _y;
	width = _width;

	automate = false;
	autoDir = FORWARD;
	state = IDLE;

	if (_val == 0.0)
	{
		max = 1.0;
		min = 0.0;
		val = 0.0;
	}
	else
	{
		min = _val;
		max = _val + 1;
		val = _val;
	}
		
}

void Slider::Draw(Window &_win)
{
	Update(_win);

	// draw text
	_win.font->draw(core::stringw((int)min), rect<s32>(x - 10, y, x + 100, y + 10), SLIDER_COLOR);
	_win.font->draw(core::stringw(val), rect<s32>(Val2pos(val), y - 15, x + 100, y + 10), SLIDER_COLOR);
	_win.font->draw(core::stringw((int)max), rect<s32>(x + width + 10, y, x + 100, y + 10), SLIDER_COLOR);

	_win.driver->draw2DLine(position2d<s32>(x, y), position2d<s32>(x + width, y), SLIDER_COLOR);

	for (int radius = 0; radius < 5; radius++)
	{
		for (double i = 0; i < 2 * M_PI; i += PX_PER_RAD(radius))
		{
			_win.driver->drawPixel(Val2pos(val) + cos(i)*radius, y + sin(i)*radius, SLIDER_COLOR);
		}
	}
	// \TODO:  Implement render: slider and text
}

void Slider::setRange(double _min, double _max)
{
	min = _min;
	max = _max;

		 if (val < _min)	val = _min;
	else if (val > _max)	val = _max;
}

void Slider::setWidth(double _width)
{
	width = _width;
}

double Slider::getVal()
{
	return val;
}

void Slider::setVal(double _val)
{
	val = _val;
}

void Slider::setAuto(bool _auto, AUTO_MODE _autoMode)
{
	automate = _auto;
	autoMode = _autoMode;
}

double Slider::Pos2percent(int _pos)
{
	return (_pos - x) / width;
}

double Slider::Pos2val(int _pos)
{
	return (max - min)*Pos2percent(_pos) + min;
}

int Slider::Percent2pos(double _percent)
{
	return (_percent * width) + x;
}

int Slider::Val2pos(double _val)
{
	return Percent2pos((_val - min) / (max - min));
}

void Slider::UpdateAuto()
{
	switch (autoMode)
	{
		case LOOP:
			switch (autoDir)
			{
				case FORWARD:
					if (val > max)
						val = min;
					else
						val++;
					break;

				case BACKWARD:
					if (val < min)
						val = max;
					else
						val--;
					break;

				default:
					// ???
					break;
			};
		
			break;

		case REVERSE:
			switch (autoDir)
			{
				case FORWARD:
					if (val > max)
					{
						val--;
						autoDir = BACKWARD;
					}
					else
					{
						val++;
					}
					break;
				
				case BACKWARD:
					if (val < min)
					{
						val++;
						autoDir = FORWARD;
					}
					else
					{
						val--;
					}
					break;
				default:
					// ???
					break;
			}
			break;

		default:
			// ???
			break;
	}
}

void Slider::IdleLogic(Window &_win)
{
	position2di pos = _win.reciever.GetMouseState().positionCurr;
	bool cursorOverCircle = (BTWN(Val2pos(val) - SLIDER_POINT_RADIUS, pos.X, Val2pos(val) + SLIDER_POINT_RADIUS) &&
							 BTWN(y - SLIDER_POINT_RADIUS, pos.Y, y + SLIDER_POINT_RADIUS));
	bool leftEdge = _win.reciever.GetMouseState().LeftButtonEdge;

	if (cursorOverCircle && leftEdge)
	{
		state = HOLD;
	}
	else
	{
		if (automate == true)
		{
			UpdateAuto();
		}
	}

}

void Slider::HoldLogic(Window &_win)
{
	bool leftButton = _win.reciever.GetMouseState().LeftButtonDown;

	if (!leftButton)
	{
		state = IDLE;
	}
	else
	{
		double mouseVal = Pos2val(_win.reciever.GetMouseState().positionCurr.X);
		if (BTWN(min, mouseVal, max))
			val = mouseVal;
	}
}

void Slider::Update(Window &_win)
{
	switch (state)
	{
	case IDLE:
		IdleLogic(_win);
		break;

	case HOLD:
		HoldLogic(_win);
		break;

	default:
		// ???
		break;
	}
}