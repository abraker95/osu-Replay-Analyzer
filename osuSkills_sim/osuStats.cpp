#include "osuStats.h"

#include "irrlicht/include/irrlicht.h"
#include "mathUtils.h"
#include "osuCalc.h"
#include "Geometry.h"
#include "SkillsCalc.h"

using namespace irr;
using namespace core;

bool isAtTick(double _currVal, double _prevVal, double _targetLatency)
{
//	double prevMultStep = floor(_prevVal / _targetLatency)*(_targetLatency / floor(_targetLatency));
//	double currMultStep = floor(_currVal / _targetLatency)*(_targetLatency / floor(_targetLatency));

	double prevMultStep = _prevVal / floor(_targetLatency);
	double currMultStep = _currVal / floor(_targetLatency);

	return (prevMultStep != currMultStep);
}

void drawRefreshRateTimings(Window &_win, int _xoffset, int _yoffset, std::vector<Hitcircle> &_hitcircles, int _shift, double _px_ms, double _FPS)
{
	double updateLatency = (1000.0 / _FPS);

	// simulate refresh rate
	for (double t = 0.0; t <_hitcircles[_hitcircles.size() - 1].getTime(); t += _px_ms)
	{
		double curr = (t + 0 * _px_ms) / updateLatency;
		double next = (t + 1 * _px_ms) / updateLatency;
		double resolution = ceil(next - curr);

		if (isAtTick(t, t - _px_ms, updateLatency))
		{
			int xpos = t*updateLatency + _xoffset;
			int ypos = 0 + _yoffset;
			int width = resolution;
			_win.driver->draw2DRectangle(SColor(255, 0, 0, 255), rect<s32>(xpos - _shift, ypos, xpos + resolution - _shift, ypos + 20));
		}
	}
}

void drawHitobjectTimings(Window &_win, int _xoffset, int _yoffset, std::vector<Hitcircle> &_hitcircles, int _shift, double _px_ms, double _FPS)
{
	double updateLatency = (1000.0 / _FPS);

	// Show hitobject timings (when need to hit)
	for (double i = 0; i < _hitcircles.size(); i++)
	{
		int xpos = _hitcircles[i].getTime() * _px_ms;
		int ypos = 0 + _yoffset + _xoffset;
		int width = updateLatency*_px_ms;
		_win.driver->draw2DRectangle(SColor(255, 255, 0, 255), rect<s32>(xpos - _shift, ypos, xpos + width - _shift, ypos + 5));
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
		_win.driver->draw2DRectangle(SColor(255, 0, 155, 0), rect<s32>(xpos - _shift, ypos + 3, xpos + width - _shift, ypos+8));
	}
}

void drawVisibliltyTimings(Window &_win, int _xoffset, int _yoffset, std::vector<Hitcircle> &_hitcircles, int _shift, double _px_ms, double _FPS, double _AR)
{
	double updateLatency = (1000.0 / _FPS);

	// Show hitobject visibility
	int layer = 1;
	for (double i = 0; i < _hitcircles.size(); i++)
	{
		int xpos = (_hitcircles[i].getTime() - AR2ms(_AR)) * _px_ms + _xoffset;
		int ypos = 0 + _yoffset;
		int width = MAX(AR2ms(_AR), updateLatency)*_px_ms;

		if (layer <= 1)
			layer = getNumVisibleAt(_hitcircles, i, _AR) + 1;

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


void drawTimingGraphs(Window &_win, int _xoffset, int _yoffset, std::vector<Hitcircle> &_hitcircles, bool _relative, int _time, double _res, double _CS, double _AR)
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
	drawEyeLatencyTimings(_win, _xoffset, _yoffset, _hitcircles, shift, px_ms, FPS, _AR);
	drawVisibliltyTimings(_win, _xoffset, _yoffset, _hitcircles, shift, px_ms, FPS, _AR);
	drawCurrentTiming(_win, _xoffset, _yoffset, _hitcircles, shift, px_ms, FPS, _time);
	
	// TODO: simulate reaction time latency

	// TODO: Model player's cursor movement given parameters and time those

	// TODO: simulate button press/release timing + instability
}