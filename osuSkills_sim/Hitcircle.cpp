#include "Hitcircle.h"
#include "mathUtils.h"
#include "osuCalc.h"

#include <iostream>

int getHitcircleAt(std::vector<Hitcircle> _hitcircles, int _time)
{
	for (int i = 0; i < _hitcircles.size() - 1; i++)
	{
		if (BTWN(_hitcircles[i].getTime(), _time, _hitcircles[i + 1].getTime()))
			return i;
	}

	return -1;
}

// TODO: Figure out if this is giving correct results or not
int getNumVisibleAt(std::vector<Hitcircle> _hitcircles, int _index, double _AR, bool _hidden)
{
	int refTime = _hitcircles[_index].getTime();
	int count = 1;

	for (int i = _index + 1; i < _hitcircles.size(); i++)
	{
		if (!(_hitcircles[i].isVisible(refTime, _AR, _hidden)))
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
	if (this->isVisible(_time, _AR, _hidden))
	{
		if (_win.device->isWindowActive())
			update(_win);

		for (double i = 0; i < 2 * M_PI; i += step)
		{
			_win.driver->drawPixel(xOffset + cos(i)*radius, yOffset + sin(i)*radius, edgeCol);
		}

		for (int i = 1; i < sliders.size(); i++)
		{
			vector2di p1(std::get<XPOS>(sliders[i - 1]), std::get<YPOS>(sliders[i - 1]));
			vector2di p2(std::get<XPOS>(sliders[i]), std::get<YPOS>(sliders[i]));
			
			_win.driver->draw2DLine(p1, p2, edgeCol);
				
		}
	}
}

void Hitcircle::setCS_px(int _CS)
{
	radius = CS2px(_CS) / 2.0; // gives radius
}

bool Hitcircle::isVisible(int _time, double _AR, bool _hidden)
{
	// TODO: add a fade % threshold parameter
	if (_hidden)
		return BTWN(_time + AR2ms(_AR)*0.5, this->t, (double)_time + AR2ms(_AR));
	else
		return BTWN(_time, this->t, _time + AR2ms(_AR));
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