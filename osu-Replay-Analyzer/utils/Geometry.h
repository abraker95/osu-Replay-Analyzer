#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <tuple>
#include "../irrlicht/include/irrlicht.h"

using namespace irr;
using namespace core;

double getSlope(position2d<double> _p1, position2d<double> _p2);
double getDist(position2d<double> _p1, position2d<double> _p2);

bool isPointOnLine(position2d<double> _point, std::pair<position2d<double>, position2d<double>> _line);
bool HasIntersectionPoint(std::pair<position2d<double>, position2d<double>> _line1, std::pair<position2d<double>, position2d<double>> _line2);

double Deg2px(double _deg, double _res, double _fov);
double px2Deg(double _px, double _res, double _fov);

double getVelocity(position2d<double> _p1, position2d<double> _p2, double _t1, double _t2);

enum DIR
{
	CCW = 0,
	CW = 1
};

// Gets the directional angle in rads (-pi -> pi)
double getDirAngle(position2d<double> a, position2d<double> b, position2d<double> c);

// Gets the normal angle in rads (0 -> pi)
double getAngle(position2d<double> _p1, position2d<double> _ref, position2d<double> _p3);

// Gets the angular direction of a vector
double getAbsoluteAngle(position2d<double> _p1, position2d<double> _p2);

// Get the rotational direction
DIR getRotDir(position2d<double> _p1, position2d<double> _ref, position2d<double> _p3);

position2d<double> getMidpoint(position2d<double> _p1, position2d<double> _p2);
double getNormal(position2d<double> _p1, position2d<double> _p2);

double addAngle(double _rad1, double rad2);

double rad2deg(double _rad); 
double deg2rad(double _deg);

double getCircleArea(double _radius);
double getCircleOverlapArea(double _radius, double _dist);

#endif

