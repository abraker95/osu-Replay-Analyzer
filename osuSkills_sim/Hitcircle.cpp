#include "Hitcircle.h"
#include "mathUtils.h"
#include "osuCalc.h"

#include <iostream>
#include <tuple>

// TODO: Use better search methods
int getHitcircleAt(std::vector<Hitcircle>& _hitcircles, int _time)
{
	for (int i = 0; i < _hitcircles.size() - 1; i++)
	{
		if (BTWN(_hitcircles[i].getTime(), _time, _hitcircles[i + 1].getTime()))
			return i;
	}

	return -1;
}

int getNumVisibleAt(std::vector<Hitcircle>& _hitcircles, int _time, double _AR, bool _hidden, double _opacity)
{
	int index = getHitcircleAt(_hitcircles, _time);
	int count = 1;

	for (int i = index + 1; i < _hitcircles.size(); i++)
	{
		std::pair<int, int> visibilityTimes = _hitcircles[i].getVisiblityTimes(_AR, _hidden, _opacity, _opacity);
		if (!BTWN(visibilityTimes.first, _time, visibilityTimes.second))
			break;

		count++;
	}
	
	return count;
}

Hitcircle::Hitcircle(int _x, int _y, int _t, std::vector<std::tuple<int, int, int>> _sliders)
{
	x = _x;
	y = _y;
	t = _t;
	sliders = _sliders;

	radius = 100; // radius
	state = IDLE;
	edgeCol = IDLE_COLOR;
}

void Hitcircle::Draw(Window &_win, int _xOffset, int _yOffset, int _time, double _AR, double _CS, bool _hidden)
{
	// step is such that the circle is drawn with minimal
	// amount of iterations while not showing any gaps
	this->setCS_px(_CS);
	this->xOffset = this->x + _xOffset;
	this->yOffset = this->y + _yOffset;

	double step = PX_PER_RAD(radius);
	bool slider = (this->sliders.size() != 0);
		
	if (this->isVisible(_time, _AR, _hidden))
	{
		if (_win.device->isWindowActive())
			update(_win);

		double opacity = this->getOpacity(_time, _AR, _hidden);
		SColor fade = SColor(255, edgeCol.getRed() * opacity, edgeCol.getGreen() * opacity, edgeCol.getBlue() * opacity);

		for (double i = 0; i < 2 * M_PI; i += step)
		{
			
			_win.driver->drawPixel(xOffset + cos(i)*radius, yOffset + sin(i)*radius, fade);
		}

		for (int i = 1; i < sliders.size(); i++)
		{
			vector2di slidePoint(std::get<XPOS>(sliders[i]), std::get<YPOS>(sliders[i]));
			_win.driver->drawPixel(slidePoint.X, slidePoint.Y, fade);

			if (BTWN(std::get<TIME>(sliders[i - 1]), _time, std::get<TIME>(sliders[i])))
			{
				// draw slider point
				for (double i = 0; i < 2 * M_PI; i += step)
				{
					_win.driver->drawPixel(slidePoint.X + cos(i) * 5, slidePoint.Y + sin(i) * 5, fade);
				}
			}
		}
	}
}

void Hitcircle::setCS_px(int _CS)
{
	radius = CS2px(_CS) / 2.0; // gives radius
}

