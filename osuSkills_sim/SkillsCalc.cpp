#include "SkillsCalc.h"
#include "Geometry.h"
#include "mathUtils.h"
#include "osuCalc.h"
#include "Lambert.h"

#include <iostream>
#include <assert.h>

double react2Skill(double _timeToReact)
{
	// Original model can be found at https://www.desmos.com/calculator/lg2jqyesnu
	double a = pow(2.0, log(78608.0 / 15625.0) / log(34.0 / 25.0))*pow(125.0, log(68.0 / 25.0) / log(34.0 / 25.0));
	double b = log(2.0) / (log(2.0) - 2.0*log(5.0) + log(17.0));
	return a / pow(_timeToReact, b);
}

double EyeTime(double _theta)
{
	// original model can be found at https://www.desmos.com/calculator/vjj0kgahvu
	return 12.5*pow(_theta, 1.0 / 2.2) + 5.2*pow(_theta, 1.0 / 1.63);
}

// returns acuity factor (0.0 -> 1.0)
// _theta in deg
double EyeAcuity(double _theta)
{
	// original model can be found at https://www.desmos.com/calculator/bki7e2vbiw
	return 8.0 / (7 + 1.7*_theta + exp(0.13 * _theta));
}

// fov in deg
/*double osupx2Deg(double _dist, double _fov)
{
	// original model can be found at: https://www.desmos.com/calculator/yyghbbcnmk
	// Inverse was determined by putting the following into wolfram:
	// (S*2*tan(F)*16*tan(0.0014544410433249999482)*554.2563)/(7*1.7)=x*e^(0.13*x),solve x
		// where S is _dist, F is _fov, and 0.0014544410433249999482 is 1/12 deg
	
/// \TODO: recheck osu!px -> deg aspect ratio wise... something is not matching up
// fov is based on the horizontal 640 osu!px in view of 30 deg. Factor in vertical fov as well (480 osu!px)

	return 7.69231 * LambertW<0>((418462512.0*_dist*tan(deg2rad(_fov))) / 1484920225.0);
}*/



// _fov: field of view of the play area in deg
// _dist: distance from the location the player is currently looking at in osu!px
// _cs_px: circle size in osu!px
double AcuityPx(double _fov, double _dist, double _cs_px, double _res)
{
	// if the edge of the circles are touching or overlaping, 
	// it would be equivalent to 0 osu!px distance
	double deg = px2Deg(MAX(_dist, 0.0), _res, _fov);

	// Factor of resolution loss 
	double acuity = EyeAcuity(deg);  

	double percievedCS = acuity * _cs_px;
	//double percievedCS2 = Deg2osupx(osupx2Deg(_cs_px, _res, _fov)*acuity, _res, _fov);

	//std::cout << "deg: " << deg << "    acuitySize: " << acuity << "    percCS1:  " << percievedCS << "    percCS2:  " << percievedCS2 << std::endl;

	return percievedCS;
}

double PatternReq(std::tuple<int, int, int, int> _p1, std::tuple<int, int, int, int> _p2, std::tuple<int, int, int, int> _p3)
{
	double dist_12 = getDist(position2di(std::get<Hitcircle::XPOS>(_p1), std::get<Hitcircle::YPOS>(_p1)), 
							 position2di(std::get<Hitcircle::XPOS>(_p2), std::get<Hitcircle::YPOS>(_p2)));
	double dist_23 = getDist(position2di(std::get<Hitcircle::XPOS>(_p2), std::get<Hitcircle::YPOS>(_p2)),
							 position2di(std::get<Hitcircle::XPOS>(_p3), std::get<Hitcircle::YPOS>(_p3)));
	double dist = dist_12 + dist_23;

	double angle = getAngle(position2di(std::get<Hitcircle::XPOS>(_p1), std::get<Hitcircle::YPOS>(_p1)),
							position2di(std::get<Hitcircle::XPOS>(_p2), std::get<Hitcircle::YPOS>(_p2)),
							position2di(std::get<Hitcircle::XPOS>(_p3), std::get<Hitcircle::YPOS>(_p3)));

	double time = std::get<Hitcircle::TIME>(_p3) - std::get<Hitcircle::TIME>(_p1);

	return time / (dist*(M_PI - angle));
}

