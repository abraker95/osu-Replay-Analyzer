#include "Slider.h"
#include "../utils/mathUtils.h"

Slider::Slider(int _x, int _y, double _width, double _val) : GuiObj(_x, _y, _width, 10)
{
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
	return ((double)_pos - (double)absXpos) / (double)width;
}

double Slider::Pos2val(int _pos)
{
	return (max - min)*Pos2percent(_pos) + min;
}

int Slider::Percent2pos(double _percent)
{
	return (_percent * width) + absXpos;
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
	bool leftEdge = _win.reciever.GetMouseState().LeftButtonEdge;
	if (this->isMouseOnObj(_win) && leftEdge)
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

void Slider::Draw(Window &_win)
{
	// draw text
	_win.font->draw(core::stringw((int)min), rect<s32>(absXpos - 10, absYpos, absXpos + 100, absYpos + 10), SLIDER_COLOR);
	_win.font->draw(core::stringw(val), rect<s32>(Val2pos(val), absYpos - 15, absXpos + 100, absYpos + 10), SLIDER_COLOR);
	_win.font->draw(core::stringw((int)max), rect<s32>(absXpos + width + 10, absYpos, absXpos + 100, absYpos + 10), SLIDER_COLOR);

	_win.driver->draw2DLine(position2d<s32>(absXpos, this->getMid().Y), position2d<s32>(absXpos + width, this->getMid().Y), SLIDER_COLOR);

	for (int radius = 0; radius < 5; radius++)
	{
		for (double i = 0; i < 2 * M_PI; i += PX_PER_RAD(radius))
		{
			_win.driver->drawPixel(Val2pos(val) + cos(i)*radius, this->getMid().Y + sin(i)*radius, SLIDER_COLOR);
		}
	}
	// \TODO:  Implement render: text
}

void Slider::UpdateInternal(Window &_win)
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