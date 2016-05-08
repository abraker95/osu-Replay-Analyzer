#include "precision.h"
#include "analysis.h"
#include "Geometry.h"
#include "osuCalc.h"

#include <iostream>

double pattern2skill(std::vector<std::tuple<int, int, int, int>>& _pattern, double _CS)
{
	double dist = getDist(position2di(std::get<Hitcircle::XPOS>(_pattern[0]), std::get<Hitcircle::YPOS>(_pattern[0])),
				   	      position2di(std::get<Hitcircle::XPOS>(_pattern[1]), std::get<Hitcircle::YPOS>(_pattern[1])));
	double velocity = dist / (abs(std::get<Hitcircle::TIME>(_pattern[1]) - std::get<Hitcircle::TIME>(_pattern[0])));
	double circleSize = CS2px(_CS);
	if (dist < circleSize)
		dist = 0.0;
	else if (dist == 0.0)
		dist = 1.0;
	else
		dist = 3.2/(0.1* circleSize - 1.4) * acos(circleSize/dist);
	
	velocity = 10*pow(velocity, 2);
	circleSize = 3000 / pow(circleSize, 2);

	return dist * velocity * circleSize;
}

double getPrecisionSkill(std::vector<Hitcircle> &_hitcircles, int _time, double _CS, double _AR, bool _hidden)
{
	double precision = 0;
	int index = getHitcircleAt(_hitcircles, _time) + 1;

	if (index > _hitcircles.size() - 1) 	// if it's beyond the end
	{
		precision = 0;
	}
	else if (index < 3) // if it's at or before the beggining
	{
		precision = 0;
	}
	else
	{
		std::vector<std::tuple<int, int, int, int>> pattern;
		double dist = 0;
		double velocity = 0;

		if (_hitcircles[index - 1].isSlider())
			if (_time < _hitcircles[index - 1].getEndTime())
				index--;

		pattern = getPattern(_hitcircles, _time, index, _CS, 2);		
		precision = 1.25 * pattern2skill(pattern, _CS);

		std::cout << precision << std::endl;
	}

	return precision;
}