// Original model can be found at https://www.desmos.com/calculator/f2brlvcjmf
double Pattern2Reaction(std::tuple<int, int, int, int> _p1, std::tuple<int, int, int, int> _p2, std::tuple<int, int, int, int> _p3, double _ARms)
{
	const double damping = 25.0;
	double curveSteepness = (300.0 / _ARms) * damping;
	double patReq = PatternReq(_p1, _p2, _p3);

	return _ARms  - _ARms*std::exp(-curveSteepness*patReq) /*+ curveSteepness*sqrt(curveSteepness*patReq)*/;
}

std::tuple<int, int, int, int> getPointAt(Hitcircle &_hitcircle, int _index)
{
	position2di pos = _hitcircle.getPos();
	int time = _hitcircle.getTime();
	int timeSinceStart = 0;

	if (_hitcircle.isSlider())
	{
		pos = _hitcircle.getSliderPointAt(_index);
		time = _hitcircle.getSliderTimeAt(_index);
		timeSinceStart = time - _hitcircle.getTime();
	}
	
	return std::tuple<int, int, int, int>(pos.X, pos.Y, time, timeSinceStart);
}

std::vector<std::tuple<int, int, int, int>> getPattern(std::vector<Hitcircle> &_hitcircles, int _time, int _index, double _CS, int _quant = 3)
{
	Hitcircle &circle = _hitcircles[_index];
	std::vector<std::tuple<int, int, int, int>> points;

	// If it is a slider, then iterate through and find reaction points.
	if (circle.isSlider())
	{
		int i = 0, prev_i = i;
		int timeDelta = circle.getSliderTimeAt(1) - circle.getSliderTimeAt(0) + 1;
		int pointTime = 0;

		// find the last point before current time
		for (; pointTime < _time && pointTime + timeDelta < circle.getEndTime(); i++)
			pointTime = circle.getSliderTimeAt(i);
		
		// record the point
		points.push_back(getPointAt(circle, i--));
		prev_i = i;
		
		// get 2 points before the current point which are at least a circle size apart
		while (i >= 0 && points.size() < _quant)
		{
			int dist = 0;

			// find the next point that is at least a circle size apart
			while (dist < CS2px(_CS) && i >= 0 && points.size() < 3)
			{
				// \TODO: make this more accurate
				dist = getDist(circle.getSliderPointAt(prev_i), circle.getSliderPointAt(i--));
			}

			// record the point if the position is different
			position2di prevPos = position2di(std::get<Hitcircle::XPOS>(points[points.size() - 1]), std::get<Hitcircle::YPOS>(points[points.size() - 1]));
			position2di newPos = circle.getSliderPointAt(1 + i);
			if (getDist(prevPos, newPos) > CS2px(_CS))
				points.push_back(getPointAt(circle, 1 + i--));
		}
	}
	else
	{
		position2di pos = circle.getPos();
		points.push_back(std::tuple<int, int, int, int>(pos.X, pos.Y, circle.getTime(), 0));
	}

	// check if we have the required amount of points
	if (points.size() < _quant && _index > 0)
	{
		// if not, go to the previous hitobject
		std::vector<std::tuple<int, int, int, int>> pattern = getPattern(_hitcircles, _time, _index - 1, _CS, _quant - points.size());
		
		points.insert(points.end(),
			std::make_move_iterator(pattern.begin()),
			std::make_move_iterator(pattern.end()));		

		assert(points.size() != 0);
		return points;
	}
	else
	{
		assert(points.size() != 0);
		return points;
	}
}

double getReactionSkill(std::vector<Hitcircle> &_hitcircles, int _time, double _CS, double _AR, bool _hidden)
{
	double timeToReact = 0.0;
	int index = getHitcircleAt(_hitcircles, _time) + 1;

	if (index > _hitcircles.size() - 1)
	{
		timeToReact = 0;
	}
	else if (index < 2)
	{
		std::pair<int, int> visibilityTimes = _hitcircles[0].getVisiblityTimes(_AR, _hidden, 0.3, 1.0);
		timeToReact = _hitcircles[0].getTime() - visibilityTimes.first;
	}
	else
	{
		std::vector<std::tuple<int, int, int, int>> pattern = getPattern(_hitcircles, _time, index, _CS, 3);
		double timeSinceStart = std::get<3>(pattern[2]);  // Time since started holding slider
		
		std::pair<int, int> visibilityTimes = _hitcircles[0].getVisiblityTimes(_AR, _hidden, 0.3, 1.0);
		double actualARTime = (_hitcircles[0].getTime() - visibilityTimes.first) + timeSinceStart;

		timeToReact = Pattern2Reaction(pattern[2], pattern[1], pattern[0], actualARTime);
	}

	return react2Skill(timeToReact);
}

