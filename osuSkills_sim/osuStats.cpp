#include "osuStats.h"

#include "irrlicht/include/irrlicht.h"
#include "mathUtils.h"
#include "osuCalc.h"
#include "Geometry.h"
#include "reaction.h"
#include <iostream>

using namespace irr;
using namespace core;

int UpdateHitobjectTimings(Window &_win, std::vector<Hitcircle> &_hitcircles, int _i, int _xpos, int _ypos, int _width, int _height)
{
	position2di pos = _win.reciever.GetMouseState().Position;
	bool cursorOverObject = (BTWN(_xpos, pos.X, _xpos + _width) &&
							 BTWN(_ypos, pos.Y, _ypos + _height));
	
	// TODO: Allow hitObject timing minipulation

	return _xpos;
}

void drawRefreshRateTimings(Window &_win, int _xoffset, int _yoffset, std::vector<Hitcircle> &_hitcircles, int _shift, double _px_ms, double _FPS)
{
	double updateLatency = (1000.0 / _FPS);

	// simulate refresh rate
	double start = 0;
	double end = _hitcircles[_hitcircles.size() - 1].getTime() + updateLatency * 5;
	for (double t = start; t < end; t += updateLatency)
	{
		int xpos = t*_px_ms + _xoffset;
		int ypos = 0 + _yoffset;
		int width = 1;

		if (xpos < _shift)
			continue;
		else if (xpos - width >= (_shift + 811))
			break;

		_win.driver->draw2DRectangle(SColor(255, 0, 0, 255), rect<s32>(xpos - _shift, ypos, xpos + width - _shift, ypos + 20));
	}
}

void drawHitobjectTimings(Window &_win, int _xoffset, int _yoffset, std::vector<Hitcircle> &_hitcircles, int _shift, double _px_ms, double _FPS)
{
	double updateLatency = (1000.0 / _FPS);

	// Show hitobject timings (when need to hit)
	for (int i = 0; i < _hitcircles.size(); i++)
	{
		// Code that snaps timings to FPS
			//double t1 = ((double)_hitcircles[i].getTime() / updateLatency);
			//double time = ceil(t1)*updateLatency;


		int time = _hitcircles[i].getTime();
		int xpos = time*_px_ms;
		int ypos = 0 + _yoffset + _xoffset;
		int width = updateLatency*_px_ms;

		if (xpos < _shift)
			continue;
		else if (xpos - width >= (_shift + 811))
			break;

		xpos -= _shift;

		_win.driver->draw2DRectangle(SColor(255, 175, 0, 175), rect<s32>(xpos, ypos, xpos + width, ypos + 5));
		_win.driver->draw2DRectangle(SColor(255, 255, 0, 255), rect<s32>(xpos, ypos, xpos + 1 , ypos + 5));
	}
}

void drawHumanReactionTimings(Window &_win, int _xoffset, int _yoffset, std::vector<Hitcircle> &_hitcircles, int _shift, double _px_ms, double _FPS, double _AR)
{
	const int HUMAN_REACTION = 240; // ms

	// Show human reaction timings
	for (double i = 0; i < _hitcircles.size() - 1; i++)
	{
		double time = _hitcircles[i + 1].getTime() - AR2ms(_AR);
		int xpos = time * _px_ms + _xoffset;
		int ypos = 0 + _yoffset;
		int width = HUMAN_REACTION*_px_ms;

		if (xpos < _shift - AR2ms(_AR) * _px_ms)
			continue;
		else if (xpos - width >= (_shift + 811))
			break;

		_win.driver->draw2DRectangle(SColor(255, 0, 255, 0), rect<s32>(xpos - _shift, ypos+3, xpos + width - _shift, ypos+8));
	}
}

void drawEyeLatencyTimings(Window &_win, int _xoffset, int _yoffset, std::vector<Hitcircle> &_hitcircles, int _shift, double _px_ms, double _FPS, double _AR)
{
	const int HUMAN_REACTION = 240; // ms

	// Show eye latency timings
	for (double i = 1; i < _hitcircles.size() - 1; i++)
	{
		// assume that the player begins to look at the next note when the 
		// note is 2 times the CS distance away from the circle for now
		// assume that the player doesn't snap to note for now
		double distPrev = getDist(_hitcircles[i].getPos(), _hitcircles[i - 1].getPos());
		double timePrev = _hitcircles[i].getTime() - _hitcircles[i - 1].getTime();
		double vel = distPrev / timePrev;

		double timeCertainty = _hitcircles[i - 1].getTime();
		if (vel != 0.0)
			timeCertainty += MAX(distPrev - _hitcircles[i].getSizePx()*2.0, 0.0) / vel;

		// Assume player looks at the next note as soon as the player percieves it
		/// \TODO: This needs fixing when dealing with more than 1 note
		double timeNextVisible = _hitcircles[i + 1].getTime() - AR2ms(_AR) + HUMAN_REACTION;

		double time = 0.0;
		if (timeCertainty < timeNextVisible)
			time = timeNextVisible;
		else
			time = timeCertainty;

		int xpos = time * _px_ms + _xoffset;
		int ypos = 0 + _yoffset;
		double distNext = getDist(_hitcircles[i].getPos(), _hitcircles[i + 1].getPos());
		int width = EyeTime(px2Deg(distNext, 640, 50.0))*_px_ms;

		if (xpos < _shift - AR2ms(_AR)*_px_ms)
			continue;
		else if (xpos - width >= (_shift + 811))
			break;

		_win.driver->draw2DRectangle(SColor(255, 0, 155, 0), rect<s32>(xpos - _shift, ypos + 3, xpos + width - _shift, ypos+8));
	}
}

