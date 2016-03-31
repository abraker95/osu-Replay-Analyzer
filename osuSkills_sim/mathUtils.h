#ifndef MATHUTILS_H
#define MATHUTILS_H

#define	M_PI	(3.14159265358979323846)
#define MAX(a, b) (a >= b? a: b)

static inline double MIN(double a, double b)
{
	return (a <= b ? a : b);
}

static inline bool BTWN(double lss, double val, double gtr)
{
	return ((lss <= val) && (val <= gtr));
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