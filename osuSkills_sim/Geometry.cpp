#include "Geometry.h" 
#include "mathUtils.h"
#include <math.h>

double getSlope(position2di _p1, position2di _p2)
{
	if ((_p2.X - _p1.X) == 0)
		return DBL_MAX;
	else
		return ((double)(_p2.Y - _p1.Y)) / ((double)(_p2.X - _p1.X));
}

double getDist(position2di _p1, position2di _p2)
{
	double distX = _p2.X - _p1.X;
	double distY = _p2.Y - _p1.Y;
	return sqrt((double)(distX*distX + distY*distY));
}

double px2Deg(double _px, double _res, double _fov)
{
	return rad2deg(2.0 * tan(_px / _res * atan(deg2rad(_fov) / 2.0)));
}

double Deg2px(double _deg, double _res, double _fov)
{
	return _res * (atan(deg2rad(_deg) / 2.0) / atan(deg2rad(_fov) / 2.0));
}

double getVelocity(position2di _p1, position2di _p2, double _t1, double _t2)
{
	double dist = getDist(_p1, _p2);
	double time = _t2 - _t1;
	return dist / time;
}

double getAngle(position2di _p1, position2di _ref, position2di _p2)
{
	double a = (_ref.X - _p1.X)*(_ref.X - _p1.X) + (_ref.Y - _p1.Y)*(_ref.Y - _p1.Y);
	double b = (_ref.X - _p2.X)*(_ref.X - _p2.X) + (_ref.Y - _p2.Y)*(_ref.Y - _p2.Y);
	double c = (_p2.X - _p1.X)*(_p2.X - _p1.X) + (_p2.Y - _p1.Y)*(_p2.Y - _p1.Y);

	if (a == 0 || b == 0)
		return 0.0;
	else
		return acos((a + b - c) / sqrt(4 * a*b));
}

double getAbsoluteAngle(position2di _p1, position2di _p2)
{
	double theta = atan(getSlope(_p1, _p2));
	double a = M_PI * PARITY((double)_p2.X - (double)_p1.X);
	return theta - a - 2*M_PI*PARITY(theta - a/2);
}

position2di getMidpoint(position2di _p1, position2di _p2)
{
	return position2di(_p2.X + (_p1.X - _p2.X) / 2, _p2.Y + (_p1.Y - _p2.Y) / 2);
}

double getNormal(position2di _p1, position2di _p2)
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