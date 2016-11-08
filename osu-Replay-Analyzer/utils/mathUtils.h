#ifndef MATHUTILS_H
#define MATHUTILS_H

#include <algorithm>
#include <stdint.h>
#include <vector>

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
	return ((MIN(lss, gtr) <= val) && (val <= MAX(lss, gtr)));
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

static inline double SIGN(double val)
{
	return ABS(val) / val;
}


// Linear interpolation
// Percent: 0.0 -> 1.0
static inline double lerp(double _low, double _hi, double _percent)
{
	return _low * (1 - _percent) + _hi * _percent;
}

static long binomialCoefficient(int n, int k)
{
	if (k < 0 || k > n)		return 0;
	if (k == 0 || k == n)	return 1;

	k = std::fmin(k, n - k);  // take advantage of symmetry
	long c = 1;
	for (int i = 0; i < k; i++)
		c = c * (n - i) / (i + 1);

	return c;
}

static inline double bernstein(int i, int n, double t)
{
	return binomialCoefficient(n, i) * pow(t, i) * pow(1 - t, n - i);
}

// Thanks http://llvm.org/docs/doxygen/html/LEB128_8h_source.html
static uint64_t decodeULEB128(const uint8_t *p, unsigned *n = nullptr)
{
	const uint8_t *orig_p = p;
	uint64_t Value = 0;
	unsigned Shift = 0;
	do
	{
		Value += uint64_t(*p & 0x7f) << Shift;
		Shift += 7;
	} while (*p++ >= 128);

	return Value;
}

template <typename T>
static std::vector<T> Merge(std::vector<T> first, std::vector<T> second)
{
	for (T x : second)
		first.push_back(x);

	return first;
}

// Guassian function
// Param: _val = x value
//		  _sigma = controls width
//		  _norm = normalization flag (makes peak = 1.0)
static double Guassian(double _val, double _sigma, bool _norm = true)
{
	double guass = 1.0 / (2.0 * _sigma*sqrt(M_PI))*exp(-((_val / (2.0*_sigma))*(_val / (2.0*_sigma))));

	if(_norm)	return guass / Guassian(0, _sigma, false);
	else		return guass;
}

// Returns a triangle wave function
// Param: _val = x value
//		  _amp = amplitude
static double Triangle(double _val, double _amp)
{
	return ABS((fmod(_val + (_amp / 2.0), _amp)) - (_amp / 2.0));
}

// Average
static double Avg(std::vector<double>& _vals)
{
	double sum = 0.0;
	for (double val : _vals) sum += val;

	return sum / (double)_vals.size();
}

// Circular Average
// Thanks http://webspace.ship.edu/pgmarr/Geo441/Lectures/Lec%2016%20-%20Directional%20Statistics.pdf
static double CirclularAvg(std::vector<double>& _rads)
{
	double sumX = 0.0, sumY = 0.0;
	for (double rad : _rads)
	{
		sumX += cos(rad);
		sumY += sin(rad);
	}

	return atan2(sumY, sumX);
}

// Circular Average
// Thanks http://webspace.ship.edu/pgmarr/Geo441/Lectures/Lec%2016%20-%20Directional%20Statistics.pdf
static double CirclularAvg(std::vector<double>& _x, std::vector<double>& _y)
{
	double sumX = 0.0, sumY = 0.0;

	for (double x : _x) sumX += x;
	for (double y : _y) sumY += y;

	return atan2(sumY, sumX);
}

// Variance
static double Var(std::vector<double>& _vals)
{
	double avg = Avg(_vals);

	double sum = 0.0;
	for (double val : _vals) sum += ((val - avg) * (val - avg));

	return sum / (double)_vals.size();
}

// Circular Variance
// Thanks http://webspace.ship.edu/pgmarr/Geo441/Lectures/Lec%2016%20-%20Directional%20Statistics.pdf
static double CirclularVar(std::vector<double>& _rads)
{
	double sumX = 0.0, sumY = 0.0;
	for (double rad : _rads)
	{
		sumX += cos(rad);
		sumY += sin(rad);
	}

	return 1.0 - (sqrt((sumY*sumY + sumX*sumX))/_rads.size());
}

// Circular Variance
// Thanks http://webspace.ship.edu/pgmarr/Geo441/Lectures/Lec%2016%20-%20Directional%20Statistics.pdf
static double CirclularVar(std::vector<double>& _x, std::vector<double>& _y)
{
	double sumX = 0.0, sumY = 0.0;

	for (double x : _x) sumX += x;
	for (double y : _y) sumY += y;

	return 1.0 - (sqrt((sumY*sumY + sumX*sumX)) / _x.size());
}

// Standard Deviation
static double StdDev(std::vector<double>& _vals)
{
	return sqrt(Var(_vals));
}

#endif