void drawVisibliltyTimings(Window &_win, int _xoffset, int _yoffset, std::vector<Hitcircle> &_hitcircles, int _shift, double _px_ms, double _FPS, double _AR, bool _hidden)
{
	double updateLatency = (1000.0 / _FPS);
	
	// Show hitobject visibility
	int layer = 1;
	
	/*double ARend = AR2ms(_AR);
	if (_hidden)
		ARend *= 0.5;*/

	for (double i = 0; i < _hitcircles.size(); i++)
	{
		std::pair<int, int> visibilityTimes = _hitcircles[i].getVisiblityTimes(_AR, _hidden, 0.1, 0.7);

		int time = visibilityTimes.first;
		int xpos = time * _px_ms + _xoffset;
		int ypos = 0 + _yoffset;
		int width = MAX((visibilityTimes.second - visibilityTimes.first) + (_hitcircles[i].getEndTime() - _hitcircles[i].getTime()), updateLatency)*_px_ms;

		if (xpos < _shift - visibilityTimes.first*_px_ms) // if it goes out the left side of the visible area
			continue;
		else if (xpos - width >= (_shift + 811)) // if it goes out the right side of the visible area
			break;

		// \TODO: Account for prev notes being visible when the following notes appear0
		if (layer <= 1)
			layer = MAX(getNumVisibleAt(_hitcircles, visibilityTimes.first, _AR, _hidden, 0.1) + 1,
						getNumVisibleAt(_hitcircles, visibilityTimes.second, _AR, _hidden, 0.1) + 1);

		_win.driver->draw2DRectangle(SColor(255, 255, 150, 255), rect<s32>(xpos - _shift, ypos + 2 - 5*layer, xpos + width - _shift, ypos + 5 - 5*layer));	
		layer--;
	}
}

void drawCurrentTiming(Window &_win, int _xoffset, int _yoffset, std::vector<Hitcircle> &_hitcircles, int _shift, double _px_ms, double _FPS, double _time)
{
	// show current time
	{
		int xpos = _time*_px_ms + _xoffset;
		int ypos = 0 + _yoffset;
		int width = _px_ms;
		_win.driver->draw2DRectangle(SColor(255, 255, 255, 255), rect<s32>(xpos - _shift, ypos, MAX(xpos + width, xpos + 1.0) - _shift, ypos+20));
	}
}


void drawTimingGraphs(Window &_win, int _xoffset, int _yoffset, std::vector<Hitcircle> &_hitcircles, bool _relative, int _time, double _res, double _CS, double _AR, bool _hidden)
{
	const double SCREEN_WIDTH = 740.0;
	const double FPS = 60.0;
	double px_ms = _res; //  ms/px ???

	double shift = 0.0;
	if (_relative)
		shift = MAX(_time*px_ms - (SCREEN_WIDTH / 2), 0.0);
		
	drawRefreshRateTimings(_win, _xoffset, _yoffset, _hitcircles, shift, px_ms, FPS);
	drawHitobjectTimings(_win, _xoffset, _yoffset, _hitcircles, shift, px_ms, FPS);
	//drawHumanReactionTimings(_win, _xoffset, _yoffset, _hitcircles, shift, px_ms, FPS, _AR);
	//drawEyeLatencyTimings(_win, _xoffset, _yoffset, _hitcircles, shift, px_ms, FPS, _AR);
	drawVisibliltyTimings(_win, _xoffset, _yoffset, _hitcircles, shift, px_ms, FPS, _AR, _hidden);
	drawCurrentTiming(_win, _xoffset, _yoffset, _hitcircles, shift, px_ms, FPS, _time);
	
	// TODO: simulate reaction time latency

	// TODO: Model player's cursor movement given parameters and time those

	// TODO: simulate button press/release timing + instability
}