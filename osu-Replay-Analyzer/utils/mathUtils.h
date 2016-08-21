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


#endif