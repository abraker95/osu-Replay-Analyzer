#include "osuCalc.h"
#include "mathUtils.h"

// diameter
double CS2px(double _CS)
{
	return 109 - 9 * _CS;
}

// diameter
double px2CS(double _px)
{
	return (109 - _px) / 9;
}

double AR2ms(double _AR)
{
	if (_AR <= 5) return 1800 - 120 * _AR;
	else		  return 1950 - 150 * _AR;
}

double ms2AR(double _ms)
{
	if (_ms >= 1200.0) return (1800 - _ms) / 120.0;
	else			   return (1950 - _ms) / 150.0;
}

// for 300 score
double OD2ms(double _OD)
{
	return 79.5 - 6.0 * _OD;
}

// for 300 score
double ms2OD(double _ms)
{
	return (79.5 - _ms) / 6.0;
}

// Slider osu!px/s
double SV2px_s(double _SV, double _BPM)
{
	return (_BPM / 60.0) * _SV * 100;
}

// Approach Circle at t
double ApCS2px_ps(double _CS, double _AR, double _t)
{
	return 4 * CS2px(_CS) - 3 * CS2px(_CS) * (_t / MAX(800, AR2ms(_AR)));
}