#ifndef MATHUTILS_H
#define MATHUTILS_H

#define	M_PI	(3.14159265358979323846)

static inline double MAX(double a, double b)
{
	return (a >= b ? a : b);
}

static inline double MIN(double a, double b)
{
	return (a <= b ? a : b);
}

static inline double BOUND(double _min, double _val, double _max)
{
	return MIN(MAX(_val, _min), _max);
}

static inline bool BTWN(double lss, double val, double gtr)
{
	return ((lss <= val) && (val <= gtr));
}

static inline double getPercent(double _min, double _val, double _max)
{
	return 1.0 - ((_max - BOUND(_min, _val, _max)) / (_max - _min));
}

static inline double getValue(double _min, double _max, double _percent)
{
	return  _max - (1.0 - _percent)*(_max - _min);
}

static inline double PX_PER_RAD(double radius)
{
	return (M_PI / (4.0*radius));
}

static inline double ABS(double val)
{
	return (val < 0 ? -val : val);
}

static inline double PARITY(double val)
{
	return ((ABS(val) / val - 1) / 2);
}


#endif