#include "precision.h"
#include "analysis.h"

#include "../../../utils/Geometry.h"
#include "../../../utils/mathUtils.h"
#include "../../osuCalc.h"

#include <iostream>

double pattern2skill(std::vector<std::tuple<int, int, int, int>>& _pattern, double _CS)
{
//	double dist = getDist(position2di(std::get<Hitcircle::XPOS>(_pattern[0]), std::get<Hitcircle::YPOS>(_pattern[0])),
//				   	      position2di(std::get<Hitcircle::XPOS>(_pattern[1]), std::get<Hitcircle::YPOS>(_pattern[1])));
	/*double velocity = dist / (abs(std::get<Hitcircle::TIME>(_pattern[1]) - std::get<Hitcircle::TIME>(_pattern[0])));
	double circleSize = CS2px(_CS);
	if (dist < circleSize)
		dist = 0.0;
	else if (dist == 0.0)
		dist = 0.0;
	else
		dist = 3.2/(0.1* circleSize - 1.4) * acos(circleSize/dist);
	
	velocity = 4.0*(exp(velocity) - 1);
	
	// v1
	//circleSize = 3000.0 / pow(circleSize, 1.25);

	//v2
	circleSize = 30000.0 / pow(circleSize, 2.0);

	return dist * velocity * circleSize;*/

//	double angleDist = atan(CS2px(_CS) / dist);
//	double time = abs(std::get<Hitcircle::TIME>(_pattern[1]) - std::get<Hitcircle::TIME>(_pattern[0]));

	//const double n = 4.1;
	//double c = pow(M_PI, n) / (pow(4, n) - 1);
	
	//angleDist = (c / pow(angleDist, n)) - (c / pow(M_PI, n));

	//angleDist = 6500*pow(angleDist, -1.8);
	//time = 0.13*(1.0-exp(-dist))*pow(time, -0.7);

	//return (CS2px(_CS)/(20*pow(time+500, 1.0)))*angleDist;

	//v10
	return 0; // 21428.57*(pow(_CS, 2)) / (pow(CS2px(_CS), 2) * 10 * log10(time) * pow(angleDist, 0.7));
}

double getPrecisionSkill(Beatmap* _beatmap, int _time)
{
	double precision = 0;
	/*int index = getHitcircleAt(_hitcircles, _time) + 1;

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
		precision = pattern2skill(pattern, _CS);

		std::cout << precision << std::endl;
	}*/

	return precision;
}


