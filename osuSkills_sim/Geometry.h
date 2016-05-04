#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <tuple>

#include "irrlicht/include/irrlicht.h"
using namespace irr;
using namespace core;

double getSlope(position2di _p1, position2di _p2);
double getDist(position2di _p1, position2di _p2);

bool isPointOnLine(position2di _point, std::pair<position2di, position2di> _line);
bool HasIntersectionPoint(std::pair<position2di, position2di> _line1, std::pair<position2di, position2di> _line2);

double Deg2px(double _deg, double _res, double _fov);
double px2Deg(double _px, double _res, double _fov);

double getVelocity(position2di _p1, position2di _p2, double _t1, double _t2);

// Gets the directional angle in rads (-pi -> pi)
double getDirAngle(position2di a, position2di b, position2di c);

// Gets the normal angle in rads (0 -> pi)
double getAngle(position2di _p1, position2di _ref, position2di _p3);
double getAbsoluteAngle(position2di _p1, position2di _p2);

position2di getMidpoint(position2di _p1, position2di _p2);
double getNormal(position2di _p1, position2di _p2);

double addAngle(double _rad1, double rad2);

double rad2deg(double _rad); 
double deg2rad(double _deg);



#endif

