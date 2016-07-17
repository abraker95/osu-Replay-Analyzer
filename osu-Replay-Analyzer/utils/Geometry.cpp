#include "Geometry.h" 
#include "mathUtils.h"

#include <math.h>
#include "../irrlicht/include/irrlicht.h"
#include "../osu!/osuCalc.h"

double getSlope(position2d<double> _p1, position2d<double> _p2)
{
	if ((_p2.X - _p1.X) == 0)
		return DBL_MAX;
	else
		return ((double)(_p2.Y - _p1.Y)) / ((double)(_p2.X - _p1.X));
}

double getDist(position2d<double> _p1, position2d<double> _p2)
{
	double distX = _p2.X - _p1.X;
	double distY = _p2.Y - _p1.Y;
	return sqrt((double)(distX*distX + distY*distY));
}

bool isPointOnLine(position2d<double> _point, std::pair<position2d<double>, position2d<double>> _line)
{
	position2d<double> checkPoint = position2d<double>(_line.first.X, _point.Y);
	
	// Just make sure the point has the same slope going to any of the other points on the line and
	// it's between the 2 points of the line
	if (getSlope(_line.first, checkPoint) != getSlope(_line.first, _line.second))
	{
		return false;
	}
	else if (!(BTWN(_line.first.X, _point.X, _line.second.X) && BTWN(_line.first.Y, _point.Y, _line.second.Y)))
	{
		return false;
	}
	else
	{
		return true;
	}

}

// Thanks http://www.softwareandfinance.com/Visual_CPP/VCPP_Intersection_Two_lines_EndPoints.html
bool HasIntersectionPoint(std::pair<position2d<double>, position2d<double>> _line1, std::pair<position2d<double>, position2d<double>> _line2)
{
	bool l1_checkX1 = BTWN(_line2.first.X, _line1.first.X, _line2.second.X);
	bool l1_checkX2 = BTWN(_line2.first.X, _line1.second.X, _line2.second.X);
	bool l1_checkY1 = BTWN(_line2.first.Y, _line1.first.Y, _line2.second.Y);
	bool l1_checkY2 = BTWN(_line2.first.Y, _line1.second.Y, _line2.second.Y);

	bool l2_checkX1 = BTWN(_line1.first.X, _line2.first.X, _line1.second.X);
	bool l2_checkX2 = BTWN(_line1.first.X, _line2.second.X, _line1.second.X);
	bool l2_checkY1 = BTWN(_line1.first.Y, _line2.first.Y, _line1.second.Y);
	bool l2_checkY2 = BTWN(_line1.first.Y, _line2.second.Y, _line1.second.Y);

	bool result = ( l2_checkX1 && l2_checkX2 && l1_checkY1 && l1_checkY2) ||
				  ( l1_checkX1 && l1_checkX2 && l2_checkY1 && l2_checkY2); 

	// make sure it's not detecting connected points
	result &= (_line1.second != _line2.first) && (_line1.first != _line2.second);
	
	return result;


}

double px2Deg(double _px, double _res, double _fov)
{
	return rad2deg(2.0 * tan(_px / _res * atan(deg2rad(_fov) / 2.0)));
}

double Deg2px(double _deg, double _res, double _fov)
{
	return _res * (atan(deg2rad(_deg) / 2.0) / atan(deg2rad(_fov) / 2.0));
}

double getVelocity(position2d<double> _p1, position2d<double> _p2, double _t1, double _t2)
{
	double dist = getDist(_p1, _p2);
	double time = ABS(_t2 - _t1);
	return dist / time;
}

// Positive is counter-clock wise and negative is clock-wise
// Returns values in radians between -pi and +pi
double getDirAngle(position2d<double> a, position2d<double> b, position2d<double> c)
{
	vector2df ab(b.X - a.X, b.Y - a.Y);
	vector2df cb(b.X - c.X, b.Y - c.Y);

	double dot = (ab.X * cb.X + ab.Y * cb.Y); // dot product
	double cross = (ab.X * cb.Y - ab.Y * cb.X); // cross product

	return atan2(cross, dot);;
}

// Returns 3 points make in radians between 0 and pi
double getAngle(position2d<double> _p1, position2d<double> _ref, position2d<double> _p2)
{
	return abs(getDirAngle(_p1, _ref, _p2));
}

/// \TODO: check what happends if point are on top of each other
DIR getRotDir(position2d<double> a, position2d<double> b, position2d<double> c)
{
	return (DIR)(getDirAngle(a, b, c) < 0);
}

double getAbsoluteAngle(position2d<double> _p1, position2d<double> _p2)
{
	double theta = atan(getSlope(_p1, _p2));
	double a = M_PI * PARITY((double)_p2.X - (double)_p1.X);
	return theta - a - 2*M_PI*PARITY(theta - a/2);
}