bool Hitcircle::isVisible(int _time, double _AR, bool _hidden)
{
	double opacity = getOpacity(_time, _AR, _hidden);
	if (opacity > 0.0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

double Hitcircle::getOpacity(int _time, double _AR, bool _hidden)
{
	double preampTime = this->t - AR2ms(_AR);	// Time when the AR goes into effect

	if (_hidden)
	{ 
		double fadeinDuration = 0.4*AR2ms(_AR);				// how long the fadein period is
		double fadeinTimeEnd = preampTime + fadeinDuration; // When it is fully faded in
		
		// Fadein period always lasts from preamp time to 40% from preamp time to hit time
		double percentFadeIn = getPercent(preampTime, _time, fadeinTimeEnd);

		// If it's not fully faded in, then we haven't gotten up to the later stuff 
		if (percentFadeIn < 1.0)
		{
			return percentFadeIn;
		}
		else // fadeout
		{
			// If it's a slider, then the fade out period lasts from when it's fadedin to
			// 70% to the time it the slider ends
			if (this->isSlider())
			{
				double fadeoutDuration = (this->getEndTime() - fadeinTimeEnd); // how long the fadeout period is
				double fadeoutTimeEnd = fadeinTimeEnd + fadeoutDuration;		   // When it is fully faded out
				return (1.0 - getPercent(fadeinTimeEnd, _time, fadeoutTimeEnd));
			}
			else
			{
				double fadeoutDuration = 0.7*(this->t - fadeinTimeEnd);		// how long the fadeout period is
				double fadeoutTimeEnd = fadeinTimeEnd + fadeoutDuration;	// When it is fully faded out
				return (1.0 - getPercent(fadeinTimeEnd, _time, fadeoutTimeEnd));
			}
		}
	}
	else
	{
		double fadeinDuration = MIN(AR2ms(_AR), 400);		// how long the fadein period is
		double fadeinTimeEnd = preampTime + fadeinDuration; // When it is fully faded in

		// Fadein period always lasts from preamp time to 400 ms after preamp time or
		// when the object needs to be hit, which ever is smaller
		double percentFadeIn = getPercent(preampTime, _time, fadeinTimeEnd);

		// If it's not fully faded in, then we haven't gotten up to the later stuff 
		if (percentFadeIn < 1.0)
		{
			return percentFadeIn;
		}
		else // fadeout
		{
			// If it is during the slider hold period, then it's fully visible.
			// Otherwise, it's not visible anymore.
			if (this->isSlider())
			{
				if (_time > this->getEndTime())
				{
					// \TODO: Opacity when pressed and when missed
					return 0.0; 
				}
				else
				{
					return 1.0;
				}
			}
			else
			{
				if (_time > this->t)
				{
					// \TODO: Opacity when pressed and when missed
					return 0.0;
				}
				else
				{
					return 1.0;
				}
			}
		}
	}
}

std::pair<int, int> Hitcircle::getVisiblityTimes(double _AR, bool _hidden, double _opacityStart, double _opacityEnd)
{
	double preampTime = this->t - AR2ms(_AR);	// Time when the AR goes into effect
	std::pair<int, int> times;

	if (_hidden)
	{
		double fadeinDuration = 0.4*AR2ms(_AR);				// how long the fadein period is
		double fadeinTimeEnd = preampTime + fadeinDuration; // When it is fully faded in

		times.first = getValue(preampTime, fadeinTimeEnd, _opacityStart);
		
		
		// If it's a slider, then the fade out period lasts from when it's fadedin to
		// 70% to the time it the slider ends
		if (this->isSlider())
		{
			double fadeoutDuration = (this->getEndTime() - fadeinTimeEnd); // how long the fadeout period is
			double fadeoutTimeEnd = fadeinTimeEnd + fadeoutDuration;		   // When it is fully faded out
			times.second = getValue(fadeinTimeEnd, fadeoutTimeEnd, 1.0 - _opacityEnd);

			return times;
		}
		else
		{
			double fadeoutDuration = 0.7*(this->t - fadeinTimeEnd);		// how long the fadeout period is
			double fadeoutTimeEnd = fadeinTimeEnd + fadeoutDuration;	// When it is fully faded out
			times.second = getValue(fadeinTimeEnd, fadeoutTimeEnd, 1.0 - _opacityStart); // <-- no this is not a mistake :D

			return times;
		}
	}
	else
	{
		double fadeinDuration = MIN(AR2ms(_AR), 400);		// how long the fadein period is
		double fadeinTimeEnd = preampTime + fadeinDuration; // When it is fully faded in

		// Fadein period always lasts from preamp time to 400 ms after preamp time or
		// when the object needs to be hit, which ever is smaller
		times.first = getValue(preampTime, fadeinTimeEnd, _opacityStart);

		// If it is during the slider hold period, then it's fully visible.
		// Otherwise, it's not visible anymore.
		if (this->isSlider())
		{
			times.second = this->getEndTime();
			return times;
		}
		else
		{
			times.second = this->t;
			return times;
		}
	}
}

position2di Hitcircle::getPos(bool _absolute)
{
	if (_absolute)
		return position2di(this->xOffset, this->yOffset);
	else
		return position2di(this->x, this->y);
}

int Hitcircle::getTime()
{
	return this->t;
}

// return diameter osu osu!px
double Hitcircle::getSizePx()
{
	return this->radius*2.0;
}

void Hitcircle::setTime(int _time)
{
	this->t = _time;
}

bool Hitcircle::isSlider()
{
	return (this->sliders.size() != 0);
}

position2di Hitcircle::getEndPoint()
{
	if (this->isSlider())
	{
		std::tuple<int, int, int> sliderPoint = this->sliders[this->sliders.size() - 1];
		return position2di(std::get<XPOS>(sliderPoint), std::get<YPOS>(sliderPoint));
	}
	else
	{
		return position2di(this->x, this->y);
	}
}


int Hitcircle::getEndTime()
{
	if (this->isSlider())
	{
		std::tuple<int, int, int> sliderPoint = this->sliders[this->sliders.size() - 1];
		return std::get<TIME>(sliderPoint);
	}
	else
	{
		return this->t;
	}
}

int Hitcircle::getHoldPeriod()
{
	return this->getEndTime() - this->getTime();
}

position2di Hitcircle::getSliderPointAt(int _index)
{
	if (_index < sliders.size())
		return position2di(std::get<XPOS>(sliders[_index]), std::get<YPOS>(sliders[_index]));
	else
		return position2di(-1, -1);
}

int Hitcircle::getSliderTimeAt(int _index)
{
	if (_index < sliders.size())
		return std::get<TIME>(sliders[_index]);
	else
		return -1;
}

// ------ [PRIVATE] ------

void Hitcircle::IdleLogic(Window &_win)
{
	position2di pos = _win.reciever.GetMouseState().Position;
	bool cursorOverCircle = (BTWN(this->xOffset - radius, pos.X, this->xOffset + radius) && 
		                     BTWN(this->yOffset - radius, pos.Y, this->yOffset + radius));

	//std::cout << pos.X << std::endl;

	if (cursorOverCircle)
		state = HIGHLIGHTED;
	else
		edgeCol = IDLE_COLOR;
}

void Hitcircle::HighlightedLogic(Window &_win)
{
	position2di pos = _win.reciever.GetMouseState().Position;
	bool cursorOverCircle = (BTWN(this->xOffset - radius, pos.X, this->xOffset + radius) &&
							 BTWN(this->yOffset - radius, pos.Y, this->yOffset + radius));

	if (!cursorOverCircle)
		state = IDLE;
	else
	{
		bool leftEdge = _win.reciever.GetMouseState().LeftButtonEdge;
		if (leftEdge)
		{
			state = SELECTED;
		}

		edgeCol = HIGHLIGHTED_COLOR;
	}
}

// A known problem is that the when circles overlap, selecting them
// forces them to collapse into the same position. Further interaction
// with the circles will result the same effect as if there is one circle
// To fix this, an environment manager is needed, but currently not planned.
void Hitcircle::SelectedLogic(Window &_win)
{
	position2di pos = _win.reciever.GetMouseState().Position;
	bool leftButton = _win.reciever.GetMouseState().LeftButtonDown;
	bool leftEdge = _win.reciever.GetMouseState().LeftButtonEdge;
	bool cursorOverCircle = (BTWN(this->xOffset - radius, pos.X, this->xOffset + radius) &&
						  	 BTWN(this->yOffset - radius, pos.Y, this->yOffset + radius));

	if (!cursorOverCircle && leftEdge)
		state = IDLE;
	else if (leftButton)
	{
		// has to be += due the fact that x - xoffset = 0, 
		// resolving the assignment to x = pos.x
		// this->x = pos.X - this->xOffset + this->x  => this->x = pos.X - 0;
		this->x += pos.X - this->xOffset;
		this->y += pos.Y - this->yOffset;

		edgeCol = SELECTED_COLOR;
	}
}

void Hitcircle::update(Window &_win)
{
	switch (state)
	{
		case IDLE:
			IdleLogic(_win);
			break;

		case HIGHLIGHTED:
			HighlightedLogic(_win);
			break;

		case SELECTED:
			SelectedLogic(_win);
			break;

		default:
			// ???
			break;
	}
}