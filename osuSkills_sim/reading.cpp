#include "reading.h"
#include "analysis.h"
#include "osuCalc.h"
#include "geometry.h"

#include <iostream>
#include <tuple>

double Pattern2Reading(std::tuple<int, int, int, int> _p1, std::tuple<int, int, int, int> _p2, std::tuple<int, int, int, int> _p3, double _ARms, double _CSpx)
{
	std::cout << rad2deg(getDirAngle(position2di(std::get<Hitcircle::XPOS>(_p3), std::get<Hitcircle::YPOS>(_p3)), 
			   						 position2di(std::get<Hitcircle::XPOS>(_p2), std::get<Hitcircle::YPOS>(_p2)), 
									 position2di(std::get<Hitcircle::XPOS>(_p1), std::get<Hitcircle::YPOS>(_p1)))) << std::endl;

	return 0.0;
}

double getReadingSkill(std::vector<Hitcircle> &_hitcircles, int _time, double _CS, double _AR, bool _hidden)
{
	double reading = 0.0;
	int index = getHitcircleAt(_hitcircles, _time) + 1;

	if (index > _hitcircles.size() - 1) 	// if it's beyond the end
	{
		// \TODO
	}
	else if (index < 3) // if it's at or before the beggining
	{ 
		// \TODO
	}
	else
	{
		double timeSinceStart = 0;
		std::pair<int, int> visibilityTimes;
		std::vector<std::tuple<int, int, int, int>> pattern;

		if (_hitcircles[index - 1].isSlider())
			if (_time < _hitcircles[index - 1].getEndTime())
				index--;

		pattern = getPattern(_hitcircles, _time, index, _CS, 3);
		if (_hitcircles[index].isSlider())
			timeSinceStart = std::get<3>(pattern[2]);  // Time since started holding slider

		visibilityTimes = _hitcircles[0].getVisiblityTimes(_AR, _hidden, 0.1, 1.0);
		//double actualARTime = (_hitcircles[0].getTime() - visibilityTimes.first) + timeSinceStart;

		reading = Pattern2Reading(pattern[2], pattern[1], pattern[0], 0.0, CS2px(_CS) / 2.0);
	}

	return reading;
}