position2d<double> getMidpoint(position2d<double> _p1, position2d<double> _p2)
{
	return position2d<double>(_p2.X + (_p1.X - _p2.X) / 2, _p2.Y + (_p1.Y - _p2.Y) / 2);
}

double getNormal(position2d<double> _p1, position2d<double> _p2)
{
	return -1 / (getSlope(_p1, _p2));
}

double addAngle(double _rad1, double _rad2)
{
	_rad1 = _rad1 + _rad2;
	if (_rad1 >= 2 * M_PI)
		_rad1 -= 2 * M_PI;
	else if (_rad1 < 0.0)
		_rad1 += 2 * M_PI;
	return _rad1;
}

double rad2deg(double _rad)
{
	return _rad * 180 / M_PI;
}

double deg2rad(double _deg)
{
	return (_deg * M_PI) / 180.0;
}

double getCircleArea(double _radius)
{
	return M_PI*_radius*_radius;
}

// thanks http://jwilson.coe.uga.edu/EMAT6680Su12/Carreras/EMAT6690/Essay2/essay2.html
double getCircleOverlapArea(double _radius, double _dist)
{
	if (_dist == 0.0)
		return getCircleArea(_radius);
	else if (_radius == 0.0 || _radius*2.0 <= _dist)
		return 0.0;
	else
		return 2*_radius*_radius*acos(_dist / (2.0*_radius)) - (_dist / 4.0)*sqrt(4.0*_radius*_radius - _dist*_dist);
}

/// \TODO: This currently return only the first overlap detected. Do we need to keep checking for more in case of crazy sliders?
double GetHitobjectOverlapArea(Play *_play, int _indexA, int _indexB)
{
	std::vector<Hitobject>& hitobjects = _play->beatmap->getHitobjects();

	// out of bounds check
	if (!(BTWN(0, _indexA, hitobjects.size() - 1) && BTWN(0, _indexB, hitobjects.size() - 1)))
		return 0;

	// Load up the needed vars
	double diameter = CS2px(_play->getMod()->getCS());

	double startTimeA = hitobjects[_indexA].getTime();
	double startTimeB = hitobjects[_indexB].getTime();

	double endTimeA = hitobjects[_indexA].getEndTime();
	double endTimeB = hitobjects[_indexB].getEndTime();

	Hitobject::Slider* sliderA = nullptr, *sliderB = nullptr;
	
	if (hitobjects[_indexA].isHitobjectLong())	sliderA = &hitobjects[_indexA].slider;
	if (hitobjects[_indexB].isHitobjectLong())	sliderB = &hitobjects[_indexB].slider;

	// if it's a non sliders, then just iterate by 1 ms
	double velocityA, velocityB;
	if (sliderA == nullptr)	velocityA = 1.0;
	else					velocityA = sliderA->getVelocity();
	
	if (sliderB == nullptr)	velocityB = 1.0;
	else					velocityB = sliderB->getVelocity();

	// divide by 0 protection
	if (velocityA == 0.0) velocityA = 1.0;
	if (velocityB == 0.0) velocityB = 1.0;

	double area = 0.0;
	double currDistA = INT_MAX, prevDistA = INT_MAX;
	for (double timeA = startTimeA; timeA <= endTimeA; timeA += (diameter / 2.0*velocityA))
	{
		prevDistA = currDistA;

		double currDistB = INT_MAX, prevDistB = INT_MAX;
		for (double timeB = startTimeB; timeB <= endTimeB; timeB += (diameter / 2.0*velocityB))
		{
			// set the points to calculate
			vector2d<double> pointA, pointB;
			if (sliderA == nullptr)	pointA = hitobjects[_indexA].getPos();
			else					pointA = sliderA->GetSliderPos(timeA);

			if (sliderB == nullptr)	pointB = hitobjects[_indexB].getPos();
			else					pointB = sliderB->GetSliderPos(timeB);

			// Calculate distance
			prevDistB = currDistB;
			currDistB = getDist(pointA, pointB);

			// If this is not a slider, then there is nothing to iterate over
			// There is also no previous calculation, so take currDistB instead
			if (sliderB == nullptr)
			{
				currDistA = currDistB;
				break;
			}

			// Find closest point between the 2 object when iterating over object B
			if (currDistB > prevDistB)
			{
				currDistA = prevDistB;
				break;
			}
		}

		// if this is not a slider, then there is nothing to iterate over
		if (sliderA == nullptr)
		{
			// Since this is a point, there is no previous distance measurement
			area = getCircleOverlapArea(diameter / 2.0, currDistA);
			break;
		}
	
		// Find closest point between the 2 object when iterating over object A
		if (currDistA > prevDistA)
		{
			// The previous distance measurement is actaully the closer one
			area = getCircleOverlapArea(diameter / 2.0, prevDistA);
			break;
		}
	}

	return area;
}