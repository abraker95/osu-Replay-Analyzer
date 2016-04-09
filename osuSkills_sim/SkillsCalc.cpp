#include "SkillsCalc.h"
#include "Geometry.h"
#include "mathUtils.h"
#include "osuCalc.h"
#include "Lambert.h"
#include <iostream>

	double getThetaSum(std::vector<Hitcircle> &_hitcircles, int _ref, double _CS, double _AR, bool _hidden)
	{
		const double angleThreshold = deg2rad(px2Deg(3 * CS2px(_CS), 640, 60)); //= deg2rad(60);
		double thetaSum = 0.0;

		// can't do anthing angle based if it's the first or last circle
		if (_ref < 0 || _ref >= _hitcircles.size() - 1)
		{
			thetaSum = 0.1;
		}
		else
		{
			for (int i = _ref - 1; i < _hitcircles.size(); i++)
			{
				// make sure i is not the reference circle
				if (i == _ref) i++;

				// make sure we did go over bounds if we did skip ref
				if (i >= _hitcircles.size())
					break;

				// if i is not visible from the next hitcircle, then abort
				if (!_hitcircles[i].isVisible(_hitcircles[_ref].getTime(), _AR) && i > _ref)
					break;

				for (int j = i + 2; j < _hitcircles.size(); j++)
				{
					// if j is not visible from the next hitcircle, then abort
					if (!_hitcircles[j].isVisible(_hitcircles[_ref].getTime(), _AR))
						break;

					double theta = (M_PI - getAngle(_hitcircles[i].getPos(), _hitcircles[_ref].getPos(), _hitcircles[j].getPos()));
					//std::cout << theta << " ";
					thetaSum += theta / angleThreshold;
				}
			}
		}

		return thetaSum;
	}

double getDistSum(std::vector<Hitcircle> &_hitcircles, int _ref, double _CS, double _AR, bool _hidden)
{
	const double distThreshold = CS2px(_CS);
	double distSum = 0.0;

	for (int i = _ref; i < _hitcircles.size() - 1; i++)
	{
		// if i is not visible from the ref hitcircle, then abort
		if (!_hitcircles[i].isVisible(_hitcircles[_ref].getTime(), _AR) && i >_ref)
			break;

		double dist = getDist(_hitcircles[i].getPos(), _hitcircles[i + 1].getPos());
		distSum += dist / distThreshold;
	}

	return distSum;
}

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

double getTimeSum(std::vector<Hitcircle> &_hitcircles, int _ref, double _CS, double _AR, bool _hidden)
{
	//const double timeThreshold = 350; // ms
	const double fov = 60.0; // in deg
	double reqResponceTime = 0.0;

	if (_ref < 1)
	{
		reqResponceTime = AR2ms(_AR);
	}
	else
	{
		double dist = MAX(getDist(_hitcircles[_ref].getPos(), _hitcircles[_ref - 1].getPos()), 0.0);
		double eyeLatency = EyeTime(px2Deg(dist, 640.0, fov));

		// if the hitobject is not yet visible when the player is supposed to hit the note
		if (!_hitcircles[_ref - 1].isVisible(_hitcircles[_ref].getTime(), _AR, false))
		{

			reqResponceTime = AR2ms(_AR) - eyeLatency;
		}
		else
		{
			reqResponceTime = (_hitcircles[_ref].getTime() - _hitcircles[_ref - 1].getTime()) - eyeLatency;
		}

		//	const double PLAYER_MAX_VEL = 6.0;   // osu!px/ms
		//	double minTime = getDist(_hitcircles[_ref].pos, _hitcircles[_ref - 1].pos) / PLAYER_MAX_VEL;
		//	double remainderFocusTime = (_hitcircles[_ref].time - _hitcircles[_ref - 1].time) - minTime;

		//	timeSum = MIN(AR2ms(_AR) - timeSum, remainderFocusTime);


	}

	reqResponceTime = react2Skill(reqResponceTime);
	return reqResponceTime;
}

double CalcChaos(std::vector<Hitcircle> &_hitcircles, int _ref, double _CS, double _AR, bool _hidden)	
{
	double thetaSum = getThetaSum(_hitcircles, _ref, _CS, _AR, _hidden);
	double distSum = getDistSum(_hitcircles, _ref, _CS, _AR, _hidden);
	double timeSum = getTimeSum(_hitcircles, _ref, _CS, _AR, _hidden);

	if (thetaSum == 0.0 || thetaSum == INFINITY) thetaSum = 0.1;
	if (distSum == 0.0) distSum = 0.1;
	if (timeSum == 0.0) timeSum = 0.1;

	double chaos = thetaSum*0.1 * timeSum;

	//std::cout << "      " << thetaSum << std::endl;
	std::cout << "thetaSum: " << thetaSum << "  distSum: " << distSum << "  timeSum: " << timeSum << "   chaos: " << chaos << std::endl;
 
	return thetaSum;
}

// Eye latency is actually dependent factors which are yet to be simulated.
// Assume eye latatency is always true for now
void calc()
{
	/*
	double cs_px = CS2px(_CS);
	double dist = getDist(_hitcircles[0].getPos(), _hitcircles[1].getPos()) - cs_px;
	double percievedCSpx = AcuityPx(fov, dist, cs_px, 640.0);

	// for targeting. If the player can't hit the note because the note is too unfocused
	// in the player's periheral vision, then the player will move their eyes before hitting it
	if (getPrecisionAbility(player precision skill) < percievedCSpx)
		eyeLatency = EyeTime(px2Deg(dist, 640.0, fov));

	// for note order based clutter reading. The player will need to look through notes to find which
	// note to hit next
	if(getNoteOrderReadingAbility(player note order reading skill) < noteOrderChaos)
	{
		for(i -> allVisible)
			eyeLatency += EyeTime(px2Deg(dist, 640.0, fov));
	}
	*/
}