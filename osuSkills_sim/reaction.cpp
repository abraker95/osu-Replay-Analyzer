#include "reaction.h"
#include "Geometry.h"
#include "mathUtils.h"
#include "osuCalc.h"
#include "Lambert.h"
#include "analysis.h"

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


/*-------------------------------------------------*/


// Original model can be found at https://www.desmos.com/calculator/f2brlvcjmf
double Pattern2Reaction(std::tuple<int, int, int, int> _p1, std::tuple<int, int, int, int> _p2, std::tuple<int, int, int, int> _p3, double _ARms, double _CSpx)
{
	const double damping = 0.36;
	double curveSteepness = (300.0 / (_ARms + 250.)) * damping;
	double patReq = PatternReq(_p1, _p2, _p3, _CSpx);

	//std::cout << patReq << "   " << _ARms - _ARms*std::exp(-curveSteepness*patReq)<< "   " << _ARms << std::endl;

	return _ARms  - _ARms*std::exp(-curveSteepness*patReq) /*+ curveSteepness*sqrt(curveSteepness*patReq)*/;
}

double getReactionSkill(std::vector<Hitcircle> &_hitcircles, int _time, double _CS, double _AR, bool _hidden)
{
	double timeToReact = 0.0;
	int index = getHitcircleAt(_hitcircles, _time) + 1;

	if (index > _hitcircles.size() - 1) 	// if it's beyond the end
	{
		timeToReact = 0;
	}
	else if (index < 3) // if it's at or before the beggining
	{
		std::pair<int, int> visibilityTimes = _hitcircles[0].getVisiblityTimes(_AR, _hidden, 0.1, 1.0);
		timeToReact = _hitcircles[0].getTime() - visibilityTimes.first;
	}
	else
	{
		double timeSinceStart = 0.0;
		double actualARTime = 0.0;
		std::pair<int, int> visibilityTimes;
		std::vector<std::tuple<int, int, int, int>> pattern;

		if (_hitcircles[index - 1].isSlider())
			if(_time < _hitcircles[index - 1].getEndTime())
				index--;

		pattern = getPattern(_hitcircles, _time, index, _CS, 3);
		if (_hitcircles[index].isSlider())
			timeSinceStart = std::get<3>(pattern[2]);  // Time since started holding slider
		
		visibilityTimes = _hitcircles[0].getVisiblityTimes(_AR, _hidden, 0.1, 1.0);
		actualARTime = (_hitcircles[0].getTime() - visibilityTimes.first) + timeSinceStart;
		timeToReact = Pattern2Reaction(pattern[2], pattern[1], pattern[0], actualARTime, CS2px(_CS)/2.0);
	}

	return react2Skill(timeToReact);
}

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

